#include <pthread.h>
#include <utility>
#include <array>
#include "ctarr_utils.hpp"

struct Cstring
{
    static constexpr unsigned long SIZE = 64;

    bool is_empty = true;
    char c_str[SIZE] = {0};

    constexpr Cstring() : c_str{0}, is_empty{true} {};
    constexpr Cstring(const char *str)
    {
        unsigned long idx = 0;
        const char *p = str;

        while (*p != '\0' && idx < SIZE)
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

        for (unsigned long i = 0; i < SIZE; ++i)
        {
            crc ^= static_cast<unsigned char>(c_str[i]);
            for (int i = 0; i < 8; ++i)
                crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
        }

        return ~crc;
    }

    static constexpr unsigned int static_crc32(const char *str)
    {
        Cstring cstr(str);
        return cstr.get_crc32();
    }
};

template <typename Type>
struct Pair
{
    unsigned int crc{};
    Cstring first{};
    Type second{};

    constexpr Pair() : crc{}, first{}, second{} {}

    constexpr Pair(const Cstring& key, const Type& value, unsigned long crc)
    {
        this->first = key;
        this->second = value;
        this->crc = crc;
    }

    constexpr Pair(const Cstring& key, const Type& value)
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

    constexpr bool operator==(const Pair& pair) const
    {
        return (this->crc == pair.crc);
    }

    constexpr auto operator<(const Pair& pair) const
    {
        return (this->crc < pair.crc);
    }

    constexpr auto operator%(const unsigned long val) const
    {
        return Pair<Type>(first, second, crc % val);
    }
}; 

// Map to std::array ------------------------------------------------------------------------------
template <typename Type, typename Array>
struct map_to_runtime;

template <typename Type, Pair<Type>... Ps>
struct map_to_runtime<Type, ctarray<Pair<Type>, Ps...>> {
    static constexpr std::array<Type, sizeof...(Ps)> value = {
        { { Ps.second }... }
    };
};

// Pad --------------------------------------------------------------------------------------------
template <typename Type, typename Array, unsigned int Last = 0>
struct pad;

template <typename Type, Pair<Type> I0, Pair<Type>... Is, unsigned int Last>
struct pad<Type, ctarray<Pair<Type>, I0, Is...>, Last>
{
    using fit_item = ctarray_fit_t<Pair<Type>, ctarray<Pair<Type>, I0>, (Last == 0
                                                                         ? I0.crc - Last
                                                                         : I0.crc - Last - 1)>;
    using type = ctarray_concat_t<Pair<Type>,
                                  fit_item,
                                  typename pad<Type, ctarray<Pair<Type>, Is...>, I0.crc>::type
    >;
};

template <typename Type, Pair<Type> I0, unsigned int Last>
struct pad<Type, ctarray<Pair<Type>, I0>, Last>
{
    using type = ctarray_fit_t<Pair<Type>, ctarray<Pair<Type>, I0>, I0.crc - Last - 1>;
};

template <typename Type, unsigned long Size, Pair<Type>... Pairs>
struct map
{
    using norm_arr = ctarray_norm_t<Pair<Type>, ctarray_sort_t<Pair<Type>, ctarray<Pair<Type>, Pairs...>>, Size>;
    static_assert(ctarray_doubles_v<Pair<Type>, norm_arr> == false);
    using padded = typename pad<Type, norm_arr>::type;

    static constexpr std::array<Type, padded::arr.size()> arr = map_to_runtime<Type, padded>::value;
};

template <typename Type, unsigned long Size, Pair<Type>... Pairs>
class Cmap
{
public:
    constexpr Cmap() {}
    constexpr ~Cmap() {}

    void print(void)
    {
        for (unsigned long i = 0; i < cmap::arr.size(); ++i)
            std::cout << "[" << i << "]" << " " << cmap::arr[i] << "\n";
        std::cout << "-----------------\n";
    }

    Type get(const Cstring &str) const
    {
        unsigned int idx = str.get_crc32() % Size;

        if (idx >= cmap::arr.size())
            throw std::runtime_error("Key not found");

        return cmap::arr[idx];
    }

private:
    using cmap = map<Type, Size, Pairs...>;

};
