// Move semantic
// compiler flag to bypass RVO optimization: -fno-elide-constructors

#include <iostream>

class Holder
{
    public:

    Holder(int size)
    {
        std::cout << "Ctor. size = " << size << '\n';
        m_data = new int[size];
        m_size = size;
    }

    Holder(const Holder& other)
    {
        std::cout << "Copy Ctor" << '\n';
        m_data = new int[other.m_size];
        std::copy(other.m_data, other.m_data + other.m_size, m_data);
        m_size = other.m_size;
    }

    Holder& operator=(const Holder& other) 
    {
        std::cout << "Assignement operator" << '\n';

        if(this == &other) return *this;

        delete[] m_data;

        m_data = new int[other.m_size];
        std::copy(other.m_data, other.m_data + other.m_size, m_data);
        m_size = other.m_size;

        return *this;
    }

    Holder(Holder&& other)
    {
        std::cout << "Copy rvalue Ctor" << '\n';

        m_data = other.m_data;
        m_size = other.m_size;

        other.m_data = nullptr;
        other.m_size = 0;
    }

    Holder& operator=(Holder&& other)     // <-- rvalue reference in input  
    {  
        std::cout << "Assignement rvalue operator" << '\n';

        if (this == &other) return *this;

        delete[] m_data;

        m_data = other.m_data;
        m_size = other.m_size;

        other.m_data = nullptr;
        other.m_size = 0;

        return *this;
    }

    ~Holder()
    {
        std::cout << "Dtor" << '\n';
        delete[] m_data;
    }

    private:

    int*   m_data;
    size_t m_size;
};

Holder createHolder(int size)
{
    return Holder(size);
}

int main()
{
    Holder h1(1000);                // regular constructor
    Holder h2(h1);                  // copy constructor (lvalue in input)
    Holder h3 = createHolder(2000); // move constructor (rvalue in input) (1) 

    std::cout << "------" << '\n';

    h2 = h3;                        // assignment operator (lvalue in input)
    h2 = createHolder(500);         // move assignment operator (rvalue in input);
    return 0;
}
