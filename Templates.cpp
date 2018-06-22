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

