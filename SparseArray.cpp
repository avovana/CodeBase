// My SparseArray implementation
// "operator+()" is implemented but doesn't work

#include <cstdint>
#include <iostream>
#include <cassert>
#include <utility>
#include <bitset>

template<typename T, uint64_t Mask>
class SparseArray 
{
    template<typename T1, typename T2>
    using EnableIfConvertible = std::enable_if_t<std::is_convertible_v<T1, T2>>;

    public:
    using ElementType = T;

    constexpr SparseArray() 
        : values() { 
            std::cout << "Default ctor" << '\n';
        }

    template<typename... Args>
    constexpr SparseArray(Args&&... args) 
    : values{args...} {
        std::cout << "Args ctor. Mask = " << Mask << '\n';
    }

    template<uint8_t Index>
    constexpr ElementType get() const {
        if(isSet(Index))
            return values[countEntityNumber(Index)];
        else
            return T();
    }

    template<typename TOther, uint64_t MaskOther, typename = EnableIfConvertible<TOther, T>>
    constexpr auto operator +(const SparseArray<TOther, MaskOther>& other)  {
        constexpr auto max_index = maxIndex(MaskOther) > maxIndex(Mask) ? maxIndex(MaskOther) : maxIndex(Mask);

        auto indexes = std::make_index_sequence<max_index>{};
        auto sequence = make_sequence_impl(other, indexes);

        constexpr decltype(Mask) NewMask = MaskOther | Mask;
        using NewType = decltype(values[0] + other.values[0]);
        
        return createArray<NewType, NewMask>(sequence);
    }

    constexpr static std::size_t countEntityNumber (size_t index) {
        uint64_t subMask{0};

        for(uint64_t i = 0; i < index; ++i) {
            subMask = subMask | 0b1;

            if(i + 1 < index)
                subMask <<= 1;
        }
        return popcount(Mask & subMask);
    }

    private:
    template <std::size_t index, typename TOther, uint64_t MaskOther>
    constexpr std::size_t generate_ith_number(const SparseArray<TOther, MaskOther>& other) {
        return get<index>() + other.template get<index>();
    }

    template <typename TOther, uint64_t MaskOther, std::size_t... Is> 
    constexpr auto make_sequence_impl(const SparseArray<TOther, MaskOther>& other, std::index_sequence<Is...>) {
        return std::index_sequence<generate_ith_number<Is>(other)...>{};
    }

    template<typename NewElementType, uint64_t NewMask, std::size_t... I>
    constexpr auto createArray(std::index_sequence<I...>) {
        return SparseArray<NewElementType, NewMask>(I...);
    }

    constexpr static std::size_t popcount (size_t value) {
        return value != 0 ? (value & 0b1) + popcount(value >> 1) : 0;
    }

    constexpr static std::size_t isSet (size_t pos) {
        return (Mask >> pos) & 0b1;
    }

    constexpr static std::size_t maxIndex (size_t value) {
        return value != 0 ? 1 + maxIndex(value >> 1) : 0;
    }

    static const int mask = Mask;
    static const int size = popcount(Mask);

    ElementType values[size];  
};

int main ()
{
    SparseArray<float,  3> array0(1.0f, 2.0f);
    SparseArray<double, 10> array1(      4.0,    7.0);

    //auto sum = array0 + array1;
//
    //static_assert(sizeof(sum) == sizeof(double) * 3, "Invalid sum array size");
    static_assert(sizeof(array0) == sizeof(float) * 2, "Invalid array size");
    static_assert(sizeof(array1) == sizeof(double) * 2, "Invalid array size");
//
    //assert((std::is_same_v<typename decltype(sum)::ElementType, double> == true));
//
    //assert(sum.get<0>() == 1.0);
    //assert(sum.get<1>() == 6.0);
    //assert(sum.get<2>() == 0.0);
    //assert(sum.get<3>() == 7.0);
//
    SparseArray<float, 3> array2;
    assert(array2.get<0>() == 0.0f);
    assert(array2.get<1>() == 0.0f);

    return 0 ;
}
