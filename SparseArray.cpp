// My SparseArray implementation
// "operator+()" is not implemented yet

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
        constexpr decltype(Mask) NewMask = MaskOther | Mask;

        auto seq2 = std::make_index_sequence<popcount(NewMask)>{};
        auto seq = make_sequence_impl(other, seq2);
        
        //auto seq_other = std::make_index_sequence<popcount(MaskOther)>{};
        //std::cout << "popcount(MaskOther): " << popcount(MaskOther) << '\n';
        //std::cout << "maxIndex(MaskOther) " << maxIndex(MaskOther) << '\n';
        
        constexpr auto max_index = maxIndex(MaskOther) > maxIndex(Mask) ? maxIndex(MaskOther) : maxIndex(Mask);
        
        auto seq_other = std::make_index_sequence<max_index>{};
        
        make_sequence_impl2(other, seq_other);
        
        return createArray<NewMask>(seq);
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
    //-----------------------
    template <std::size_t idx, typename TOther, uint64_t MaskOther>
    constexpr std::size_t print2(const SparseArray<TOther, MaskOther>& other) 
    {
        
        //std::cout << "other.template get<idx>(): " << other.template get<idx>() << '\n' << '\n';
        //std::cout << "other get<idx>() + get<idx>(): " << other.template get<idx>() + get<idx>() << '\n';
        //return (other.template get<idx>()) + get<idx>();
        //this.template get<idx>();
        
        return other.template get<idx>() + get<idx>();
    }
    
    template <typename T2>
    constexpr static T2 print(T2 arg) 
    {
        std::cout << "arg: " << arg << '\n';
        return arg;
    }

    template <typename TOther, uint64_t MaskOther, std::size_t... Is> 
    constexpr void make_sequence_impl2(const SparseArray<TOther, MaskOther>& other, std::index_sequence<Is...>) {
        //auto x = {print(Is)...};
        //auto x = {print2<Is>()...};
        //auto x = {print2<Is>(SparseArray<float,  3>(1.0f, 2.0f))...};
        auto x = {print2<Is>(other)...};
    }
//-----------------------
    
    template <typename TOther, uint64_t MaskOther, typename Type>
    constexpr static Type generate_ith_number(const SparseArray<TOther, MaskOther>& other, const Type index) {
        //return get<index>() + other.template get<index>();
        return index * 3;
    }

    template <typename TOther, uint64_t MaskOther, std::size_t... Is> 
    constexpr auto make_sequence_impl(const SparseArray<TOther, MaskOther>& other, std::index_sequence<Is...>) {
        return std::index_sequence<generate_ith_number(other, Is)...>{};
    }

    template<uint64_t NewMask, std::size_t... I>
    constexpr auto createArray(std::index_sequence<I...>) {
        return SparseArray<T, NewMask>(I...);
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
    
    std::cout << "array0.get<0>: " << array0.get<0>() << '\n';
    std::cout << "array0.get<1>: " << array0.get<1>() << '\n';
    
    std::cout << "array1.get<0>: " << array1.get<0>() << '\n';
    std::cout << "array1.get<1>: " << array1.get<1>() << '\n';
    std::cout << "array1.get<2>: " << array1.get<2>() << '\n';
    std::cout << "array1.get<3>: " << array1.get<3>() << '\n';
    
    auto sum = array0 + array1;
    std::cout << "sum.get<0>: " << sum.get<0>() << '\n';
    std::cout << "sum.get<1>: " << sum.get<1>() << '\n';
    std::cout << "sum.get<2>: " << sum.get<2>() << '\n';
    std::cout << "sum.get<3>: " << sum.get<3>() << '\n';
    
    SparseArray<float, 3> array2;
    assert(array2.get<0>() == 0.0f);
    assert(array2.get<1>() == 0.0f);
    
  return 0 ;
}
