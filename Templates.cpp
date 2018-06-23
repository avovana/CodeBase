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
