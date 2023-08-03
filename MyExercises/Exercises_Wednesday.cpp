#include <iostream>
#include <thread>
#include <vector>
#include <utility>
#include <variant>

namespace firstTry {
    class ExecutionTimer {
    public:
        static std::chrono::milliseconds duration(void(*fun)(int, double), int a, double b) {
            std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
            auto tpStart = std::chrono::time_point_cast<std::chrono::milliseconds>(start);
            std::invoke(fun, a, b);
            std::chrono::system_clock::time_point stop = std::chrono::system_clock::now();
            auto tpStop = std::chrono::time_point_cast<std::chrono::milliseconds>(stop);
            return tpStop - tpStart;
        }

    private:
        std::chrono::milliseconds m_duration;
    };

    void g(int a, double b)
    {
        // simulate some work (function with parameters)
        std::cout << "calling g with parameters " << a << " and " << b << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    void exercise01() {
        std::chrono::milliseconds time = ExecutionTimer::duration(g, 10, 20.0);
        std::cout << time.count() << " msecs." << std::endl;
    }
}

namespace secondTry {
    template<typename ... T>
    class ExecutionTimer {
    public:
        static std::chrono::milliseconds duration(void(*fun)(T ...), T ... args) {
            std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
            auto tpStart = std::chrono::time_point_cast<std::chrono::milliseconds>(start);
            std::invoke(fun, args ...);
            std::chrono::system_clock::time_point stop = std::chrono::system_clock::now();
            auto tpStop = std::chrono::time_point_cast<std::chrono::milliseconds>(stop);
            return tpStop - tpStart;
        }

    private:
        std::chrono::milliseconds m_duration;
    };

    template<typename T>
    void  g(T a) {
        std::cout << a << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    template<typename T, typename ... U>
    void g(T a, U ... b)
    {
        // simulate some work (function with parameters)
        std::cout << a << " and ";
        g(b ...);
    }

    void exercise02() {
        std::chrono::milliseconds time = ExecutionTimer<int, double, char, long>::duration(g, 10, 20.0, 'A', 234235325L);
        std::cout << time.count() << " msecs." << std::endl;
    }
}

namespace variadic1 {
    template<typename T>
    bool andAll(T val) {
        return val;
    }
    
    template<typename T, typename ... U>
    bool andAll(T val, U ... vals) {
        return val && andAll(vals ...);
    }

    template<typename T>
    bool orAll(T val) {
        return val;
    }

    template<typename T, typename ... U>
    bool orAll(T val, U ... vals) {
        return val || orAll(vals ...);
    }

    void exercise() {
        bool result = andAll(true, false, true);
        std::cout << std::boolalpha << result << std::endl;

        result = andAll(true, true, true, true, true, true, true, true, true, true);
        std::cout << std::boolalpha << result << std::endl;

        result = orAll(false, false, false);
        std::cout << std::boolalpha << result << std::endl;

        result = orAll(true, true, true, true, true, true, true, true, true, true);
        std::cout << std::boolalpha << result << std::endl;
    }
}

namespace variadic2 {
    template<typename T, typename U>
    bool sameType(T arg1, U arg2) {
        return std::is_same<decltype(arg1), decltype(arg2)>::value;
    }

    template<typename T, typename T2, typename ... U>
    bool sameType(T arg1, T2 arg2, U ... args) {
        return std::is_same<decltype(arg1), decltype(arg2)>::value && sameType(arg2, args ...);

    }
    
    void exercise() {
        bool result;
        result = sameType(1, 2, 3, 4, 5, 6, 7, 8, 9);
        std::cout << std::boolalpha << result << std::endl;

        result = sameType(1, 2, 3, 4, '?', 5, 6, 7, 8, 9);
        std::cout << std::boolalpha << result << std::endl;
    }
}

namespace utility1 {

    class Media {
    public: 
        friend class Book; 
        friend class Movie;

        Media(std::string title, std::string author, float price, int amount)
            : m_author(author), m_title(title), m_price(price), m_amount(amount) {}

        virtual const float balance() = 0;
        const float getPrice() { return m_price; };

        virtual void print() {
            std::cout << m_title << " by " << m_author << "; " << m_amount << "*" << m_price << "€" << std::endl;;
}
    private:
        std::string m_author;
        std::string m_title;
        float m_price;
        int m_amount;
    };

    class Book : Media {
    public:
        Book(std::string title, std::string author, float price, int amount)
            : Media(title, author, price, amount) {
                std::cout << "we have a new movie" << title << ", " << amount << "*" << price << "€" << std::endl; 
            }

          const float balance() override {
              return m_price * m_amount;
          }

          const float getPrice() { return Media::getPrice(); };

          void print() {
              Media::print();
          }
    };

    class Movie : public Media {
    public: Movie(std::string title, std::string author, float price, int amount) : 
        Media(title, author, price, amount) { 
            std::cout << "we have a new book" << title << ", " << amount << "*" << price << "€" << std::endl; 
        }

        const float balance() override {
            return m_price * m_amount;
        }

        const float getPrice() { return Media::getPrice(); };

        void print() {
            Media::print();
        }
    };

    template<typename T, typename U>
    class Bookstore {
    public: 
        Bookstore(std::vector<std::variant<T, U>> stock) : m_stock(stock) {}

        Bookstore(std::initializer_list<std::variant<T, U>> stock) : m_stock(stock) {}

        void addMedia(std::variant<T, U> item) {
            m_stock.emplace_back(item);
        }

        void listStock() {
            auto visitor = [](auto&& arg) {
                arg.print();
            };

            std::cout << std::endl << "Stock" << std::endl;
            for (auto item : m_stock) {
                std::cout << "- ";
                std::visit(visitor, item);
            }
            std::cout << std::endl;
        }

        double totalBalance() {
            auto visitor = [](auto&& arg) {
                //std::cout << arg.getPrice() << " ";
                return arg.balance();
            };

            float sum = 0;
            for (auto item : m_stock) {
                sum += std::visit(visitor, item);
                //sum += std::visit([](auto&& arg) { return arg.balance(); }, item);
            }
            return sum;
        }
    private: 
        std::vector<std::variant<Book, Movie>> m_stock;
    };

    void exercise() {
        Book cBook{ "C", "Dennis Ritchie", 11.99F, 12 };
        Book javaBook{ "Java", "James Gosling", 17.99F, 21 };
        Book cppBook{ "C++", "Bjarne Stroustrup", 16.99F, 4 };
        Book csharpBook{ "C#", "Anders Hejlsberg", 21.99F, 8 };

        Movie movieTarantino{ "Once upon a time in Hollywood", "Quentin Tarantino", 6.99F, 3 };
        Movie movieBond{ "Spectre", "Sam Mendes", 8.99F, 6 };

        using MyBookstore = Bookstore<Book, Movie>;

        MyBookstore bookstore = MyBookstore{
            cBook, movieBond, javaBook, cppBook, csharpBook, movieTarantino
        };

        std::cout << "Balance: " << bookstore.totalBalance() << std::endl;

        Book proCpp{ "Professional C++", "Marc Gregoire", 55.99F, 1 };
        Movie aMov{ "a movie", "whoever", 100.0F, 10 };
        bookstore.addMedia(proCpp);
        bookstore.addMedia(aMov);

        std::cout << "Balance: " << bookstore.totalBalance() << std::endl;

        bookstore.listStock();
    }
}


namespace utility1_1 {

    class Media {
    public:

          

          virtual const float balance() = 0;
          virtual const float getPrice() = 0;

          virtual void print() = 0; //{ std::cout << m_title << " by " << m_author << "; " << m_amount << "*" << m_price << "€" << std::endl; }
    
    };

    class Book : Media {
    public:
        Book(std::string title, std::string author, float price, int amount)
            : m_author(author), m_title(title), m_price(price), m_amount(amount) {
            std::cout << "we have a new movie" << title << ", " << amount << "*" << price << "€" << std::endl;
        }

        const float balance() override {
            return m_price * m_amount;
        }

        const float getPrice() override { return m_price; };

        void print() override {
            std::cout << m_title << " by " << m_author << "; " << m_amount << "*" << m_price << "€" << std::endl;
        }

    private:
        std::string m_author;
        std::string m_title;
        float m_price;
        int m_amount;
    };

    class Movie : public Media {
    public: Movie(std::string title, std::string director, float price, int amount)
        : m_director(director), m_title(title), m_price(price), m_amount(amount) {
            std::cout << "we have a new book" << title << ", " << amount << "*" << price << "€" << std::endl;
        }

          const float balance() override {
              return m_price * m_amount;
          }

          const float getPrice() override { return m_price; };

          void print() override  {
              std::cout << m_title << " by " << m_director << "; " << m_amount << "*" << m_price << "€" << std::endl;
          }

    private:
        std::string m_director;
        std::string m_title;
        float m_price;
        int m_amount;
    };

    class Bookstore {
    public:
        Bookstore() {
            std::cout << "new Bookstore" << std::endl;
        };

        Bookstore(std::vector<Media*> stock) : m_stock(stock) {}

        //Bookstore(std::initializer_list<std::variant<T, U>> stock) : m_stock(stock) {}

        void addMedia(Media* item) {
            m_stock.push_back(item);
        }

        /*void addMedia(Movie item) {
            m_stock.emplace_back(item);
        }
        */
        void listStock() {
            for (auto item : m_stock) {
                std::cout << "- ";
                item->print();
            }
            std::cout << std::endl;
        }

        double totalBalance() {
            float sum = 0;
            for (auto item : m_stock) {
                sum += item->balance();
                //sum += std::visit([](auto&& arg) { return arg.balance(); }, item);
            }
            return sum;
        }
    private:
        std::vector<Media*> m_stock;
    };

    void exercise() {
        Book cBook{ "C", "Dennis Ritchie", 11.99F, 12 };
        Book javaBook{ "Java", "James Gosling", 17.99F, 21 };
        Book cppBook{ "C++", "Bjarne Stroustrup", 16.99F, 4 };
        Book csharpBook{ "C#", "Anders Hejlsberg", 21.99F, 8 };

        Movie movieTarantino{ "Once upon a time in Hollywood", "Quentin Tarantino", 6.99F, 3 };
        Movie movieBond{ "Spectre", "Sam Mendes", 8.99F, 6 };

        /*std::vector<Media*> media;
        media.push_back(&cBook);
        media.push_back(movieBond);
        media.push_back(javaBook);
        media.push_back(cppBook);
        media.push_back(csharpBook);
        media.push_back(movieTarantino);*/

        //using MyBookstore = Bookstore<Book, Movie>;

        Bookstore bookstore = Bookstore();
        bookstore.addMedia((Media*)(& cBook));
        bookstore.addMedia((Media*)(&movieBond));
        bookstore.addMedia((Media*)(&javaBook));
        bookstore.addMedia((Media*)(&cppBook));
        bookstore.addMedia((Media*)(&csharpBook));
        bookstore.addMedia((Media*)(&movieTarantino));
        
        std::cout << "Balance: " << bookstore.totalBalance() << std::endl;

        Book proCpp{ "Professional C++", "Marc Gregoire", 55.99F, 1 };
        Movie aMov{ "a movie", "whoever", 100.0F, 10 };
        bookstore.addMedia((Media*)(&proCpp));
        bookstore.addMedia((Media*)(&aMov));

        std::cout << "Balance: " << bookstore.totalBalance() << std::endl;

        bookstore.listStock();
    }
}


int main()
{
    //using namespace firstTry;
    //exercise01();
    //using namespace secondTry;
    //exercise02();
    //using namespace variadic1;
    //exercise();
    //using namespace variadic2;
    //exercise();
    //using namespace utility1;
    //utility1::exercise();
    using namespace utility1_1;
    utility1_1::exercise();

    return 0;
}