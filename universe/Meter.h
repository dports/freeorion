#ifndef _Meter_h_
#define _Meter_h_

#include <array>
#include <cstdint>
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include "../util/Export.h"


/** A Meter is a value and associated initial value that is used to track information
  * about gamestate. A typical example is the population meter of a planet. */
class FO_COMMON_API Meter {
public:
    Meter() = default;
    constexpr explicit Meter(float v) noexcept :
        cur(Clamp(FromFloat(v))),
        init(Clamp(FromFloat(v)))
    {};
    constexpr Meter(float c, float i) noexcept :
        cur(Clamp(FromFloat(c))),
        init(Clamp(FromFloat(i)))
    {}

    [[nodiscard]] constexpr float Current() const noexcept { return FromInt(cur); };
    [[nodiscard]] constexpr float Initial() const noexcept { return FromInt(init); };

    [[nodiscard]] std::array<std::string::value_type, 64> Dump(uint8_t ntabs = 0) const noexcept; ///< returns text of meter values

    [[nodiscard]] constexpr bool operator==(const Meter& rhs) const noexcept
    { return cur == rhs.cur && init == rhs.init; }

    [[nodiscard]] constexpr bool operator<(const Meter& rhs) const noexcept
    { return cur < rhs.cur || (cur == rhs.cur && init < rhs.init); }

    constexpr void SetCurrent(float current_value) noexcept
    { cur = Clamp(FromFloat(current_value)); }

    constexpr void Set(float current_value, float initial_value) noexcept {
        cur = Clamp(FromFloat(current_value));
        init = Clamp(FromFloat(initial_value));
    }

    constexpr void ResetCurrent() noexcept { cur = DEFAULT_AS_INT; } // initial unchanged

    constexpr void Reset() noexcept {
        cur = DEFAULT_AS_INT;
        init = DEFAULT_AS_INT;
    }

    constexpr void AddToCurrent(float adjustment) noexcept
    { cur = Clamp(FromFloat(FromInt(cur) + adjustment)); }

    constexpr void ClampCurrentToRange(float min = DEFAULT_VALUE, float max = LARGE_VALUE) noexcept
    { cur = Clamp(cur, min, max); }

    constexpr void BackPropagate() noexcept { init = cur; }

    using ToCharsArrayT = std::array<std::string::value_type, 24>;
    [[nodiscard]] ToCharsArrayT ToChars() const;
    std::size_t ToChars(char* buffer, char* buffer_end) const;
    std::size_t SetFromChars(std::string_view chars);

    static constexpr float DEFAULT_VALUE = 0.0f;                        ///< value assigned to current or initial when resetting or when no value is specified in a constructor
    static constexpr float LARGE_VALUE = static_cast<float>(2 << 15);   ///< a very large number, which is useful to set current to when it will be later clamped, to ensure that the result is the max value in the clamp range
    static constexpr float INVALID_VALUE = -LARGE_VALUE;                ///< sentinel value to indicate no valid value for this meter

private:
    static constexpr float FLOAT_INT_SCALE = 1000.0f;
    // Value must be rounded, otherwise a calculated increase of 0.99999997 will be truncated to 0.999.
    // Negative values are increased by truncation, so the offset must be negative, too.
    static constexpr int32_t FromFloat(float f) { return static_cast<int32_t>(f * FLOAT_INT_SCALE + (f > 0 ? 0.5f : -0.5f)); }
    static constexpr float FromInt(int32_t i) { return i / FLOAT_INT_SCALE; }

    static constexpr int32_t DEFAULT_AS_INT = static_cast<int32_t>(DEFAULT_VALUE * FLOAT_INT_SCALE + 0.5f);
    static constexpr int32_t LARGE_AS_INT = static_cast<int32_t>(LARGE_VALUE * FLOAT_INT_SCALE + 0.5f);

    static constexpr int32_t Clamp(int32_t i, const int32_t min_as_int = -LARGE_AS_INT,
                                   const int32_t max_as_int = LARGE_AS_INT)
    {
        i = (max_as_int > i) ? i : max_as_int;
        i = (min_as_int < i) ? i : min_as_int;
        return i;
    }

    int32_t cur = DEFAULT_AS_INT;
    int32_t init = DEFAULT_AS_INT;

    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version);
};

BOOST_CLASS_VERSION(Meter, 2)


template <typename Archive>
void Meter::serialize(Archive& ar, const unsigned int version)
{
    if constexpr (Archive::is_loading::value) {
        if (version < 2) {
            float c = 0.0f, i = 0.0f;
            ar  & boost::serialization::make_nvp("c", c)
                & boost::serialization::make_nvp("i", i);
            cur = FromFloat(c);
            init = FromFloat(i);
            return;
        }
    }
    ar  & boost::serialization::make_nvp("c", cur)
        & boost::serialization::make_nvp("i", init);
}

namespace boost::archive {
    class xml_iarchive;
    class xml_oarchive;
}

template<> void Meter::serialize(boost::archive::xml_iarchive&, const unsigned int version);
template<> void Meter::serialize(boost::archive::xml_oarchive&, const unsigned int version);

#endif
