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

// Array construction in compile time with generating custom sequence

#include <type_traits>
#include <utility>
#include <array>

template <typename T = std::size_t>
constexpr T generate_ith_number(const std::size_t index) {
  static_assert(std::is_integral<T>::value, "T must to be an integral type");

  if (index == 0) return 0;
  return (3 * index - 1) / 2;
}

template <std::size_t... Is> 
constexpr auto make_sequence_impl(std::index_sequence<Is...>)
{
    return std::index_sequence<generate_ith_number(Is)...>{};
}

template <std::size_t N> 
constexpr auto make_sequence()
{
    return make_sequence_impl(std::make_index_sequence<N>{});
}

template <std::size_t... Is>
constexpr auto make_array_from_sequence_impl(std::index_sequence<Is...>)
{
    return std::array<std::size_t, sizeof...(Is)>{Is...};
}

template <typename Seq>
constexpr auto make_array_from_sequence(Seq)
{
    return make_array_from_sequence_impl(Seq{});
}

int main()
{
    constexpr auto arr = make_array_from_sequence(make_sequence<6>());
    static_assert(arr[0] == 0);
    static_assert(arr[1] == 1);
    static_assert(arr[2] == 2);
    static_assert(arr[3] == 4);
    static_assert(arr[4] == 5);
    static_assert(arr[5] == 7);
}
