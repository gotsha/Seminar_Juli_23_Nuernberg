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
ip++; // Adresse wird abh�ngig vom Datentyp um x Byte erh�ht // Zeigerarithmetik

// Referenz
int& ri = n;// ri ist ein Alias // Stellvertretername
// Zuordnung mittels
ri = 456; // -> ohne Dereferenzierung...
ri++; // Wert von n wird erh�ht -> 457

// Wozu Referenzen wenn es auch Pointer gibt?
// Schreibweise einfacher (keine * und & bei Zuweisungen...)
// Adresse erh�hen (Zeigerarithmetik) gef�hrlich
```
sayHello(std::string&& message) - && zeigt an, dass Referenz auf Objekte ohne Name �bergeben werden, z.B. sayHello(strA + strB)

LValue und RValue aus C. (C ist in C++ komplett (zu 99%) enthalten)
- LValue -> links von Zuweisungszeichen -> hat eine Adresse.
- RVAlue -> hat keine Adresse bzw. ich kenn diese nicht

`std::move` ist keine Fkt sondern ein cast

# Move Semantics
new Anweisung entspricht malloc in C
```BigData::BigData(const BigData& data) {} //das ist der copy Constructor. 
vec.push_back(BigData(10, 1)); //dadurch wird ein tempor�res Objekt angelegt; dieses wird danach auch wieder freigegeben -> destructor
```

um diese unn�tze Kopie zu vermeiden gibt es move anstatt copy semantics. Eigentlich klauen wir dadurch das Objekt
BigData::BigData(BigData&& data) noexcept;

wenn new... brauchen wir immer ein delete -> nicht den Standard-Destruktor nutzen

_Rule of Three_
Klassen ben�tigen standardm��ig diese Elemente wenn es bei dynamische Objekte davon gibt oder R�ckgaben mit return.
(new, oder z.B. int* array)

_Rule of Five_
um unn�tze Kopien zu vermeiden -> move constructor + assignment
bei dynamischen Zeigern (new, oder z.B. int* array)


_Rule of Zero_
auch hier dyn Speicherverwaltung m�glich: reserve => initialer Speicher

in C++ liegt ein std::vector<int> vec; auf dem Stack, der r�umt sich von allein auf. Zus�tzlich gibt es den heap. In Java gibt es nur den heap.

# auto
Funktionen mit R�ckgabewert auto: R�ckgabewerte in der Fkt m�ssen �bereinstimmen
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

auto wird h�ufig mit Referenz-Keyword & verwendet um unbeabsichtigte Kopien zu vermeiden
   
const Schl�sselwort wird verwendet wenn m�glich da der Compiler solche Objekte effizienter �bersetzt.

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

// Vektor f�llen
// STL Algorithmus
// ==> verwendet intern std::memset was hochperfomant ist
std::fill(start, end, 123);

//wenn man nicht alles gleich belegen m�chte, z.B. wie bei vec[i] = 2*i+1;
int belegeVor() {
	static int i = 0; //unsch�n weil hierdurch quasi globale Variable angelegt wird
	return 2*i+1;
}
// Historisch: die erste Variante -> �bergabe von Funktionsadresse
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
Lambda w�re dann
`std::sort(std::begin(vec), std::end(vec), [] (int n1, int n2) { return n1 < n2; });`
Ein Lambda/Eine Lambda-Funktion (eigentlich ein schlechter Name) ist ein Ojekt (Instanz einer Klasse)!!!!! keine Funktion

Funktionsaufrufe bei elementaren Datentypen, z.B. compare(int n1, int n2) by value machen (da ist es aufw�ndiger f�r den Prozessor mit Referenzen zu arbeiten). Andere Typen by reference um Kopien zu vermeiden

Wenn man jetzt wiedre das Flag mit reinbringen m�chte:
`std::sort(std::begin(vec), std::end(vec), [flag = true] (int n1, int n2) { return (flag) ? n1 < n2 : n1 > n2; });`
flag wird hier ohne Datentyp angegeben da durch die Vorbelegung automatisch definiert.

um eine Instanzvariable einer Lambda-Funktion intern ver�ndern zu k�nnen muss die Fkt als mutable definiert werden
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
theoretisch k�nnte ich auch
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
macht z�hlt im Falle der Verwendung von `static` die Variable von 124 bis 129.
Ansonsten gibt es eine Kopie des Objekts und somit ist die Ausgabe zweimal 124..126.

Vorteil von lambdas:
- mit implace-Funktionen muss man nicht seitenweise scrollen.
- k�rzere Schreibweise als Funktoren

Verwenden lokaler Variablen in Lambda
```cpp
auto l1 = [=] { //alle als Kopie
    std::cout << "Copy:      " << n << " " << m << std::endl;
};

auto l2 = [&] { //alle al Referenz
    std::cout << "Reference: " << n << " " << m << std::endl;
};

auto l3 = [&, m] { // alle als Referenz au�er die genannten
    std::cout << "Both:      " << n << " " << m << std::endl;
};

auto l4 = [=, &m] { // alle als Kopie au�er die genannten
    std::cout << "More both: " << n << " " << m << std::endl;
};
```
Der Datentyp von lambda ist ein std::function


# Generische Lambda Ausdr�cke


# Smart Pointer
```cpp
new Point [n];
delete[] ip;
delete ip; // was passiert dann? UB - undefined behaviour
```
f�r jedes _new_ muss auch ein _delete_ aufgerufen werden.


## std::unique_ptr - Zu einem Zeitpunkt nur EIN BESITZER
es existiert ein .get() um den hinter dem unique_ptr liegenden Zeiger zu gelangen.

Mit _std::move_ kann der unique_ptr weitergereicht werden. 

### Programm _test_01()_
am Ende des Scopes werden die Destruktoren aufgerufen
und in diesem Bsp werden die Pointer ptr1..3 gel�scht.

### Programm _test_02()_
beim Debuggen hat _ptr_ noch in der Zeile 
```cpp
std::unique_ptr<int> ptr{ loadUniquePointer() };
```
eine Adresse und den Wert 100. Noch bevor _loadUniquePointer()_ den ptr zur�ckgibt.
=> copy - Elision
Der Compiler erkennt, dass _loadUniquePointer()_ den passenden Typ zur�ckgibt und optimiert
das. Ergebnis wird zur�ck in R�ckgabeslot der aufrufenden Funktion kopiert.
(s. CopyMoveElision bei den cpp_modern_examples)

_storeUniquePointer(std::unique_ptr<int>& ptr)_ muss Referenz bekommen; andernfalls w�re
das Programm nicht �bersetzungsf�hig.

```cpp
void storeUniquePointer(std::unique_ptr<int>& ptr)
{
    // take ownership right now:
    std::unique_ptr<int> ptr2{ std::move(ptr) };
}

void test_02()
{
    // provide a function with a unique pointer: who owns the pointer now?
    storeUniquePointer(ptr);

    // does this work?
    std::cout << "*ptr:   " << *ptr << std::endl;
}
```
Das funktioniert nicht, da am Ende von storeUniquePointer() ptr2 vom Destructor gel�scht wird.


## std::shared_ptr - geteilter Besitz
enth�lt Z�hler. Zweiter Speicher f�r Pointer, CB _control block_ der die Adresse und den
Z�hler enth�lt. Der Zeiger auf dem Stack zeigt auf diesen Speicher am heap.
Wenn der Z�hler von shared-Pointern durch die Destruktoren beim L�schen der Pointer
vom Stack durch Verlassen des Scopes wieder auf 0 ist, wird auch der Speicher
am Heap freigegeben.

Wenn _storeSharedPointer()_ mit einer Referenz anstatt value definiert wird, wird auch kein
zus�tzlicher shared_ptr angelegt und somit der Zeiger auch nicht erh�ht.

`std::shared_ptr<int> ptr1{ new int{123} }`
vs.
`std::shared_ptr<int> ptr1{ std::make_shared<int>(123) }`
. Die zweite Variange mit _make_shared_ ist effizienter da nur eine Speicherplatzanforderung
am Heap ausgef�hrt wird.

Man arbeitet normalerweise nicht alleine mit dem std::shared_ptr sondern zusammen mit dem
std::weak_ptr


## std::weak_ptr
### test_01
```cpp
std::shared_ptr<int> ptr2{ weakPtr.lock() };
if (ptr2 != nullptr) {...}
```
durch .lock() kann aus einem weak_ptr, einem schwachen Zeiger, ein starker Zeiger gemacht werden.
Das funktioniert jedoch nicht immer -> nullptr check.
Wenn wir den nullptr zur�ckbekommen wissen wir, dass der Datensatz im Speicher nicht mehr
vorhanden ist.

Im Code arbeiten wir ausschlie�lich mit starken Zeigern -> wir machen nicht *weakPtr um den Wert
bekommen sondern *ptr2

### test_02
Forward declaration => riecht immer nach m�glichen Problemen
Problem bei test_0 ist, dass die Destruktoren nicht aufgerufen werden. Warum? �bungsaufgabe > SmartPointer >
<a href="https://github.com/gotsha/cpp_modern_examples/blob/master/GeneralSnippets/Exercises/Exercises_15_SmartPointers.md#aufgabe-3-beobachtungen-eines-zyklus-von-smart-pointer-objekten">Aufgabe 15-3</a>
Verweisen auf sich selbst mit std::shared_ptr -> selbst bei Verlassen des Scopes bleiben diese erhalten,
sie sind jedoch nicht mehr erreichbar.
Wenn m_rightNode und m_leftNode jedoch als std::weak_ptr definiert sind
(s. WeakPtr.cpp, class ParentNode Zeile 83, 84), dann werden auch die Destruktoren sauber aufgerufen.

std::shared_ptr ist _teuer_. unterst�tzt mutex, thread-safe...

**Bsp. Observer Pattern**  
Clients werden der Source mittels weak_ptr bekannt gemacht. Dadurch k�nnen die Clients auch 
gel�scht werden. Die Source l�uft einfach die Liste der angemeldeten Clients durch, macht einen
lock und wenn diese noch da sind werden sie informiert.

Frage ist immer: welches Objekt muss im Speicher sein? Wenn andere nicht darauf bestehen, dass
das Objekt noch im Speicher ist, dann werden diese einen weak_ptr haben. Ansosnten einen shared_ptr.


# Initialisierung

Uniform Initialization = Brace Initialization

Wann immer {} in Erscheinung treten haben wir es mit einer Initialisierung zu tun. Das gilt auch
f�r Konstruktor-Aufruf.
\{} : Initialisierung / gilt auch f�r Konstruktor-Aufruf
() : Methodenaufruf

**Erg�nzungen**  
`int n{};` ist das gleiche wie `int n = 0;`  
`int n{1};`ist das gleiche wie `int n = 1;`  
\{} ist die Vorbelegung mit dem Datentyp-spezifischen Nullwert.


# std::initializer_list
|Vergleich|
|---|
| std::vector | Daten liegen auf dem Heap (new) |
| std::initializer_list | Daten liegen auf dem Stack (NO new)|
|
| std::string | Daten liegen auf dem Heap (new)
| std:string_view | Daten liegen nicht auf dem Heap (NO new)
|
| std::array | Daten liegen auf dem Heap oder Stack
| std::span | Daten sind bereits vorhanden

## test_04
```cpp
TinyContainer(int value) {}
TinyContainer(std::initializer_list<int>) {};

TinyContainer tc2{ 1};
```
Hier wird per Festlegung von c++ die initializer_list
Implementierung aufgerufen.
M�chte man die int-Implementierung aufrufen geht dies mit
`TinyContainer tc2(1)` mit runden Klammern.
        
