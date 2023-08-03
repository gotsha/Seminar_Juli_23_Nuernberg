module exercises:move

#include <iostream>
#include <vector>

class Person {
private:
    std::string m_name;         // name of person
    std::vector<int> m_values;  // some arbitrary person values

public:
    Person() {}
    Person(const std::string& name) : m_name{ name } {}
    Person(const Person&) {}


    void addValue(int value) {
        m_values.push_back(value);
    }

    friend std::ostream& operator<< (std::ostream& os, const Person& cust) {
        os << "{ " << cust.m_name << ": ";
        for (auto val : cust.m_values) {
            os << val << ' ';
        }
        os << "}";
        return os;
    }
};

int move()
{
    // create a person with some initial values
    Person dagobert{ "Dagobert Duck" };
    for (int value : { 1, 2, 3, 4, 5, 6, 7, 8, 9 }) {
        dagobert.addValue(value);
    }

    // print person
    std::cout << "Person: " << dagobert << std::endl;

    // insert person into a collection
    std::vector<Person> persons;
    persons.push_back(std::move(dagobert));

    // print person again
    std::cout << "Person: " << dagobert << std::endl;
    std::cout << "persons: " << persons[0] << std::endl;

    return 0;
}