// C++17 template argument deduction

#include <iostream>

template<typename T>
struct Thingy
{
    T t;

    template<typename Z>
    Thingy(Z z) : t(z)
    { 
        std::cout << "templated ctor. ";
    }

    Thingy(int z);

    template<typename... Args>
    Thingy(Args&&... args);
};

Thingy(const char *) -> Thingy<std::string>;

template<typename... Args>
Thingy(Args&&... args) -> Thingy<int>;

template<typename T>
Thingy<T>::Thingy(int z)
{
    std::cout << "sep: " << z;
}

template<typename T> template<typename... Args>
Thingy<T>::Thingy(Args&&... args)
{ 
    (std::cout << "args: " << ... << args);
}

int main() 
{
    Thingy<int> thing(1, 2);                // template argument set explicitly
    std::cout << '\n';
    
    Thingy thing2{"A String"};              // template argument deduction
    std::cout << ' ' << thing2.t << '\n';
    
    Thingy thing3(1, 2);
}

// Sfinae
/*
std::enable_if
*/
#include <iostream>
#include <type_traits>

template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
sum(T a, T b) {
  std::cout << "std::is_integral<T>\n";
  return a + b;
}

template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
sum(T a, T b) {
  std::cout << "std::is_floating_point<T>\n";
  return a + b;
}

int main() {
  std::cout << sum(4, 5) << '\n';
  std::cout << sum(5.3, 2.4) << '\n';
}

// Sfinae
// Choosing a proper constructor
/*
std::enable_if
std::initializer_list
*/
#include <iostream>
#include <initializer_list>

template<typename T>
struct A
{
    template<typename... Args>
    A(Args&&... /*args*/)
    { 
        std::cout << "Constructor ... Args" << '\n';
    }
    
    template<typename T2, typename = std::enable_if_t<std::is_same<T, T2>::value> >
    A(std::initializer_list<T2> /*init_list*/)
    { 
        std::cout << "Constructor init_list" << '\n';
    }
};

int main()
{
    A<int> example1({1,2});
    A<int> example2({1.0, 2.3});
    
    return 0;
}

// old-style metaprogramming
// generate new type thanks to recursion

#include <tuple>

template<typename CoordinateType, unsigned N, typename... REST>
struct generate_tuple_type
{
 typedef typename generate_tuple_type<CoordinateType, N-1, CoordinateType, REST...>::type type;
};

template<typename CoordinateType, typename... REST>
struct generate_tuple_type<CoordinateType, 0, REST...>
{
  typedef std::tuple<REST...> type;
};

int main()
{
  using gen_tuple_t = generate_tuple_type<int, 3, double>::type;
  using hand_tuple_t = std::tuple<int, int, int, double>;
  static_assert( std::is_same<gen_tuple_t, hand_tuple_t>::value, "different types" );
}
