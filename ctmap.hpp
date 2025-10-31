#include <utility>
#include <array>
#include "ctarr_utils.hpp"

template <unsigned long Size>
struct Cstring
{
    constexpr Cstring() : c_str{0}, is_empty{true} {};
    constexpr Cstring(const char *str)
    {
        unsigned long idx = 0;
        const char *p = str;

        while (*p != '\0' && idx < Size)
        {
            c_str[idx] = *p;
            ++idx;
            ++p;
        }

        is_empty = false;
    }

    constexpr ~Cstring() {}

    constexpr auto operator<=>(const Cstring& str) const
    {
        auto crc = this->get_crc32();
        auto strcrc = str.get_crc32();

        return (crc <=> strcrc);
    }    

    constexpr auto operator<(const Cstring& str) const
    {
        auto crc = this->get_crc32();
        auto strcrc = str.get_crc32();

        return (crc < strcrc);
    }

    constexpr unsigned int get_crc32(void) const
    {
        unsigned int crc = 0xFFFFFFFF;
        for (unsigned long i = 0; i < Size; ++i)
        {
            crc ^= static_cast<unsigned char>(c_str[i]);
            for (int i = 0; i < 8; ++i)
                crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
        }
        return ~crc;
    }

    bool is_empty = true;
    char c_str[Size] = {0};
};

template <typename Type>
struct Pair
{
    constexpr Pair() : crc{}, first{}, second{} {}

    constexpr Pair(const Cstring<64>& key, const Type& value, unsigned long crc)
    {
        this->first = key;
        this->second = value;
        this->crc = crc;
    }

    constexpr Pair(const Cstring<64>& key, const Type& value)
    {
        this->crc = key.get_crc32();
        this->first = key;
        this->second = value;
    }

    constexpr Pair(const Pair& pair)
    {
        this->crc = pair.crc;
        this->first = pair.first;
        this->second = pair.second;
    }

    constexpr auto operator<=>(const Pair& pair) const
    {
        return (this->crc <=> pair.crc);
    }

    constexpr auto operator<(const Pair& pair) const
    {
        return (this->crc < pair.crc);
    }

    constexpr auto operator%(const unsigned long val) const
    {
        return Pair<Type>(first, second, crc % val);
    }

    unsigned int crc{};
    Cstring<64> first{};
    Type second{};
}; 

template <typename Type, unsigned long Size, Pair<Type>... Pairs>
struct Map
{
    using type = ctarray_sort_t<Pair<Type>, ctarray_fit_t<Pair<Type>, ctarray_norm_t<Pair<Type>, ctarray<Pair<Type>, Pairs...>, Size>, Size - sizeof...(Pairs)>>;

    static constexpr std::array<Pair<Type>, Size> arr = type::arr;
};
