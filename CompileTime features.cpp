// compile-time array creation (constexpr, array, index_sequence)

#include <utility>
#include <array>

template<typename T, std::size_t N, std::size_t... I>
constexpr auto create_array_impl(std::index_sequence<I...>) {
    return std::array<T, N>{ {I...} };
}

template<typename T, std::size_t N>
constexpr auto create_array() {
    return create_array_impl<T, N>(std::make_index_sequence<N>{});
}

int main() {
    constexpr auto array = create_array<std::size_t, 4>();

    static_assert(array[0] == 0, "Yay");
    static_assert(array[1] == 1, "Yay");
    static_assert(array[2] == 2, "Yay");
    static_assert(array[3] == 3, "Yay");
    return 0;
}

// compile-time array size computation
/*
godbolt output:
main:
  mov eax, 2
  ret
*/
template<typename T, unsigned N>
int len (T(&)[N]) 
{ 
  return N;
}

int main()
{
    int x[] = {6, 7};
    return len(x);
}
