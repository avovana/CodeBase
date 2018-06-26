// Array elements sum

#include <iostream>
#include <array>
#include <iterator>
#include <algorithm>

template<typename T, unsigned long N>
std::array<T, N>& operator+=(std::array<T, N>& lhs, const std::array<T, N>& rhs) {
    
    auto sum = [](auto l, auto r) { return l + r; };
    
    std::transform(std::begin(lhs), 
                   std::end(lhs), 
                   std::begin(rhs), 
                   std::begin(lhs), 
                   sum
                  );
    
    return lhs;
}

template<typename T, std::size_t N>
void print(const char * msg, std::array<T, N>& array)
{
    std::cout << msg;
    std::copy(std::begin(array), std::end(array), std::ostream_iterator<T>(std::cout, " "));
    std::cout << '\n';
}

int main()
{
    std::array<int, 4> array1{1,2,3,4};
    std::array<int, 4> array2{5,6,7,8};
    
    print("Original array: ", array1);
    
    array1 += array2;
    
    print("After addition array: ", array1);
    
    return 0;
}

// Array construction in compile time
/*
pack expansion
std::integer_sequence
*/

#include <utility>
#include <array>
#include <iostream>
//#include <boost/type_index.hpp> // better then <typeinfo>
#include <typeinfo>

template<typename T, T... Ns>
constexpr auto make_iota_array(T const offset, std::integer_sequence<T, Ns...>) -> std::array<T, sizeof...(Ns)> 
{
    return {{(Ns + offset)...}};
}

template<typename T, T N>
constexpr auto make_iota_array(T const offset = {})
{
    static_assert(N >= T{}, "no negative sizes");
    return make_iota_array<T>(offset, std::make_integer_sequence<T, N>{});
}

int main() 
{
    static constexpr auto ar = make_iota_array<int, 10>(99);

    //std::cout << boost::typeindex::type_id<decltype(ar)>().pretty_name() << '\n';
    std::cout << typeid(ar).name() << '\n';

    for (auto const n : ar)
        std::cout << n << '\n';
}
