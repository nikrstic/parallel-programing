#include<vector>
#include<iostream>
#include<omp.h>
using namespace std;

/*
Cilj je implementirati odloženo izračunavanje i to tako da se
izračunavanje paralelizuje.

Kod odloženog izračunavanja, u liniji poput
a + b - f(b*c) + g(a),
rezultat se NE izračunava odmah po pozivu odgovarajućeg operatora
ili funkcije, već je rezultat izraza stablo izračunavanja koje
pamti koje operacije treba primeniti na koje operande.
Rezultat dobijemo tek naknadno kada eksplicitno ili implicitno
zatražimo te rezultate.
Prednost odloženog izračunavanja je što konačna računica može biti
efikasnije implementirana i u pogledu brzine i u pogledu memorije.
Naime, ako su a i b vektori dužine n, za izračunavanje prethodne
linije neće biti potrebno kreirati nekoliko međurezultata od kojih
svaki zahteva po O(n) memorije, a rezultat će biti dobijen jednom
for petljom oblika
for(int i=0; i < n; i++)
	rezultat[i] = a[i] + b[i] - f(b[i]*c[i]) + g([a]);
Prednost u odnosu na rešenje u kome računamo vektore međurezultate
je što jednu petlju možemo više ubrzati korišćenjem više niti, nego
u slučaju da imamo više petlji (setite se da kreiranje niti zahteva
vreme).

Dodatna prednost odloženog izračunavanja, koju NEĆEMO implementirati
ovde je potencijalno uprošćavanje grafa izračunavanja. Na primer,
ako rezultat treba dobiti kao a + a + a + b - b, u pametnijoj implementaciji
bilo bi dovoljno izračunati rezultat kao 3*a.
*/

template<typename T>
class Vektor {
	vector<T> vec;  // wrapper za std::vector<T>
public:
	using value_type = T;
	Vektor() = default;
	Vektor(int n) : vec(vector<T>(n)) {	}
	Vektor(const Vektor&) = default;

	const T operator[] (int i) const { return vec[i]; }
	T& operator[] (int i) { return vec[i]; }
	int size() const { return vec.size(); }

	// Savet: vratiti se na ostatak klase Vektor tek nakon
	// što proučite ostatak fajla. 
	template<typename U>
	Vektor(const U& drugi) {
		// Konstruktor kopije omogući će nam da u liniji poput
		// Vektor<T> r = a + b - a*f(b) od proxy objekta koij
		// predstavlja stablo izračunavanja dobijemo rezultujući 
		// vektor. 
		int n = drugi.size();
		vec.resize(n);
		// NAPOMENA: ova paralelna implementacija nije u potpunosti bezbedna.
		// Naime, dok jedna nit čita vektore i izračunava rezultat, druga nit
		// može izmeniti vektore. 
		// Rešenje može da bude (ne zahteva se za ispit!) da svaki vektor
		// ima pridodat mutex i da pre evaluacije stabla izračunavanja "pokupimo"
		// sve lock objekte krećući se kroz celo stablo i zatim ih zaključamo
		// odjednom.
		#pragma omp parallel for num_threads(4)
		for (int i = 0; i < n; i++)
			vec[i] = drugi[i]; // u preklopljenom operatoru [] se dešava izračunavanje!
	}

	template<typename U>
	Vektor& operator=(const U& drugi) {
		vec.clear();
		int n = drugi.size();
		vec.resize(n);

		#pragma omp parallel for num_threads(4)
		for (int i = 0; i < n; i++)
			vec[i] = drugi[i];
	}

};

// Koristićemo funktore da parametrizujemo šablone koji će predstavljati
// binarne i unarne izraze. 
struct Plus {
	template<typename T>
	T operator() (const T& a, const T& b) const {
		return a + b;
	}
};


struct Minus {
	template<typename T>
	T operator() (const T& a, const T& b) const {
		return a - b;
	}
};

struct Puta {
	template<typename T>
	T operator() (const T& a, const T& b) const {
		return a * b;
	}
};

/*
Sledi jedna od najbitnijih klasa, tj. jedan od najbitnijih šablona klase.
BinarnaOperacija predstavljaće čvor u stablu izračunavanja koji odgovara
binarnoj operaciji. Zadatak klase je da definiše na koji način treba izračunati
neki čvor. Izračunavanje same vrednosti čvora ne dešava se u kontruktoru, već
će se vrednost rezultata na određenom indeksu i (setite se da ćemo raditi sa vektorima)
izračunati tek onda kada bude zahtevana operatorom [].
Unutar operatora [] obići ćemo rekurzivno celo stablo izračunavanja i izračunati potrebnu
vrednost.
Da rešenje bude što univerzalnije, a ujedno što brže, koristićemo šablone.
Kod odloženog izračunavanja korišćenje šablona nije neophodno - moguće je postići
isti efekat nasleđivanjem i dinamičkim polimorfizmom, ali statički polimorfizam (šabloni u C++)
je znatno brži.
*/
template<typename BinOp, typename L, typename R>
class BinarnaOperacija {
	// BinOp je tip binarne operacije. U praksi, funktor ili lambda izraz
	// koji uzima dva argumenta i računa neku vrednost.
	// L i R su tipovi levog i desnog operanda. I levo i desno može se osim 
	// vektora naći druga binarna operacija ili unarna operacija. 
public:
	using value_type = typename L::value_type; // podrazumevamo da i L i R imaju value_type (isti za L i R)
private:
	BinOp binOp; // pamtimo koju operaciju treba primeniti da se 
	// dobiju željeni rezultati i pamtimo na koje podatke treba
	// primeniti datu operaciju.
	const L& leviOperand; // referanca na konstantu tipa L
	const R& desniOperand; // referenca na konstantu tipa R
	// Poenta sa referencama je da izbegnemo kopiranje
public:
	BinarnaOperacija(BinOp _binOp,
		const L& _leviOperand,
		const R& _desniOperand) :
		binOp(_binOp), leviOperand(_leviOperand),
		desniOperand(_desniOperand) {
		// prazan konstruktor, nema nikakave računice ovde.
	}

	/*
	Jedan od najbitnijih delova klase je preklopljen operator
	pristupa elementima, operator[].
	Kada zatražimo element na nekom indeksu i, tek tada će se izračunati
	vrednost datog elementa. Izračunaćemo vrednost tako što ćemo primeniti
	ranije upamćenu binarnu operaciju na i-ti element levog i i-ti element
	desnog operanda. Međutim, setite se da levi i desni operand mogu biti
	binarni, unarni izrazi ili vektori. Kada pristupimo i-tom elementu
	levog ili desnog operanda, označimo ga sa x, sa x[i], u slučaju da je
	x binarni izraz rekurzivno će se i za taj izraz izvršiti logika kojom će se
	operator pridružen x-u primeniti na njegove operande i tako sve dok ne
	stignemo do operanada koji su vektori kod kojih se rekurzija završava
	(ako grafički predstavimo stablo izračunavanja, vektori su listovi stabla).
	*/
	value_type operator[] (int i) const {
		return binOp(leviOperand[i], desniOperand[i]);
	}
	int size() const { return leviOperand.size(); } // leviOperand.size() == desniOperand.size()
};

// NAPOMENA: u slučaju malog broja binarnih operacija, 
// može se ista logika implementirati pisanjem više
// klasa koje odgovaraju binarnim čvorovima kojima je unapred
// definisan operator. Na primer: 
//template<typename L, typename R>
//class PlusCvor {
//	...
//	value_type operator[] (int i) {
//		return levi_operand[i] + desni_operand[i];
//	}
//};


/*
Objekte klase BinarnaOperacija nećemo kao korisnici kôda
eksplicitno kreirati. U pitanju su instance šablona koje veoma
lako postaju veoma teške za snalaženje ako bismo ručno pisali
kod.
Sa druge strane, cilj nam je da imamo jednostavan i čitljiv kod.
Na primer, ako treba kreirati čvor koji predstavlja sabiranje
dva vektora a i b, cilj nam je da taj čvor kreiramo sa a + b,
a ne da pišemo eksplicitno
BinarnaOperacija<Plus, Vektor<T>, Vektor<T>> bop = BinarnaOperacija<Plus, Vektor<T>, Vektor<T>>(Plus(), a, b);
Zato ćemo preklopiti operator + tako da kao rezultat daje željenu
binarnu operaciju, tj. proxy objekat (videti projektni uzorak zastupnik).

Iako će preklapanjem operatora + korisnički kod biti jednostavniji,
samo preklapanje operatora takođe se može zapisati na više načina.
Slede 3 implementacije. Od najkompleksnije, do najjednostavnije
(sve 3 rade apsolutno istu stvar!)
*/

/*
// verzija 1
template<typename L, typename R>
BinarnaOperacija<Plus, L, R> operator+ (
	const L& l, const R& r) {
	return BinarnaOperacija<Plus, L, R>(Plus(), l, r);
}
*/

/*
// verzija 2
// Ako se u vreme prevođenja kôda na osnovu return izraza zna kog
// tipa će biti rezultat poziva funkcije, umesto navođenja tačnog tipa
// možemo iskoristiti ključnu reč auto.
template<typename L, typename R>
auto operator+ (
	const L& l, const R& r) {
	// očigledno kog tipa je rezultat:
	return BinarnaOperacija<Plus, L, R>(Plus(), l, r);
}*/

// verzija 3: 
// Može još kompaktnije. Od C++ verzije 17 moguća je dedukcija
// tipa šablona na osnovu tipa vrednosti koje se prosleđuju konstruktoru.
template<typename L, typename R>
auto operator+ (const L& l, const R& r) {
	// Ako je l tipa L, a r tipa R,
	// nema potrebe za eksplicitnim navođenjem
	// <Plus, L, R> nakon BinarnaOperacija
	return BinarnaOperacija(Plus(), l, r);
}

// Isti pristup za operatore - i *...
template<typename L, typename R>
auto operator- (const L& l, const R& r) {
	return BinarnaOperacija(Minus(), l, r);
}

template<typename L, typename R>
auto operator * (const L& l, const R& r) {
	return BinarnaOperacija(Puta(), l, r);
}


// Slično klasi BinarnaOperacija, implementiraćemo i unarnu
// operaciju. Logika je za nijansu jednostavnija nego kod binarne
// operacije... 
template<typename F, typename Arg>
class UnarnaOperacija {
public:
	using value_type = typename Arg::value_type;
private:
	const Arg& arg; // arg je argument na koji treba primeniti prosleđenu funkciju.
	F f; // f je unarna funkcija
public:
	UnarnaOperacija(F _f, const Arg& _arg) :
		f(_f), arg(_arg) {}
	int size() const { return arg.size(); }
	value_type operator[] (int i) const {
		return f(arg[i]);
	}
};

struct SQRT_Funktor {
	template<typename T>
	T operator() (const T& arg) const { return sqrt(arg); }
};

// Ponovo, zarad kompaktnijeg i upotrebljivijeg koda nećemo
// kasnije nigde eksplicitno kreirati objekte tipa UnarnaFunkcija,
// već ćemo (slično preklapanju operatora kod binarnih) napisati
// funkcije koje kao rezultat daju odgovarajuće unarne operacije.
template<typename Arg>
auto sqrt(const Arg& arg) {
	return UnarnaOperacija(SQRT_Funktor(), arg);
}

int main() {
	int n = 10;
	Vektor<float> a(n), b(n);
	for (int i = 0; i < n; i++) {
		a[i] = i;
		b[i] = i * i;
	}
	auto c = sqrt(a+b) - a * b;
	// Preći mišem preko ključne reči auto da vam prikaže tačan
	// tip koji se krije iza auto. 
	Vektor<float> rezultat = c;
	for (int i = 0; i < n; i++)
		cout << rezultat[i] << " " << endl;
	return 0;
}