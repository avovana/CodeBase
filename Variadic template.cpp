// Parametr pack output
/*
variadic template
operator overloading
default constructor
*/

#include <iostream>

template <typename T>
void print(T value) 
{
	std::cout << value << std::endl;
}

template <typename First, typename... Rest>
void print(First firstValue, Rest... rest) 
{
	print(firstValue);
	print(rest...);
}

struct SomeStruct
{
    SomeStruct(int x_) 
    : x{x_}
    { }
    
    SomeStruct() = default;
    
    friend std::ostream& operator << (std::ostream& out, const SomeStruct& obj);
    
    private:
    int x = 4;
};

std::ostream& operator << (std::ostream& out, const SomeStruct& obj)
{
    out << obj.x;
    return out;
}

int main()
{
    SomeStruct instance;

    print(1, 2, 3.0, instance, SomeStruct(5));
    
    return 0;
}

// Parametr pack output
/*
variadic template
operator overloading
default constructor
std::forward
std::initializer_list
*/
#include <iostream>

template <typename... Ts>
void print_all(Ts... args)
{
    for (const auto& p : { args... })
        std::cout << p << "\n";
}

template <typename... Ts>
void wrapper(Ts... args)
{
    print_all(std::forward<Ts>(args)...);
}

struct SomeStruct
{
    SomeStruct(int x_) 
    : x{x_}
    { }
    
    SomeStruct() = default;
    
    friend std::ostream& operator << (std::ostream& out, const SomeStruct& obj);
    
    private:
    int x = 4;
};

std::ostream& operator << (std::ostream& out, const SomeStruct& obj)
{
    out << obj.x;
    return out;
}

int main()
{
    SomeStruct instance;

    wrapper(instance, SomeStruct(3));
    
    return 0;
}

// Take type of parametr pack by index
/*
variadic template
*/
#include <iostream>
#include <typeinfo>

struct SomeStruct
{ };

template <std::size_t I, typename T, typename ...Ts>
struct nth_element_impl {
    using type = typename nth_element_impl<I-1, Ts...>::type;
};

template <typename T, typename ...Ts>
struct nth_element_impl<0, T, Ts...> {
    using type = T;
};

template <std::size_t I, typename ...Ts>
using nth_element = typename nth_element_impl<I, Ts...>::type;


int main()
{
    nth_element<3, double, int, float, SomeStruct> variable;
    std::cout << typeid(variable).name() << std::endl;
    
    return 0;
}

// Applaying one function to all members of parametr pack
/*
initializer_list
*/
#include <iostream>
#include <iterator>
#include <initializer_list>
#include <vector>

template<typename T>
struct A
{
    template<typename T2, typename = std::enable_if_t<std::is_same<T, T2>::value> >
    A(std::initializer_list<T2> init_list)
    { 
        std::cout << "Constructor init_list" << '\n';
        std::copy(init_list.begin(), init_list.end(), std::ostream_iterator<T>( std::cout, " "));
        std::copy(init_list.begin(), init_list.end(), std::back_inserter(data));
    }
    
    private:
    std::vector<T> data;
};

template<typename T>
T getHead(T arg)
{
    return arg;
}

template <class... Args>
void doSomethingForAll(Args const&... args) 
{
    auto x = {getHead(args)...};
    A<int> p(x);
}

int main()
{
    doSomethingForAll(1,2,3,4);
    return 0;
}

// Iterating over parametr pack with if constexpr C++17 and print
/*
if constexpr
*/

#include <iostream>

template <typename T>
void print(T value) 
{
	std::cout << value << std::endl;
}

template <int number, typename Head,  typename... Rest>
void get(Head firstValue, Rest... rest)
{
    if constexpr(number > 0) 
    {
        std::cout << "number > 0" << '\n';
        get<number - 1>(rest...);
    } 
    else 
    {
        std::cout << "number = 0" << '\n';
        print(firstValue);
    }
}

template <int number, typename... Rest>
void print(Rest... rest)
{
    std::cout << "Parametr pack size: " << sizeof...(rest) << std::endl;
    get<number>(rest...);
}

int main()
{
    print<2>(3, 4, 5.6);
    return 0;
}

// Iterating over parametr pack with if constexpr C++17 getting value
/*
if constexpr
*/

#include <iostream>
#include <typeinfo>

template <int number, typename Head,  typename... Rest>
auto get_impl(Head firstValue, Rest... rest)
{
    if constexpr(number > 0) 
    {
        std::cout << "number > 0" << '\n';
        return get_impl<number - 1>(rest...);
    } 
    else 
    {
        std::cout << "number = 0" << '\n';
        return firstValue;
    }
}

template <int number, typename... Rest>
auto get(Rest... rest)
{
    std::cout << "Parametr pack size: " << sizeof...(rest) << std::endl;
    return get_impl<number>(rest...);
}

int main()
{
    auto value = get<2>(3, 4, 5.6);
    std::cout << "value: " << value << '\n';
    std::cout << "value has type: " << typeid(value).name() << '\n';
        
    return 0;
}
