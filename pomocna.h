//---------------------------------------------------------------------------

#ifndef pomocnaH
#define pomocnaH
#include <algorithm>
#include <utility>
#include <vector>
#include <list>
#include <queue>
#include <set>
using namespace std;
//---------------------------------------------------------------------------

struct Tacka {
	double x, y;
	Tacka(double x, double y): x(x), y(y) {}
	bool operator<(Tacka);
	bool operator==(Tacka);
	double operator-(Tacka);
	void Crtaj(TImage*, TColor = clBlack, int = 3);
};

int Orijentacija(Tacka, Tacka, Tacka);


struct Duz {
	Tacka A, B;
	Duz(Tacka a, Tacka b): A(a), B(b) {
		if (B < A) {
			swap(A, B);
		}
	}
	void Crtaj(TImage*, TColor = clBlack, int velicina = 3);
	bool horizontalna();
	bool vertikalna();
};

struct Trokut {
	Tacka A, B, C;
	Trokut(Tacka a, Tacka b, Tacka c): A(a), B(b), C(c) {
		if (Orijentacija(A, B, C) < 0) {
			swap(B, C);
		}
	}
};

void iscrtajPoligon(vector<Tacka>&, TImage*);
bool daLiSeDuziSijeku(Duz, Duz);
bool tackaUTrouglu(Tacka, Trokut);
bool tackaUnutarKonveksnog(Tacka, vector<Tacka>&);
pair<int, int> nadjiTangente(Tacka, vector<Tacka>&);
void pomjeriIteratorNaprijed(list<int>::iterator&, list<int>&);
void pomjeriIteratorNazad(list<int>::iterator&, list<int>&);

struct HorVerDuziPoX {
	bool operator()(pair<Duz*, Tacka> d1, pair<Duz*, Tacka> d2) const {
		return d2.second < d1.second;
	}
};

struct AktivneHorVerPoY {
	bool operator()(Duz* d1, Duz *d2) const {
		return d1->A.y < d2->A.y;
	}
};

#endif
