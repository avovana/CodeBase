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
