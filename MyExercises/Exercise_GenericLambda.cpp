#include <iostream>
#include <vector>

class Person
{
public:
	Person(std::string name, size_t age) : m_name(name), m_age(age) {}

	
	auto operator > (int age) const { return m_age > age; } //compare -> compare Operator muss hier implementiert sein

	std::string getName() const { return m_name; }
	size_t getAge() const { return m_age; }
private:
	std::string m_name;
	size_t m_age;

	
};
std::ostream& operator << (std::ostream& os, const Person& p) { 
	std::cout << p.getName() << ": " << p.getAge();
	return os;
};


int main() {
	std::vector<Person> personValues{
		Person{ "Hans", 40 },
		Person{ "Sepp", 60 }
	};

	auto isGreaterThanFifty = [](const auto& n) { return n > 50; };

	auto pos = std::find_if(
		std::begin(personValues),
		std::end(personValues),
		isGreaterThanFifty
	);

	if (pos == std::end(personValues)) {
		std::cout << "keine Person gefunden " << std::endl;
	}
	else {
		std::cout << *pos << std::endl;
	}

	return 0;
}