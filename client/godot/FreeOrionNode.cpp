#include "FreeOrionNode.h"

#include <codecvt>

#include <boost/xpressive/xpressive.hpp>
#include <boost/uuid/nil_generator.hpp>

#include <OS.hpp>

#include "GodotClientApp.h"
#include "GodotFleet.h"
#include "GodotSystem.h"

#include "../ClientNetworking.h"
#include "../../combat/CombatLogManager.h"
#include "../../Empire/Empire.h"
#include "../../network/Message.h"
#include "../../universe/Fleet.h"
#include "../../universe/System.h"
#include "../../util/Directories.h"
#include "../../util/GameRules.h"
#include "../../util/i18n.h"
#include "../../util/Version.h"

std::atomic_bool quit(false);

// Copied from ChatWnd.cpp
namespace {
    std::string UserStringSubstitute(const boost::xpressive::smatch& match) {
        auto key = match.str(1);

        if (match.nested_results().empty()) {
            // not parameterized
            if (UserStringExists(key))
                return UserString(key);
            return key;
        }

        if (UserStringExists(key))
            // replace key with user string if such exists
            key = UserString(key);

        auto formatter = FlexibleFormat(key);

        size_t arg = 1;
        for (auto submatch : match.nested_results())
            formatter.bind_arg(arg++, submatch.str());

        return formatter.str();
    }

    // finds instances of stringtable substitutions and/or string formatting
    // within the text \a input and evaluates them. [[KEY]] will be looked up
    // in the stringtable, and if found, replaced with the corresponding
    // stringtable entry. If not found, KEY is used instead. [[KEY,var1,var2]]
    // will look up KEY in the stringtable or use just KEY if there is no
    // such stringtable entry, and then substitute var1 for all instances of %1%
    // in the string, and var2 for all instances of %2% in the string. Any
    // intance of %3% or higher numbers will be deleted from the string, unless
    // a third or more parameters are specified.
    std::string StringtableTextSubstitute(const std::string& input) {
        using namespace boost::xpressive;

        sregex param = (s1 = +_w);
        sregex regex = as_xpr("[[") >> (s1 = +_w) >> !*(',' >> param) >> "]]";

        return regex_replace(input, regex, UserStringSubstitute);
    }
}


void FreeOrionNode::_register_methods() {
    register_method("_exit_tree", &FreeOrionNode::_exit_tree);

    register_method("new_single_player_game", &FreeOrionNode::new_single_player_game);
    register_method("network_thread", &FreeOrionNode::network_thread);
    register_method("get_version", &FreeOrionNode::get_version);
    register_method("is_server_connected", &FreeOrionNode::is_server_connected);
    register_method("connect_to_server", &FreeOrionNode::connect_to_server);
    register_method("join_game", &FreeOrionNode::join_game);
    register_method("auth_response", &FreeOrionNode::auth_response);
    register_method("get_systems", &FreeOrionNode::get_systems);
    register_method("get_fleets", &FreeOrionNode::get_fleets);

    godot::register_signal<FreeOrionNode>("ping", "message", GODOT_VARIANT_TYPE_STRING);
    godot::register_signal<FreeOrionNode>("error", "problem", GODOT_VARIANT_TYPE_STRING, "fatal", GODOT_VARIANT_TYPE_BOOL);
    godot::register_signal<FreeOrionNode>("auth_request", "player_name", GODOT_VARIANT_TYPE_STRING, "auth", GODOT_VARIANT_TYPE_STRING);
    godot::register_signal<FreeOrionNode>("empire_status", "status", GODOT_VARIANT_TYPE_INT, "about_empire_id", GODOT_VARIANT_TYPE_INT);
    godot::register_signal<FreeOrionNode>("start_game", "is_new_game", GODOT_VARIANT_TYPE_BOOL);
    // ToDo: implement timestamps
    godot::register_signal<FreeOrionNode>("chat_message", "text", GODOT_VARIANT_TYPE_STRING, "player_name", GODOT_VARIANT_TYPE_STRING, "text_color", GODOT_VARIANT_TYPE_COLOR, "pm", GODOT_VARIANT_TYPE_BOOL);
}

FreeOrionNode::FreeOrionNode()
{}

FreeOrionNode::~FreeOrionNode()
{}

void FreeOrionNode::_init() {
#if defined(FREEORION_ANDROID)
    SetAndroidEnvironment(s_android_api_struct->godot_android_get_env(),
                          s_android_api_struct->godot_android_get_activity());
#endif
    std::string executable_path = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.to_bytes(godot::OS::get_singleton()->get_executable_path().unicode_str());

    InitDirs(executable_path);

#ifdef FREEORION_WIN32
    GetOptionsDB().Add<std::string>("misc.server-local-binary.path", UserStringNop("OPTIONS_DB_FREEORIOND_PATH"),   PathToString(GetBinDir() / "freeoriond.exe"));
#else
    GetOptionsDB().Add<std::string>("misc.server-local-binary.path", UserStringNop("OPTIONS_DB_FREEORIOND_PATH"),   PathToString(GetBinDir() / "freeoriond"));
#endif

    GetOptionsDB().SetFromFile(GetConfigPath(), FreeOrionVersionString());
    GetOptionsDB().SetFromFile(GetPersistentConfigPath());

#if !defined(FREEORION_ANDROID)
    std::vector<std::string> args;
    args.emplace_back(std::move(executable_path));
    const godot::PoolStringArray wargs = godot::OS::get_singleton()->get_cmdline_args();
    const godot::PoolStringArray::Read wargs_read = wargs.read();
    for (int i = 0; i < wargs.size(); ++ i) {
        const std::string arg = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.to_bytes(wargs_read[i].unicode_str());
        // Exclude Godot's options
        if (arg != "-s" && arg.rfind("-g", 0) != 0) {
            args.emplace_back(std::move(arg));
        }
    }

    // override previously-saved and default options with command line parameters and flags
    GetOptionsDB().SetFromCommandLine(args);
#endif

    CompleteXDGMigration();

#if !defined(FREEORION_ANDROID)
    // Handle the case where the resource.path does not exist anymore
    // gracefully by resetting it to the standard path into the
    // application bundle.  This may happen if a previous installed
    // version of FreeOrion was residing in a different directory.
    if (!boost::filesystem::exists(GetResourceDir()) ||
        !boost::filesystem::exists(GetResourceDir() / "credits.xml") ||
        !boost::filesystem::exists(GetResourceDir() / "data" / "art" / "misc" / "missing.png"))
    {
        DebugLogger() << "Resources directory " << PathToString(GetResourceDir()) << " from config.xml missing or does not contain expected files. Resetting to default.";

        GetOptionsDB().Set<std::string>("resource.path", PathToString(boost::filesystem::canonical("../default"))); // Temporary default for Godot client prototype development

        // double-check that resetting actually fixed things...
        if (!boost::filesystem::exists(GetResourceDir()) ||
            !boost::filesystem::exists(GetResourceDir() / "credits.xml") ||
            !boost::filesystem::exists(GetResourceDir() / "data" / "art" / "misc" / "missing.png"))
        {
            DebugLogger() << "Default Resources directory missing or does not contain expected files. Cannot start game.";
            throw std::runtime_error("Unable to load game resources at default location: " +
                                     PathToString(GetResourceDir()) + " : Install may be broken.");
        }
    }
#endif

    app = std::make_unique<GodotClientApp>();
}


#if defined(FREEORION_ANDROID)
const godot_gdnative_ext_android_api_struct* FreeOrionNode::s_android_api_struct = nullptr;

void FreeOrionNode::set_android_api_struct(const godot_gdnative_ext_android_api_struct *android_api_struct)
{ s_android_api_struct = android_api_struct; }
#endif

void FreeOrionNode::_exit_tree() {
    quit = true;
    app.reset();

    ShutdownLoggingSystemFileSink();
}


void FreeOrionNode::HandleMessage(Message&& msg) {
    try {
        switch (msg.Type()) {
        case Message::MessageType::AUTH_REQUEST: {
            std::string player_name;
            std::string auth;
            ExtractAuthRequestMessageData(msg, player_name, auth);
            emit_signal("auth_request", godot::String(player_name.c_str()), godot::String(auth.c_str()));
            break;
        }
        case Message::MessageType::SET_AUTH_ROLES: {
            ExtractSetAuthorizationRolesMessage(msg, app->Networking().AuthorizationRoles());
            break;
        }
        case Message::MessageType::JOIN_GAME: {
            int player_id;
            boost::uuids::uuid cookie;
            ExtractJoinAckMessageData(msg, player_id, cookie);
            app->Networking().SetPlayerID(player_id);
            break;
        }
        case Message::MessageType::HOST_ID: {
            const std::string& text = msg.Text();
            int host_id = Networking::INVALID_PLAYER_ID;
            try {
                host_id = boost::lexical_cast<int>(text);
            } catch (const boost::bad_lexical_cast& ex) {
                ErrorLogger() << "GDFreeOrion::handle_message(HOST_ID) could not convert \"" << text << "\" to host id";
            }

            app->Networking().SetHostPlayerID(host_id);
            break;
        }
        case Message::MessageType::PLAYER_STATUS: {
            Message::PlayerStatus status;
            int about_empire_id;
            ExtractPlayerStatusMessageData(msg, status, about_empire_id);
            emit_signal("empire_status", static_cast<int>(status), about_empire_id);
            break;
        }
        case Message::MessageType::HOST_SP_GAME: {
            try {
                int host_id = boost::lexical_cast<int>(msg.Text());

                app->Networking().SetPlayerID(host_id);
                app->Networking().SetHostPlayerID(host_id);
            } catch (const boost::bad_lexical_cast& ex) {
                ErrorLogger() << "GDFreeOrion::handle_message(HOST_ID) Host id " << msg.Text() << " is not a number: " << ex.what();
            }
            break;
        }
        case Message::MessageType::GAME_START: {
            bool loaded_game_data;
            bool ui_data_available;
            SaveGameUIData ui_data;
            bool save_state_string_available;
            std::string save_state_string; // ignored - used by AI but not by human client
            OrderSet orders;
            bool single_player_game = false;
            int empire_id = ALL_EMPIRES;
            int current_turn = INVALID_GAME_TURN;
            app->Orders().Reset();
            try {
                ExtractGameStartMessageData(msg,                 single_player_game,             empire_id,
                                            current_turn,        Empires(),                      GetUniverse(),
                                            GetSpeciesManager(), GetCombatLogManager(),          GetSupplyManager(),
                                            app->Players(),      app->Orders(),              loaded_game_data,
                                            ui_data_available,   ui_data,                        save_state_string_available,
                                            save_state_string,   app->GetGalaxySetupData());
            } catch (...) {
                return;
            }

            DebugLogger() << "Extracted GameStart message for turn: " << current_turn << " with empire: " << empire_id;

            app->SetSinglePlayerGame(single_player_game);
            app->SetEmpireID(empire_id);
            app->SetCurrentTurn(current_turn);

            GetGameRules().SetFromStrings(app->GetGalaxySetupData().GetGameRules());

            bool is_new_game = !(loaded_game_data && ui_data_available);
            emit_signal("start_game", is_new_game);
            break;
        }
        case Message::MessageType::ERROR_MSG: {
            std::string problem;
            bool fatal;
            int player_id;
            ExtractErrorMessageData(msg, player_id, problem, fatal);
            emit_signal("error", godot::String(problem.c_str()), fatal);
            break;
        }
        case Message::MessageType::CHAT_HISTORY: {
            std::vector<ChatHistoryEntity> chat_history;
            ExtractChatHistoryMessage(msg, chat_history);

            for (const auto& elem : chat_history) {
                emit_signal("chat_message",
                            godot::String(StringtableTextSubstitute(elem.text).c_str()),
                            godot::String(elem.player_name.c_str()),
                            elem.player_name.empty() ?
                                godot::Color(1.0f, 1.0f, 1.0f, 1.0f) :
                                godot::Color(std::get<0>(elem.text_color) / 255.0f,
                                             std::get<1>(elem.text_color) / 255.0f,
                                             std::get<2>(elem.text_color) / 255.0f,
                                             std::get<3>(elem.text_color) / 255.0f),
                            false);
            }

            break;
        }
        case Message::MessageType::PLAYER_CHAT: {
            int sending_player_id;
            boost::posix_time::ptime timestamp;
            std::string data;
            bool pm;
            ExtractServerPlayerChatMessageData(msg, sending_player_id, timestamp, data, pm);

            std::string player_name{UserString("PLAYER") + " " + std::to_string(sending_player_id)};
            godot::Color text_color{1.0f, 1.0f, 1.0f, 1.0f};
            if (sending_player_id != Networking::INVALID_PLAYER_ID) {
                const auto& players = app->Players();
                auto player_it = players.find(sending_player_id);
                if (player_it != players.end()) {
                    if (auto empire = GetEmpire(player_it->second.empire_id))
                        text_color = godot::Color(std::get<0>(empire->Color()) / 255.0f,
                                                  std::get<1>(empire->Color()) / 255.0f,
                                                  std::get<2>(empire->Color()) / 255.0f,
                                                  std::get<3>(empire->Color()) / 255.0f);
                }
            } else {
                // It's a server message. Don't set player name.
                player_name.clear();
            }

            emit_signal("chat_message",
                        godot::String(StringtableTextSubstitute(data).c_str()),
                        godot::String((pm ? player_name + UserString("MESSAGES_WHISPER") : player_name).c_str()),
                        text_color,
                        pm);

            break;
        }
        default:
            std::ostringstream stream;
            stream << msg.Type();
            emit_signal("ping", godot::String(stream.str().c_str()));
        }
    } catch (const std::exception& e) {
        ErrorLogger() << "GDFreeOrion::handle_message : Exception while reacting to message of type \""
                      << msg.Type() << "\". what: " << e.what();
    }
}

void FreeOrionNode::network_thread() {
    while(!quit) {
        if (auto msg = this->app->Networking().GetMessage()) {
            this->HandleMessage(std::move(*msg));
        } else {
            godot::OS::get_singleton()->delay_msec(20);
        }
    }
}

void FreeOrionNode::new_single_player_game() {
#ifdef FREEORION_ANDROID
    ErrorLogger() << "No single player game supported";
#else
    app->NewSinglePlayerGame();
#endif
}

godot::String FreeOrionNode::get_version() const {
    return godot::String(FreeOrionVersionString().c_str());
}

bool FreeOrionNode::is_server_connected() const {
    return app->Networking().IsConnected();
}

bool FreeOrionNode::connect_to_server(godot::String dest) {
    std::string dest8 = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.to_bytes(dest.unicode_str());
    return app->Networking().ConnectToServer(dest8);
}

void FreeOrionNode::join_game(godot::String player_name, int client_type) {
    std::string player_name8 = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.to_bytes(player_name.unicode_str());
    app->Networking().SendMessage(JoinGameMessage(player_name8, static_cast<Networking::ClientType>(client_type), boost::uuids::nil_uuid()));
}

void FreeOrionNode::auth_response(godot::String player_name, godot::String password) {
    std::string player_name8 = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.to_bytes(player_name.unicode_str());
    std::string password8 = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.to_bytes(password.unicode_str());
    app->Networking().SendMessage(AuthResponseMessage(player_name8, password8));
}

godot::Dictionary FreeOrionNode::get_systems() const {
    godot::Dictionary systems;
    for (const auto& sys : Objects().all<System>()) {
        systems[sys->ID()] = GodotSystem::Wrap(sys);
    }
    return systems;
}

godot::Dictionary FreeOrionNode::get_fleets() const {
    godot::Dictionary fleets;
    for (const auto& fleet : Objects().all<Fleet>()) {
        fleets[fleet->ID()] = GodotFleet::Wrap(fleet);
    }
    return fleets;
}

