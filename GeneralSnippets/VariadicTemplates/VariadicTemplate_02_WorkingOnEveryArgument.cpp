// =====================================================================================
// VariadicTemplate_02_WorkingOnEveryArgument.cpp // Working on every argument
// =====================================================================================

module modern_cpp:variadic_templates;

namespace VariadicTemplatesWorkingOnEveryArgument {

    // ====================================================================
    // 1. Beispiel:
    // Parameter Pack Expansion mit Hilfe von std::initializer_list
    // ====================================================================

    template <typename T>
    void doSomething(const T& value) {
        std::cout << "got value " << value << std::endl;
    }

    // doesn't compile
    //template <typename... TArgs>
    //void doSomethingForAll(const TArgs& ... args) {
    //    
    //    doSomething(args) ...;
    //    // doSomething(1), doSomething(2)
    //}

    template <typename... TArgs>
    void myDoSomethingForAll(const TArgs& ... args) {

        // doSomething(args) ...;

        std::initializer_list<int> list = {
            ( doSomething(args) , 0 ) ... 
        };

        // doSomething(1), doSomething(2)
    }

    void my_test()
    {
        int n = 1;
        int m = 2;
        int a = 0;

        a = (n = 3), (m = 4);   // Sequenz-Operator ,

        myDoSomethingForAll(1, 23.45, 'A');
    }



    template <typename... TArgs>
    void doSomethingForAll(const TArgs& ... args) {
        // std::initializer_list<int> list = { (doSomething(args), 0)... };
        std::initializer_list<int> { (doSomething(args), 0)... };
    }

    void test_01() {
        doSomethingForAll(1, '!', std::string("ABC"), 5.5);
        std::cout << std::endl;
    }

    // ====================================================================
    // 2. Beispiel:
    // C++ 17 constexpr if
    // Beispiel mit "Trailing Comma"
    // ====================================================================

    // ====================================================================
    // 1. Variante: 
    // print-Funktion mit 2 Überladungen
    // ====================================================================

    void print() {}

    template <typename Head, typename ... Tail>
    void print(Head head, Tail ... tail)
    {
        std::cout << head << ", ";
        print(tail ...);
    }

    void test_02_01() {
        print(1, '!', std::string("ABC"), 5.5);
        std::cout << std::endl;
    }

    // ====================================================================
    // 2. Variante: 
    // print-Funktion in einer Überladung und mit constexpr
    // ====================================================================

    template <typename Head, typename... Tail>
    void printEx(const Head& head, const Tail&... tail) {
        std::cout << head;
        if constexpr (sizeof...(tail) > 0) {
            std::cout << ", ";
            printEx(tail...);
        }
    }

    void test_02_02() {
        printEx(1, '!', std::string("ABC"), 5.5);
        std::cout << std::endl;
    }
}

void main_variadic_templates_working_on_every_argument()
{
    using namespace VariadicTemplatesWorkingOnEveryArgument;
    my_test();
    //test_01();
    //test_02_01();
    //test_02_02();
}

// =====================================================================================
// End-of-File
// =====================================================================================
