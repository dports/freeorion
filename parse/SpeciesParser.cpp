#include "Parse.h"

#include "ParseImpl.h"
#include "EnumParser.h"
#include "ValueRefParser.h"
#include "EnumValueRefRules.h"
#include "EffectParser.h"
#include "ConditionParserImpl.h"
#include "MovableEnvelope.h"

#include "../universe/Condition.h"
#include "../universe/Effect.h"
#include "../universe/Species.h"
#include "../util/Directories.h"

#include <boost/spirit/include/phoenix.hpp>


#define DEBUG_PARSERS 0

#if DEBUG_PARSERS
namespace std {
    inline ostream& operator<<(ostream& os, const FocusType&) { return os; }
    inline ostream& operator<<(ostream& os, const std::vector<FocusType>&) { return os; }
    inline ostream& operator<<(ostream& os, const parse::effects_group_payload&) { return os; }
    inline ostream& operator<<(ostream& os, const std::pair<PlanetType, PlanetEnvironment>&) { return os; }
    inline ostream& operator<<(ostream& os, const std::pair<const PlanetType, PlanetEnvironment>&) { return os; }
    inline ostream& operator<<(ostream& os, const std::map<PlanetType, PlanetEnvironment>&) { return os; }
    inline ostream& operator<<(ostream& os, const std::pair<const std::string, std::unique_ptr<Species>>&) { return os; }
    inline ostream& operator<<(ostream& os, const std::map<std::string, std::unique_ptr<Species>>&) { return os; }
}
#endif

namespace {
    const boost::phoenix::function<parse::detail::is_unique> is_unique_;

    struct SpeciesStrings {
        SpeciesStrings() = default;
        SpeciesStrings(std::string& name_, std::string& desc_,
                       std::string& gameplay_desc_) :
            name(std::move(name_)),
            desc(std::move(desc_)),
            gameplay_desc(std::move(gameplay_desc_))
        {}

        std::string name;
        std::string desc;
        std::string gameplay_desc;
    };

    struct SpeciesParamsAndStuff {
        SpeciesParamsAndStuff() = default;
        SpeciesParamsAndStuff(bool playable_, bool native_,
                              bool can_colonize_, bool can_produce_ships_,
                              boost::optional<std::vector<FocusType>>& foci_,
                              boost::optional<std::string>& preferred_focus_,
                              std::set<std::string>& tags_,
                              std::set<std::string>& likes_,
                              std::set<std::string>& dislikes_) :
            playable(playable_),
            native(native_),
            can_colonize(can_colonize_),
            can_produce_ships(can_produce_ships_),
            foci(std::move(foci_)),
            preferred_focus(std::move(preferred_focus_)),
            tags(std::move(tags_)),
            likes(std::move(likes_)),
            dislikes(std::move(dislikes_))
        {}
        bool                                    playable = false;
        bool                                    native = false;
        bool                                    can_colonize = false;
        bool                                    can_produce_ships = false;
        boost::optional<std::vector<FocusType>> foci;
        boost::optional<std::string>            preferred_focus;
        std::set<std::string>                   tags;
        std::set<std::string>                   likes;
        std::set<std::string>                   dislikes;
    };


    void insert_species(
        std::map<std::string, std::unique_ptr<Species>>& species,
        SpeciesStrings& strings,
        boost::optional<std::map<PlanetType, PlanetEnvironment>>& planet_environments,
        boost::optional<parse::effects_group_payload>& effects,
        boost::optional<parse::detail::MovableEnvelope<Condition::Condition>>& combat_targets,
        SpeciesParamsAndStuff& params,
        std::string& graphic,
        bool& pass)
    {
        auto species_ptr = std::make_unique<Species>(
            std::move(strings.name), std::move(strings.desc), std::move(strings.gameplay_desc),
            (params.foci ? std::move(*params.foci) : std::vector<FocusType>{}),
            (params.preferred_focus ? std::move(*params.preferred_focus) : std::string{}),
            (planet_environments ? std::move(*planet_environments) : std::map<PlanetType, PlanetEnvironment>{}),
            (effects ? std::move(OpenEnvelopes(*effects, pass)) : std::vector<std::unique_ptr<Effect::EffectsGroup>>{}),
            (combat_targets ? std::move((*combat_targets).OpenEnvelope(pass)) : nullptr),
            params.playable,
            params.native,
            params.can_colonize,
            params.can_produce_ships,
            params.tags,    // intentionally not moved
            std::move(params.likes),
            std::move(params.dislikes),
            std::move(graphic));

        species.emplace(species_ptr->Name(), std::move(species_ptr));
    }

    BOOST_PHOENIX_ADAPT_FUNCTION(void, insert_species_, insert_species, 8)

    using likes_rule_type    = parse::detail::rule<std::set<std::string> ()>;
    using likes_grammar_type = parse::detail::grammar<std::set<std::string> ()>;

    struct likes_grammar : public likes_grammar_type {
        likes_grammar(const parse::lexer& tok, parse::detail::Labeller& label) :
            likes_grammar::base_type(start, "likes_grammar"),
            one_or_more_string_tokens(tok)
        {
            start %= -(label(tok.Likes_) >>  one_or_more_string_tokens);
            start.name("Likes");
#if DEBUG_PARSERS
            debug(start);
#endif
        }

        likes_rule_type start;
        parse::detail::single_or_repeated_string<std::set<std::string>> one_or_more_string_tokens;
    };

    struct dislikes_grammar : public likes_grammar_type {
        dislikes_grammar(const parse::lexer& tok, parse::detail::Labeller& label) :
            dislikes_grammar::base_type(start, "dislikes_grammar"),
            one_or_more_string_tokens(tok)
        {
            start %= -(label(tok.Dislikes_) >>  one_or_more_string_tokens);
            start.name("Dislikes");
#if DEBUG_PARSERS
            debug(start);
#endif
        }

        likes_rule_type start;
        parse::detail::single_or_repeated_string<std::set<std::string>> one_or_more_string_tokens;
    };

    using start_rule_payload = std::pair<
        std::map<std::string, std::unique_ptr<Species>>, // species_by_name
        std::vector<std::string> // census ordering
    >;
    using start_rule_signature = void(start_rule_payload::first_type&);

    struct grammar : public parse::detail::grammar<start_rule_signature> {
        grammar(const parse::lexer& tok, const std::string& filename,
                const parse::text_iterator& first, const parse::text_iterator& last) :
            grammar::base_type(start),
            condition_parser(tok, label),
            string_grammar(tok, label, condition_parser),
            tags_parser(tok, label),
            likes(tok, label),
            dislikes(tok, label),
            effects_group_grammar(tok, label, condition_parser, string_grammar),
            one_or_more_foci(focus_type),
            planet_type_rules(tok, label, condition_parser),
            planet_environment_rules(tok, label, condition_parser)
        {
            namespace phoenix = boost::phoenix;
            namespace qi = boost::spirit::qi;

            using phoenix::construct;
            using phoenix::insert;
            using phoenix::push_back;

            qi::_1_type _1;
            qi::_2_type _2;
            qi::_3_type _3;
            qi::_4_type _4;
            qi::_5_type _5;
            qi::_6_type _6;
            qi::_7_type _7;
            qi::_8_type _8;
            qi::_9_type _9;
            qi::_pass_type _pass;
            qi::_r1_type _r1;
            qi::_val_type _val;
            qi::eps_type eps;
            qi::matches_type matches_;
            qi::omit_type omit_;
            qi::as_string_type as_string_;
            const boost::phoenix::function<parse::detail::deconstruct_movable> deconstruct_movable_;

            focus_type
                =  ( omit_[tok.Focus_]
                >    label(tok.Name_)        > tok.string
                >    label(tok.Description_) > tok.string
                >    label(tok.Location_)    > condition_parser
                >    label(tok.Graphic_)     > tok.string
                ) [ _val = construct<FocusType>(_1, _2, deconstruct_movable_(_3, _pass), _4) ]
                ;

            foci
                =    label(tok.Foci_)
                >    one_or_more_foci
                ;

            environment_map_element
                =  ( label(tok.Type_)        > planet_type_rules.enum_expr
                >    label(tok.Environment_) > planet_environment_rules.enum_expr
                ) [ _val = construct<std::pair<PlanetType, PlanetEnvironment>>(_1, _2) ]
                ;

            environment_map
                =    ('[' > +environment_map_element [ insert(_val, _1) ] > ']')
                |     environment_map_element [ insert(_val, _1) ]
                ;

            species_params_and_stuff
                =   (matches_[tok.Playable_]        // _1
                >    matches_[tok.Native_]          // _2
                >    matches_[tok.CanProduceShips_] // _3
                >    matches_[tok.CanColonize_]     // _4
                >    tags_parser                    // _5
                >   -foci                           // _6
                >   -as_string_[(label(tok.PreferredFocus_) > tok.string )] // _7
                >    likes                          // _8
                >    dislikes                       // _9
                    ) [ _val = construct<SpeciesParamsAndStuff>(_1, _2, _4, _3, _6, _7, _5, _8, _9) ]
                ;

            species_strings
                =  ( tok.Species_
                >    label(tok.Name_)                   > tok.string
                >    label(tok.Description_)            > tok.string
                >    label(tok.Gameplay_Description_)   > tok.string
                   ) [ _pass = is_unique_(_r1, _1, _2),
                       _val = construct<SpeciesStrings>(_2, _3, _4) ]
                ;

            species
                = ( species_strings(_r1)        // _1
                >   species_params_and_stuff    // _2
                > -(label(tok.EffectsGroups_)   >   effects_group_grammar)  // _3
                > -(label(tok.CombatTargets_)   >   condition_parser)       // _4
                > -(label(tok.Environments_)    >   environment_map)        // _5
                >   label(tok.Graphic_)         >   tok.string              // _6
                  ) [ insert_species_(_r1, _1, _5, _3, _4, _2, _6, _pass) ]
                ;

            start
                = +species(_r1)
                ;

            focus_type.name("Focus");
            foci.name("Foci");
            environment_map_element.name("Type = <type> Environment = <env>");
            environment_map.name("Environments");
            species_params_and_stuff.name("Species Flags, ");
            species_strings.name("Species Strings");
            species.name("Species");
            start.name("start");

#if DEBUG_PARSERS
            debug(focus_type);
            debug(foci);
            debug(environment_map_element);
            debug(environment_map);
            debug(species_params);
            debug(species_strings);
            debug(species);
            debug(start);
#endif

            qi::on_error<qi::fail>(start, parse::report_error(filename, first, last, _1, _2, _3, _4));
        }

        using focus_type_rule = parse::detail::rule<FocusType ()>;
        using foci_rule = parse::detail::rule<std::vector<FocusType> ()>;
        using environment_map_element_rule = parse::detail::rule<std::pair<PlanetType, PlanetEnvironment> ()>;
        using environment_map_rule = parse::detail::rule<std::map<PlanetType, PlanetEnvironment> ()>;
        using species_params_rule = parse::detail::rule<SpeciesParamsAndStuff ()>;
        using species_strings_rule = parse::detail::rule<SpeciesStrings (const start_rule_payload::first_type&)>;
        using species_rule = parse::detail::rule<void (start_rule_payload::first_type&)>;
        using start_rule = parse::detail::rule<start_rule_signature>;

        parse::detail::Labeller                                    label;
        const parse::conditions_parser_grammar                     condition_parser;
        const parse::string_parser_grammar                         string_grammar;
        parse::detail::tags_grammar                                tags_parser;
        likes_grammar                                              likes;
        dislikes_grammar                                           dislikes;
        parse::effects_group_grammar                               effects_group_grammar;
        foci_rule                                                  foci;
        focus_type_rule                                            focus_type;
        parse::detail::single_or_bracketed_repeat<focus_type_rule> one_or_more_foci;
        environment_map_element_rule                               environment_map_element;
        environment_map_rule                                       environment_map;
        species_params_rule                                        species_params_and_stuff;
        species_strings_rule                                       species_strings;
        species_rule                                               species;
        start_rule                                                 start;
        parse::detail::planet_type_parser_rules                    planet_type_rules;
        parse::detail::planet_environment_parser_rules             planet_environment_rules;
    };

    using manifest_start_rule_signature = void (std::vector<std::string>&);

    struct manifest_grammar : public parse::detail::grammar<manifest_start_rule_signature> {
        manifest_grammar(const parse::lexer& tok,
                         const std::string& filename,
                         const parse::text_iterator& first, const parse::text_iterator& last) :
            manifest_grammar::base_type(start)
        {
            namespace phoenix = boost::phoenix;
            namespace qi = boost::spirit::qi;

            using phoenix::push_back;

            qi::_1_type _1;
            qi::_2_type _2;
            qi::_3_type _3;
            qi::_4_type _4;
            qi::_r1_type _r1;
            qi::omit_type omit_;

            species_manifest
                =    omit_[tok.SpeciesCensusOrdering_]
                >    *(label(tok.Tag_) > tok.string [ push_back(_r1, _1) ])
                ;

            start
                =   +species_manifest(_r1)
                ;

            species_manifest.name("ParsedSpeciesCensusOrdering");

#if DEBUG_PARSERS
            debug(species_manifest);
#endif

            qi::on_error<qi::fail>(start, parse::report_error(filename, first, last, _1, _2, _3, _4));
        }

        using manifest_rule = parse::detail::rule<void (std::vector<std::string>&)>;
        using start_rule = parse::detail::rule<manifest_start_rule_signature>;

        parse::detail::Labeller label;
        manifest_rule species_manifest;
        start_rule start;
    };
}

namespace parse {
    start_rule_payload species(const boost::filesystem::path& path) {
        const lexer lexer;
        start_rule_payload::first_type species_;
        start_rule_payload::second_type ordering;

        boost::filesystem::path manifest_file;

        ScopedTimer timer("Species Parsing", true);

        for (const auto& file : ListDir(path, IsFOCScript)) {
            if (file.filename() == "SpeciesCensusOrdering.focs.txt" ) {
                manifest_file = file;
                continue;
            }

            detail::parse_file<grammar, start_rule_payload::first_type>(lexer, file, species_);
        }

        if (!manifest_file.empty()) {
            try {
                detail::parse_file<manifest_grammar, start_rule_payload::second_type>(
                    lexer, manifest_file, ordering);

            } catch (const std::runtime_error& e) {
                ErrorLogger() << "Failed to species census manifest in " << manifest_file << " from " << path
                              << " because " << e.what();
            }
        }

        return {std::move(species_), ordering};
    }
}
