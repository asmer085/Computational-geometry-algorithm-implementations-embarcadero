//---------------------------------------------------------------------------

#pragma hdrstop

#include "pomocna.h"
//---------------------------------------------------------------------------

int Orijentacija(Tacka A, Tacka B, Tacka C) {
	double p = A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y);
	if (p > 0) {
		return -1;
	}
	if (p < 0) {
		return 1;
	}
	return 0;
}

bool Tacka::operator<(Tacka t2) {
	if (this->x == t2.x)
		return this->y < t2.y;
	return this->x < t2.x;
}

bool Tacka::operator==(Tacka t2) {
	return this->x == t2.x && this->y == t2.y;
}

double Tacka::operator-(Tacka t2) {
	return sqrt(pow((this->x - t2.x), 2) + pow((this->y - t2.y), 2));
}

void Tacka::Crtaj(TImage* Slika, TColor boja, int velicina) {
	Slika->Canvas->Brush->Color=boja;
	Slika->Canvas->Ellipse(x-velicina, y-velicina, x+velicina, y+velicina);
    Slika->Canvas->Brush->Color=clBlack;
}

void Duz::Crtaj(TImage* Slika, TColor boja, int velicina) {
	Slika->Canvas->Brush->Color=boja;
	Slika->Canvas->Pen->Color=boja;
	// A.Crtaj(Slika, boja, velicina);
	// B.Crtaj(Slika, boja, velicina);

	Slika->Canvas->MoveTo(A.x, A.y);
	Slika->Canvas->LineTo(B.x, B.y);

	Slika->Canvas->Brush->Color=clBlack;
    Slika->Canvas->Pen->Color=clBlack;
}

bool Duz::horizontalna() {
	return this->A.y == this->B.y;
}

bool Duz::vertikalna() {
	return this->A.x == this->B.x;
}

void iscrtajPoligon(vector<Tacka> &tacke, TImage *Slika) {

	for(int i=0; i < tacke.size(); i++) {
		tacke[i].Crtaj(Slika, clBlue);
		int naredna = (i + 1) % tacke.size();
		Duz(tacke[i], tacke[naredna]).Crtaj(Slika);
	}
}


bool daLiSeDuziSijeku(Duz d1, Duz d2) {
	bool uslov1 = (Orijentacija(d2.A, d2.B, d1.A) != Orijentacija(d2.A, d2.B, d1.B));
	bool uslov2 = (Orijentacija(d1.A, d1.B, d2.A) != Orijentacija(d1.A, d1.B, d2.B));
	return uslov1 && uslov2;
}

bool tackaUTrouglu(Tacka t, Trokut tr) {
	int o1 = Orijentacija(tr.A, tr.B, t);
	int o2 = Orijentacija(tr.B, tr.C, t);
	int o3 = Orijentacija(tr.C, tr.A, t);

	return o1 == o2 && o2 == o3;
}

bool tackaUnutarKonveksnog(Tacka t, vector<Tacka> &konveksni) {
	int p=1, k=konveksni.size()-1;

	while(k-p > 1) {
		int s = (p + k) / 2;
		if (Orijentacija(konveksni[0], konveksni[s], t) > 0) {
			k = s;
		}
		else {
			p = s;
		}
	}

	return tackaUTrouglu(t, Trokut(konveksni[0], konveksni[p], konveksni[k]));
}

bool daLiJeGornjaTangenta(Tacka t, Tacka trenutna, Tacka prije, Tacka poslije) {
    return Orijentacija(t, trenutna, prije) < 0 && Orijentacija(t, trenutna, poslije) < 0;
}

bool daLiJeDonjaTangenta(Tacka t, Tacka trenutna, Tacka prije, Tacka poslije) {
	return Orijentacija(t, trenutna, prije) > 0 && Orijentacija(t, trenutna, poslije) > 0;
}

bool daLiJeNaVecemLuku(Tacka t, Tacka trenutna, Tacka poslije) {
	if (Orijentacija(t, trenutna, poslije) > 0) {
		return false;
	}
	return true;
}

pair<int, int> nadjiTangente(Tacka t, vector<Tacka> &konveksni) {
	int n = konveksni.size();
	int p=0, k=n-1;

	int donja=-1, gornja=-1;

	while(k - p > 1) {
		int s=(p+k)/2;
		int poslije=(s+1)%n;
		int prije=(s+n-1)%n;

		if(daLiJeGornjaTangenta(t, konveksni[s], konveksni[prije], konveksni[poslije])) {
			gornja = s;
            break;
		}
		// sredina je na vecem luku
		if (daLiJeNaVecemLuku(t, konveksni[s], konveksni[poslije])) {
			// prvi slucaj (crveni)
			if (Orijentacija(t, konveksni[s], konveksni[p]) > 0 && Orijentacija(t, konveksni[p], konveksni[(p+n-1)%n]) > 0) {
				p = s;
			}
			// drugi slucaj (zeleni)
			else {
				k = s;
			}
		}
		// sredina je manjem luku
		else {
			// prvi slucaj (zeleni)
			if (Orijentacija(t, konveksni[s], konveksni[p]) > 0 && Orijentacija(t, konveksni[p], konveksni[(p+n-1)%n]) < 0) {
				k = s;
			}
			// drugi slucaj (crveni)
			else {
				p = s;
			}
		}
	}

	if (gornja == -1) {
		if (daLiJeGornjaTangenta(t, konveksni[p], konveksni[(p+n-1)%n], konveksni[(p+1)%n])) {
			gornja = p;
		}
		else {
			gornja = k;
		}
	}

	p = 0;
	k = n-1;

	while(k - p > 1) {
		int s=(p+k)/2;
		int poslije=(s+1)%n;
		int prije=(s+n-1)%n;

		if(daLiJeDonjaTangenta(t, konveksni[s], konveksni[prije], konveksni[poslije])) {
			donja = s;
            break;
		}
		// sredina je na vecem luku
		if (daLiJeNaVecemLuku(t, konveksni[s], konveksni[poslije])) {
			// prvi slucaj (crveni)
			if (Orijentacija(t, konveksni[s], konveksni[p]) < 0 && Orijentacija(t, konveksni[p], konveksni[(p+n-1)%n]) > 0) {
				k = s;
			}
			// drugi slucaj (zeleni)
			else {
				p = s;
			}
		}
		// sredina je manjem luku
		else {
			// prvi slucaj (zeleni)
			if (Orijentacija(t, konveksni[s], konveksni[p]) < 0 && Orijentacija(t, konveksni[p], konveksni[(p+n-1)%n]) < 0) {
				p = s;
			}
			// drugi slucaj (crveni)
			else {
				k = s;
			}
		}
	}

	if (donja == -1) {
		if (daLiJeDonjaTangenta(t, konveksni[p], konveksni[(p+n-1)%n], konveksni[(p+1)%n])) {
			donja = p;
		}
		else {
			donja = k;
		}
	}

	return {donja, gornja};
}

void pomjeriIteratorNaprijed(list<int>::iterator &it, list<int> &lista) {
	it++;
	if (it == lista.end()) {
		it = lista.begin();
	}
}

void pomjeriIteratorNazad(list<int>::iterator &it, list<int> &lista) {
	if (it == lista.begin()) {
		it = --lista.end();
	}
	else {
		it--;
    }
}

#pragma package(smart_init)
