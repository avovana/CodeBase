// Pack expansion in initializer_list context

/*
Description:
Transmit arguments with different types to function thanks to tuple.
Expand set of arguments in a initializer_list context.

Articles:
https://arne-mertz.de/2016/11/more-variadic-templates/
http://riptutorial.com/cplusplus/example/26687/turn-a-std--tuple-t-----into-function-parameters
*/

#include <array>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>

template <class Arg>
int doSomething(const Arg& arg)
{
    std::cout << "arg: " << arg << " ";
    return 1;
}

// ----------------------------------------------------------------------------
template <typename Tuple, std::size_t... I>
void process(Tuple const& tuple, std::index_sequence<I...>) 
{
    auto x = {(doSomething(std::get<I>(tuple)))...};
    std::cout << ". Size: " << x.size() << "\n" << "\n";
}

template <typename Tuple>
void process(Tuple const& tuple) 
{
    process(tuple, std::make_index_sequence<std::tuple_size<Tuple>::value>());
}

// ----------------------------------------------------------------------------
int main() 
{
    process(std::make_tuple(1, 3.14, std::string("foo")));
    process(std::make_tuple('a', 2, 2.71, std::string("bar")));
    process(std::make_pair(3, std::string("pair")));
    process(std::array<int, 3>{ 1, 2, 3 });
}
