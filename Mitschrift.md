# RValue - LValue
```cpp
int n;
int *ip;
ip = &n;

//Zuweisen des Wertes 123
// a)
n=123 

// b)
*ip=123; 
ip++; // Adresse wird abhängig vom Datentyp um x Byte erhöht // Zeigerarithmetik

// Referenz
int& ri = n;// ri ist ein Alias // Stellvertretername
// Zuordnung mittels
ri = 456; // -> ohne Dereferenzierung...
ri++; // Wert von n wird erhöht -> 457

// Wozu Referenzen wenn es auch Pointer gibt?
// Schreibweise einfacher (keine * und & bei Zuweisungen...)
// Adresse erhöhen (Zeigerarithmetik) gefährlich
```
sayHello(std::string&& message) - && zeigt an, dass Referenz auf Objekte ohne Name übergeben werden, z.B. sayHello(strA + strB)

LValue und RValue aus C. (C ist in C++ komplett (zu 99%) enthalten)
- LValue -> links von Zuweisungszeichen -> hat eine Adresse.
- RVAlue -> hat keine Adresse bzw. ich kenn diese nicht

`std::move` ist keine Fkt sondern ein cast

# Move Semantics
new Anweisung entspricht malloc in C
```BigData::BigData(const BigData& data) {} //das ist der copy Constructor. 
vec.push_back(BigData(10, 1)); //dadurch wird ein temporäres Objekt angelegt; dieses wird danach auch wieder freigegeben -> destructor
```

um diese unnütze Kopie zu vermeiden gibt es move anstatt copy semantics. Eigentlich klauen wir dadurch das Objekt
BigData::BigData(BigData&& data) noexcept;

wenn new... brauchen wir immer ein delete -> nicht den Standard-Destruktor nutzen

_Rule of Three_
Klassen benötigen standardmäßig diese Elemente wenn es bei dynamische Objekte davon gibt oder Rückgaben mit return.
(new, oder z.B. int* array)

_Rule of Five_
um unnütze Kopien zu vermeiden -> move constructor + assignment
bei dynamischen Zeigern (new, oder z.B. int* array)


_Rule of Zero_
auch hier dyn Speicherverwaltung möglich: reserve => initialer Speicher

in C++ liegt ein std::vector<int> vec; auf dem Stack, der räumt sich von allein auf. Zusätzlich gibt es den heap. In Java gibt es nur den heap.

# auto
Funktionen mit RÜckgabewert auto: Rückgabewerte in der Fkt müssen übereinstimmen
```cpp
auto tueWas(bool flag, float f1, double d1) {
	if (flag) {
		return f2;
	}
	else {
		return f1;
	}
}
```
ist nicht valide.  
Was geht
```cpp
template <typename T, typename U>
auto tueWas(bool flag, T f1, U f2) -> decltype (f1 + v2) {
	if (flag) {
		return f2;
	}
	else {
		return f1;
	}
}

void anwendung() 
{
	auto result = tueWas(false, 123, 456l);
	auto result2 = tueWas(false, 123.0f, 345.5(;
}
```
oder:
```cpp
template <typename T, typename U>
decltype ( std::declval<T>() + std::declval<U>() ) tueWas(bool flag, T f1, U f2) {
	if (flag) {
		return f2;
	}
	else {
		return f1;
	}
}

void anwendung() 
{
	auto result = tueWas(false, 123, 456l);
	auto result2 = tueWas(false, 123.0f, 345.5(;
}
```

declval gibt ohne declval keinen Sinn. 

auto wird häufig mit Referenz-Keyword & verwendet um unbeabsichtigte Kopien zu vermeiden
   
const Schlüsselwort wird verwendet wenn möglich da der Compiler solche Objekte effizienter übersetzt.

# Lambdas


```cpp
// STL Container
std::vector<int> vec(10);

// Was ist ein Iterator-Objekt? Eine Position.
// STL Iteratoren
auto start = numbers.begin();
// ist identisch zu
// std::vector<int>::iterator start = numbers.begin();
auto end = numbers.end()

// Vektor füllen
// STL Algorithmus
// ==> verwendet intern std::memset was hochperfomant ist
std::fill(start, end, 123);

//wenn man nicht alles gleich belegen möchte, z.B. wie bei vec[i] = 2*i+1;
int belegeVor() {
	static int i = 0; //unschön weil hierdurch quasi globale Variable angelegt wird
	return 2*i+1;
}
// Historisch: die erste Variante -> Übergabe von Funktionsadresse
std::generate(start, end, belegeVor);



// Historisch: Die bessere Variante
operator () () {}
//erstes (): Aufrufoperator
//zweites (): Parameter

//Funktor = aufrufbares Objekt; Objekt, das den Operator () bereit stellt
class Vorbeleger
{
private:
	int m_factor;
	int m_i;

public:
	Vorbeleger (int factor) : m_factor(factor), m_i(0) {}

	operator () () {
		return m_factor * m_i + 1;
	}
};

/*void wasIstEinFunktor() {
	Vorbeleger belegeVor(2); // Instanz einer Klasse
	int result = belegeVor(); //Anwendung des Aufrufoperators auf das Objekt belegeVor
}*/

Vorbeleger beleger(3);
std::generate(
	start,
	end,
	beleger
);
```
static int i => globale Variable die aber nur in der definierenden Funktion angesprochen werden kann.

Ein Lambda ist ein Funktor.
std::begin(vec) ist die 'modernere' Schreibweise von vec.begin()

lokale Funktionen sind in C++ nicht erlaubt, lokale Klassen schon
Der Aufruf `std::sort(std::begin(vec), std::end(vec), LocalComparer{});` aus Lambda01.cpp entspricht schon einem Lambda. Nur noch anders geschrieben.
Lambda wäre dann
`std::sort(std::begin(vec), std::end(vec), [] (int n1, int n2) { return n1 < n2; });`
Ein Lambda/Eine Lambda-Funktion (eigentlich ein schlechter Name) ist ein Ojekt (Instanz einer Klasse)!!!!! keine Funktion

Funktionsaufrufe bei elementaren Datentypen, z.B. compare(int n1, int n2) by value machen (da ist es aufwändiger für den Prozessor mit Referenzen zu arbeiten). Andere Typen by reference um Kopien zu vermeiden

Wenn man jetzt wiedre das Flag mit reinbringen möchte:
`std::sort(std::begin(vec), std::end(vec), [flag = true] (int n1, int n2) { return (flag) ? n1 < n2 : n1 > n2; });`
flag wird hier ohne Datentyp angegeben da durch die Vorbelegung automatisch definiert.

um eine Instanzvariable einer Lambda-Funktion intern verändern zu können muss die Fkt als mutable definiert werden
der () operator ist const deklariert, deswegen muss die Lambda-Funktion als mutable definiert werden
```cpp
auto lambda = [variable = 123] () mutable {
	++variable;
	return variable;
}

std::cout << lambda() << std::endl;
std::cout << lambda() << std::endl;
std::cout << lambda() << std::endl;
```
theoretisch könnte ich auch
```cpp
auto lambda = [/*variable = 123*/] () mutable {
	static int variable = 123;
	++variable;
	return variable;
}

std::cout << lambda() << std::endl;
std::cout << lambda() << std::endl;
std::cout << lambda() << std::endl;
```
schreiben. Wenn man aber 
```
auto lambda1 = lambda;

std::cout << lambda() << std::endl;
std::cout << lambda() << std::endl;
std::cout << lambda() << std::endl;

std::cout << lambda1() << std::endl;
std::cout << lambda1() << std::endl;
std::cout << lambda1() << std::endl;
```
macht zählt im Falle der Verwendung von `static` die Variable von 124 bis 129.
Ansonsten gibt es eine Kopie des Objekts und somit ist die Ausgabe zweimal 124..126.

Vorteil von lambdas:
- mit implace-Funktionen muss man nicht seitenweise scrollen.
- kürzere Schreibweise als Funktoren
