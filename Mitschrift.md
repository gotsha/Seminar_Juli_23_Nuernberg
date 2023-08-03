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
sayHello(std\:\:string&& message) - && zeigt an, dass Referenz auf Objekte ohne Name �bergeben werden, z.B. sayHello(strA + strB)

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

in C++ liegt ein `std::vector<int> vec;` auf dem Stack, der r�umt sich von allein auf. Zus�tzlich gibt es den heap. In Java gibt es nur den heap.

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
_std::begin(vec)_ ist die 'modernere' Schreibweise von vec.begin()

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
        

# Range-based for Loop
Mglk `for (size_t i = 0; i != vec.size(); ++i)` ist die einzige, die einem die v�llige
Kontrolle gibt. Hier kann man alles machen: break, continue, adjust increment...

**Index bei for_each**
mit C++ 20 `for (int index{}; int n : vec) {}`

Wdh: Aufpassen, dass man bei for loops nicht unbeabsichtigte Kopien macht: Referenzen bei 
nicht elementaren Datentypen verwenden


# Structured Binding
**std::pair mit sinnvollen Namen**
Die Fkt `std::pair<int, int> divide_remainder(int dividend, int divisor) {}` liefert ein std::pair<int, int>.
Mit `auto result = divide_remainder(16, 3);` erh�lt man result vom Typ std::pair<int, int>. Zugriff auf
Quotient und Rest erh�lt man �ber _result.first_ bzw. _result.second_.
Mit 'auto [quotient, remainder] = divide_remainder(20, 3);` kann man die Ergebnisse �ber deren
Namen _quotient_ und _remainder_ ansprechen.

**Array mi Namen f�r die einzelnen Elemente**
```cpp
int arr[] = { 10, 11, 12 };
auto& [ a, b, c ] = arr;
```

**Tuple**
```cpp
std::tuple <char, int, double> values{ 'Z', 987, 987.654 };
std::get<0>(values);
int n = 1;
std::get<n>(values); //NEVER!!!
std::get<2>(values);
```


# Variant
bei std::variant muss vorher festgelegt werden welche Datentype das sein k�nnen, z.B.
`std::variant<int, float, std::string> var{ 10.456f };`.
Wenn das noch nicht feststeht muss man ein _std\:\:any_ verwenden.

**generischer Besucher**
wir m�ssen den Datentyp herausfinden -> decltype
```cpp
auto visitor = [](auto elem) {
    using ElemType = decltype(elem);
    
    if (std::is_same<ElemType,int>::value == true) {
        std::cout << "bin ein int " << elem << std::endl;
    }
    ...
}
```

`using ElemType = decltype (elem)` entspricht `typedef decltype (elem) ElemType;`

Will man das ganze universell auch f�r `auto& elem` machen : <a href=https://en.cppreference.com/w/cpp/header/type_traits>_Type traits_</a>
```cpp
using ElemTypeWithoutRef = std::remove_reference<ElemType>::type;

if (std::is_same<ElemTypeWithoutRef>, int>::value == true) {
    ...
}
```

**Template-Spezialisierung**
```cpp
//prim�res Template
template <class T>
struct my_remove_reference {
    using type = T;
};

//Template Spezialisierung
template <>
struct my_remove_reference<int> {
    using type = short; //Umbiegen von int in short
};

//Template Spezialisierung
template <class T>
struct my_remove_reference<T&> {
    using type = T;
    };
```

`std::visit` kann auch einen R�ckgabewert liefern; dazu einfach im Lambda einen Return machen

# [constexpr](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/ConstExpr/Constexpr.md)
wenn eine Klassen-Fkt als friend deklariert ist, dann geh�rt die Fkt eigentlich nicht mehr
zur definierenden Klasse.

Operatoren k�nnen in und au�erhalb der Klasse implementiert werden.
```cpp
friend constexpr Complex operator+(const Complex& x, const Complex& y)
{
    float real = x.real() + y.real();
    float imag = x.imag() + y.imag();
    return Complex{ real, imag };
}
```  
W�rde man hier friend weglassen, k�nnte man nicht _x_ und _y_ �bergeben sondern nur einen der
beiden. F�r den zweiten w�rde man dann die Elemente von _this_ nutzen.  
```cpp
constexpr Complex operator+(const Complex& x)
{
    
}
```


# Self-Invoking Lambda
<a href=https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/Lambda/Lambda01.cpp#L379>test_09</a>
ist die moderne Version des Makros. Durch das _()_ am Ende wird das constexpr Lambda direkt aufgerufen.

Das gleiche ist auch der Fall bei [CRC8-Bsp](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/Lambda/Lambda01.cpp#L379)

Mehr oder weniger alles was in C verf�gbar ist kann als constexpr verwendet werden.


# [UDL - User Defined Literal](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/Literals/Literals.md)
durch �berladung des Operators \"\" UND Syntax-Erg�nzung f�r das Suffix (z.B. _kg, _km, _rgb).
UDLs sollten einen \_ vorangestellt haben um diese von Literalen wie long, float,... unterscheiden
zu k�nnen (z.B. 123467L, 12.3F)

Bsp: `Color operator"" _rgb (unsigned long long int value) {...}`  
der Suffix, den wir haben wollen kommt nach dem \"\". Dann k�nnen wir z.B. Variablen wie
`Color red = 0xFF0000_rgb;` oder `Color abc = 123456_rgb` anlegen.

UDLs erwarten den gr��tm�glichen Datentyp, hier z.B. long long. Ansonsten auch long double bzw. char*


# [Variadische Templates](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/VariadicTemplates/VariadicTemplates_01_Introduction.md)
einpacken: vorangestelltes ...  
auspacken: nachgestelltes ...

Beispiel
```cpp
template<typename T>
void printer(T n) {
    std::cout << n << ' ';
}

// mehrer => Pack
template<typename T, typename ... U>    // m einpacken
void printer(T n, U ... m) {            // m einpacken : 2, 3, 4, 5
    std::cout << n << ' ';
    printer(m ...);                     // m auspacken
}

void test_seminar() {
    printer(1, 2, 3, 4, 5);             // 5 Args
}
```
Normalerweise verwendet man bei den variadischen Templates immer zwei Funktionen
(f�r den Fall, dass es nur noch einen Parameter gibt und den mit mehreren Par.).


Wozu? viele Hilfsfunktionen nehmen eine Variable Anzahl an Parametern entgegen. Diese
nutzen variadische Templates. Als Beispiel hier eine m�gliche Implementierung von
_std::make_unique()_:

```cpp
template <typename T, typename ... TArgs >
std::unique_ptr <T> my_make_unique(TArgs ... args) {
    std::unique_ptr <T> ptr {new T{args ...}};
    return ptr;
}
```
Wenn wir stattdessen folgendes nutzen:
```cpp
template <typename T, typename ... TArgs >
std::unique_ptr <T> my_make_unique_02(TArgs&& ... args) {
    std::unique_ptr <T> ptr {new T{ std::forward<TArgs>(args) ...}};
    return ptr;
}
```
dann wird _my_make_unique()_ hier mit einer sogenannten Universal Reference aufgerufen.  
Doppeltes && ist bei Templates eine universal reference, keine rvalue refernce wie sonst.  
Mehr dazu in [PerfectForwarding](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/PerfectForwarding/PerfectForwarding.md)

Nicht ganz so sch�n wie mit einer universal reference ist auch die einfache M�glichkeit
```cpp
std::unique_ptr <T> my_make_unique_01(const TArgs& ... args) {
    std::unique_ptr <T> ptr {new T{ args ... }};
    return ptr;
}
```
`std::forward` ist quasi der Bruder von `std::move`. Pr�ft ob lvalue; wenn, dann Referenz, sonst std::move


## Einschub: Kommaoperator
anderer Name: Sequenzoperator
```cpp
int n=1;
int m=2;
int a=0;
a = n = 3, m = 4;

//oder
for (m=0, n=1 ; m<100 ; m++) { ... }
```

## Ellipsis
```cpp
template <typename T>
void doSomething(T param) {
    std::cout << param << " ";
}

template <typename ... TArgs>
void myDoSomethingForAll(const TArgs& ... args) {
    std::initializer_list<int> list = {
        (doSomething(args) , 0) ...
    };
}

void my_test() {
    myDoSomethingForAll(1, 23.45, 'A');
}

my_test();
```
die Initializer-List dient nur dazu, den Pack auszupacken. Durch den Kommaoperator mit
anschlie�ender `0` wird hier erreicht, dass die `doSomething()` Fkt aufgerufen werden kann
obwohl sie vom type `void` ist.


# vector - emplace_back
`std::vector<MyClass> vec;`
Vermeiden einer nutzlosen Kopie wie bei 
`vec.push_back(MyClass {1, 2, 3});` durch
`vec.emplace_back(1, 2, 3);`. Dadurch wird das Objekt direkt in place angelegt.


# STL
## [std::transform](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/Transform/Transform.md)
Umsetzen eines Containers _A_ auf einen Containern _B_
v.a. hilfreich beim Arbeiten mit STL-Containern. Zum Kopieren des einen in den anderen
Containers wird dies intern standard �ber den Index gemacht -> Gr��e muss vorher feststehen.

Es ist auch m�glich, das �ber `push_back()` zu machen: `std::back_inserter()`.

ohne `back_inserter`:
```cpp
std::vector<std::string, size_t> phonebook {
    {   },
    {   },
    {   },
};
std::vector<std::string> names(5);
std::transform(
    std::begin(phonebook),
    std::end(phonebook),
    std::begin(names),
    [](const std::pair<std::string, size_t>& entry) {
        const auto& [name, phone_number] = entry;
        return name;
    }
);
```

## std::back_inserter

mit `back_inserter`:

```cpp
std::unordered_map<std::string, size_t> phonebook
{
    { "Hans Meier" , 12345678 },
    { "Franz Schneider", 81726354 },
    { "Hubert Mueller", 87654321 }
};
std::vector<std::string> names{};
std::transform(
    std::begin(phonebook),
    std::end(phonebook),
    std::back_inserter(names),
    [](const std::pair<std::string, size_t>& entry) {
        const auto& [name, phone_number] = entry;
        return name;
    }
);
```
`std::back_inserter` verwendet intern einen `push_back` => die Daten werden hinten angeh�ngt


## [std::any](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/Any/Any.md)
Der Partner von `std::variant`.
A `variant` can be _visited_, an `any` not.
A `variant` does not use additional heap memory.


## [std::optional<int>](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/Optional/Optional.md)
enth�lt ein flag ob die Variable wirklich einen Wert hat:
```cpp
std::optional<int> someValue;
if (someValue.has_value()) {
    ...
}
//andere Mglk:
if (someValue) {
    ...
}
someValue = std::nullopt; //reset
```

Wertzugriff erfolgt �ber `someValue.value()` oder `*someValue`.


## [std::array](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/Array/Array.md)
Vorteile von `std::array<int, 100> zahlen` vs. `int numbers[100]` ?  
Funktion die ein Array �bergeben bekommt
```cpp
print(ip* anfang, int len) {} // z.B. print(numbers, 100) => Stichwort array decay
// vs.
print(zahlen) {}

template<typename T, int Length>
void print(const std::array<T, Length>& array) {
    std::cout << "Length: " << array.size() << std::endl;
}
```
Das Problem ist hierbei aber, dass bei arrays verschiedener L�ngen, f�r jede Version
die Funktion neu kompiliert wird => [std::span](#std_span)

CTAD: Class Template Argument Deduction  
`std::array array1{ 9.7, 7.31 };` => `std::array<double, 2>`  
Problem:  
`std::array array3{ "Hello C++ 20" };` => `std::array<const char*, 1>`  
f�r string:
`std::array array3{ "Hello C++ 20"s };`

`.at()` Operator => enth�lt exception handling (auf Kosten von Laufzeit)

Vergleich von zwei Arrays  
Bei C-Arrays funktioniert `bool isEqual = (cArray == cArray2);` nicht, da Adressen
verglichen werden. Bei `std::array` hingegen wird Inhalt verglichen.

R�ckgabewert vom Typ Array  
Bei C-Arrays wird die Adresse einer (u.U. lokalen) Variablen zur�ckgegeben und es gibt eine Warnung.  
`std::array`s hingegen geben wirklich das Array zur�ck


## <a name="std_span">[std::span](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/Array/Array.md)</a>
Mit `std::span` lassen sich Adresse und L�nge von C-Arrays und auch Vektoren an eine andere Fkt �bergeben.  
Bps:
```cpp
void printSpan(std::span<int> values) {
    ...
}
int cArray[]{ 1, 2, 3, 4, 5 };
printSpan(cArray);
```


# Callable - [std::invoke](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/Invoke/Invoke.md)

# zur Bookstore-�bung
Keyword: Ducktyping

```cpp
template <typename ... Media>
class Bookstore
{
private:
    usint SimpleStock = std::vector<std::variant<Book, Movie>>;
    using Stock = std::vector<std::variant<TMedia ...>>;
    using StockList = std::initializer_list<std::variant<TMedia ...>>;

public:
    explicit Bookstore(StockList stock) : m_stock{ stock } {}
    //template member methond // T == Einen der Typen von TMedia
    template <typename T>
    void addMedia(const T& media) {
        std::variant<TMedia ...> tmp {media};
        m_stock.push_back(tmp);
        
        m_stock.push_back(media);
    }

}

    
using MyBookstore<Book, Movie>
using MyBookstore2<Book, Movie, CD>
```


# [CRTP - Curiously Recurring Template Pattern](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/CRTP/CRTP.md)


# [C++20 - Module](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/Modules_Import/Modules_Import.md)
neue Endung _.ixx_ f�r Module.
Diese Datei wird als _Definition_ des Moduls aufgefasst; sie muss nicht deren Impleentierung enthalten.

Bsp: in _.ixx_ sind nur die exports erw�hnt: _Module_Varian.ixx_
```cpp
export module modern_cpp:variant;

import std;

export void main_variant();
```

In der zugeh�rigen _.cpp_ gibt es dann u.a.
```cpp
module modern_cpp:variant;

namespace VariantDemo {

    void test_01() { ... }
}

void main_variant()
{
    using namespace VariantDemo;
    test_01();
}
```


# [Small String Optimization (SSO)](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/SSO/SSO.md)
automatische Optimierung kurzer `std::string`s.


# [String View](https://github.com/gotsha/Seminar_Juli_23_Nuernberg/blob/master/GeneralSnippets/StringView/StringView.md)
Vorteil gegen�ber `std::string`: Liegt im Stack, nicht im heap.

Diese Hilfsklasse kapselt einen `const char*`.  
Enh�lt als Infos den Zeiger auf den Anfang und die L�nge.

Bei _test_02_ aus _StringView.cpp_ wird aufgrund der L�nge beim konkatinieren die stringview umkopiert


# Sonstiges

## einzelne Compiler-Warnung disablen
Hier am Bsp von zwei C-Arrays die verglichen werden sollen. Aber da dies absichtlich in den
Beispielen enthalten ist soll die Warnung deaktiviert werden
```cpp
#pragma warning(push) //immer push und am Ende pop n�tig zum disablen der Warnung
#pragma warning(disable : 5056)

    // Worse: operator== compiles, but does the "wrong" thing: address comparison!
    bool isEqual = (cArray == other);

#pragma warning(pop)
```


# Follow-Up
Code-Segment vs. Data-Segment???
