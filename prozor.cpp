//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "prozor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	zavrsen_poligon = false;
	Slika->Canvas->Brush->Color=clWhite;
	Slika->Canvas->FillRect(TRect(0,0,Slika->Width,Slika->Height));
	Slika->Canvas->Brush->Color=clBlack;
	Slika->Canvas->FrameRect(TRect(0,0,Slika->Width,Slika->Height));
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SlikaMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	Tacka nova = Tacka(X, Y);

	if (RadioDodajDuz->Checked) {
		if (drugi_klik) {
			Duz nova_duz(tacke[tacke.size()-1], nova);
			nova_duz.Crtaj(Slika);
            duzi.push_back(nova_duz);
		}
		drugi_klik = !drugi_klik;
	}
	else if (RadioTackaUnutarKonveksnog->Checked) {
		if(konveksni_omotac.size() != 0) {
			if (tackaUnutarKonveksnog(nova, konveksni_omotac)) {
				ShowMessage("Tacka je unutra!");
			}
			else {
				ShowMessage("Tacka je van!");
			}
		}
	}
	else if (RadioTangente->Checked) {
		if(konveksni_omotac.size() != 0) {
			if (!tackaUnutarKonveksnog(nova, konveksni_omotac)) {
				pair<int, int> tangente = nadjiTangente(nova, konveksni_omotac);
				int donja = tangente.first;
				int gornja = tangente.second;

				Duz(nova, konveksni_omotac[gornja]).Crtaj(Slika);
				Duz(nova, konveksni_omotac[donja]).Crtaj(Slika);
			}
		}
	}
	else if (RadioCrtajPoligon->Checked) {
		if(poligon.size() > 2 && (nova - poligon[0]) < 10) {
			Duz d = Duz(poligon[poligon.size() - 1], poligon[0]);
			d.Crtaj(Slika);
			zavrsen_poligon = true;
		}
		else {
			if (zavrsen_poligon) {
				poligon.clear();
                dijagonale.clear();
                zavrsen_poligon = false;
			}
			poligon.push_back(nova);
			int n = poligon.size();
			if(n > 1) {
				Duz d = Duz(poligon[n-1], poligon[n-2]);
				d.Crtaj(Slika);
			}
			nova.Crtaj(Slika, clBlue);
		}
		return;
	}

	tacke.push_back(nova);
	nova.Crtaj(Slika, clBlue);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonProstiMnogougaoClick(TObject *Sender)
{
	for (int i = 1; i < tacke.size(); i++) {
		if (tacke[i] < tacke[0]) {
			swap(tacke[0], tacke[i]);
		}
	}
	Tacka lijeva(tacke[0]);
	sort(tacke.begin() + 1, tacke.end(), [lijeva](Tacka A, Tacka B) { return Orijentacija(lijeva, A, B) < 0; });
	iscrtajPoligon(tacke, Slika);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonOcistiClick(TObject *Sender)
{
	Slika->Canvas->Brush->Color=clWhite;
	Slika->Canvas->FillRect(TRect(0,0,Slika->Width,Slika->Height));
	Slika->Canvas->Brush->Color=clBlack;
	Slika->Canvas->FrameRect(TRect(0,0,Slika->Width,Slika->Height));
	tacke.clear();
	konveksni_omotac.clear();
	poligon.clear();
	zavrsen_poligon = false;
	dijagonale.clear();
	duzi.clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonGenerisiTackeClick(TObject *Sender)
{
	int broj_tacaka = EditBrojTacaka->Text.ToInt();
	for (int i = 0; i < broj_tacaka; i++) {
		int x = rand() % Slika->Width;
		int y = rand() % Slika->Height;
		Tacka nova(x, y);
		nova.Crtaj(Slika, clBlue);
		tacke.push_back(nova);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonPresjekDuziClick(TObject *Sender)
{
	int n(duzi.size());

	if (n < 2) {
		return;
	}

	Duz duz1(duzi[n-1]), duz2(duzi[n-2]);
	if (daLiSeDuziSijeku(duz1, duz2)) {
		ShowMessage("Duzi se sijeku!");
	}
	else {
        ShowMessage("Duzi se ne sijeku!");
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonUvijanjePoklonaClick(TObject *Sender)
{
	if (tacke.size() < 3) {
		return;
	}
	for (int i = 1; i < tacke.size(); i++) {
		if (tacke[i] < tacke[0]) {
			swap(tacke[0], tacke[i]);
		}
	}
	Tacka lijeva(tacke[0]);
	konveksni_omotac.push_back(lijeva);

	while(true) {
		int iduca(0);
        Tacka zadnja_dodana(konveksni_omotac[konveksni_omotac.size()-1]);

		if (tacke[iduca] == zadnja_dodana) {
			iduca = 1;
		}
		for (int i = 0; i < tacke.size(); i++) {
			if (zadnja_dodana == tacke[i]) {
				continue;
			}
			if(Orijentacija(zadnja_dodana, tacke[iduca], tacke[i]) > 0) {
				iduca = i;
			}
		}
		if (iduca == 0) {
			break;
		}
		konveksni_omotac.push_back(tacke[iduca]);
	}
	string poruka = "Broj tacaka na konveksnom: " + to_string(konveksni_omotac.size());
	ShowMessage(poruka.c_str());
	iscrtajPoligon(konveksni_omotac, Slika);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ButtomGrahamScanClick(TObject *Sender)
{
	if (tacke.size() < 3) {
		return;
	}

	for (int i = 1; i < tacke.size(); i++) {
		if (tacke[i] < tacke[0]) {
			swap(tacke[0], tacke[i]);
		}
	}

	Tacka lijeva = tacke[0];
	konveksni_omotac.push_back(lijeva);

    sort(tacke.begin() + 1, tacke.end(), [lijeva](Tacka A, Tacka B) { return Orijentacija(lijeva, A, B) < 0; });
	konveksni_omotac.push_back(tacke[1]);

	for(int i = 2; i < tacke.size(); i++) {
		int k = konveksni_omotac.size();
		Tacka prethodna(konveksni_omotac[k-2]), trenutna(konveksni_omotac[k-1]);
		Tacka sljedeca(tacke[i]);

		while(Orijentacija(prethodna, trenutna, sljedeca) > 0) {
			konveksni_omotac.pop_back();
			int k = konveksni_omotac.size();
			prethodna = konveksni_omotac[k-2];
			trenutna = konveksni_omotac[k-1];
		}

		konveksni_omotac.push_back(sljedeca);
	}

	string poruka = "Broj tacaka na konveksnom: " + to_string(konveksni_omotac.size());
	ShowMessage(poruka.c_str());
	iscrtajPoligon(konveksni_omotac, Slika);
}


//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonKonveksniInduktivnoClick(TObject *Sender)
{
	if (tacke.size() < 3) {
		return;
	}
	konveksni_omotac = {tacke[0], tacke[1], tacke[2]};
	if (Orijentacija(tacke[0], tacke[1], tacke[2]) > 0) {
		swap(konveksni_omotac[1], konveksni_omotac[2]);
	}

	for(int i=3; i<tacke.size(); i++) {
		Tacka trenutna(tacke[i]);

		if (tackaUnutarKonveksnog(trenutna, konveksni_omotac)) {
			continue;
		}

		pair<int, int> tangente = nadjiTangente(trenutna, konveksni_omotac);
		int donja = tangente.first;
		int gornja = tangente.second;

		if (donja > gornja) {
			konveksni_omotac.erase(konveksni_omotac.begin() + donja + 1, konveksni_omotac.end());
			if (gornja > 0) {
				konveksni_omotac.erase(konveksni_omotac.begin(), konveksni_omotac.begin() + gornja);
			}
			konveksni_omotac.insert(konveksni_omotac.begin(), trenutna);
		}
		else {
			konveksni_omotac.erase(konveksni_omotac.begin() + donja + 1, konveksni_omotac.begin() + gornja);
			konveksni_omotac.insert(konveksni_omotac.begin() + donja + 1, trenutna);
		}
	}
	string poruka = "Broj tacaka na konveksnom: " + to_string(konveksni_omotac.size());
	ShowMessage(poruka.c_str());
	iscrtajPoligon(konveksni_omotac, Slika);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonTriangulacijaClick(TObject *Sender)
{
	if (poligon.size() < 3) {
		return;
	}
	int velicina = poligon.size();
	list<int> lista;

	for(int i=0; i<velicina; i++)
		lista.push_back(i);

	int broj_trouglova = 0;

	auto p = lista.begin();
	auto t = ++lista.begin();
	auto n = ++(++lista.begin());

	while(lista.size() > 3) {
		if (Orijentacija(poligon[*p], poligon[*t], poligon[*n]) < 0) {
			// dobra orijentacija, provjeri da li je ijedna tacka u trouglu
			bool uho = true;
			auto i = n;
			pomjeriIteratorNaprijed(i, lista);
			while(i != p) {
				if(tackaUTrouglu(poligon[*i], Trokut(poligon[*p], poligon[*t], poligon[*n]))) {
					uho = false;
					break;
				}
                pomjeriIteratorNaprijed(i, lista);
			}

			if(uho) {
				dijagonale.push_back({*p, *n});
				lista.erase(t);
				t = p;
				pomjeriIteratorNazad(p, lista);
				continue;
			}
		}
		pomjeriIteratorNaprijed(p, lista);
		pomjeriIteratorNaprijed(t, lista);
		pomjeriIteratorNaprijed(n, lista);
	}
	for(auto par: dijagonale) {
		Tacka t1 = poligon[par.first];
		Tacka t2 = poligon[par.second];
		Duz(t1, t2).Crtaj(Slika);
	}
    string poruka = "Broj trouglova: " + to_string(dijagonale.size() + 1);
	ShowMessage(poruka.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SlikaMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y)
{
	string koordinate = "X: " + to_string(X) + " Y: " + to_string(Y);
    TextKoordinate->Text = koordinate.c_str();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonGenerisiHorVerDuziClick(TObject *Sender)
{
	int broj_duzi = EditBrojTacaka->Text.ToInt();
	for (int i = 0; i < broj_duzi; i++) {
		int x1,x2,y1,y2;
		if (rand() % 2 == 0) { // horizontalna
			x1 = rand() % Slika->Width;
			x2 = rand() % Slika->Width;
			y1 = rand() % Slika->Height;
			y2 = y1;
		}
		else { // vertikalna
			y1 = rand() % Slika->Height;
			y2 = rand() % Slika->Height;
			x1 = rand() % Slika->Width;
			x2 = x1;
		}
		Duz nova(Tacka(x1, y1), Tacka(x2, y2));
		nova.Crtaj(Slika, clBlue);
		nova.A.Crtaj(Slika, clRed);
		nova.B.Crtaj(Slika, clRed);
		duzi.push_back(nova);
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ButtonPresjekHorVerDuziClick(TObject *Sender)
{
	priority_queue<pair<Duz*, Tacka>, vector<pair<Duz*, Tacka>>, HorVerDuziPoX> eventi;

	for(int i=0; i<duzi.size(); i++) {
		if (duzi[i].horizontalna()) {
			eventi.push({&duzi[i], duzi[i].A});
			eventi.push({&duzi[i], duzi[i].B});
		}
		else {
			eventi.push({&duzi[i], duzi[i].A});
		}
	}

	set<Duz*, AktivneHorVerPoY> aktivne;
	vector<Tacka> presjeci;

	while(!eventi.empty()) {
		pair<Duz*, Tacka> trenutni = eventi.top();
		eventi.pop();

		Duz* trenutna_duz = trenutni.first;
		Tacka trenutna_tacka = trenutni.second;

		if (trenutna_duz->horizontalna()) {
			if (trenutna_tacka == trenutna_duz->A) { // pocetak H
				aktivne.insert(trenutna_duz);
				trenutna_tacka.Crtaj(Slika, clGreen);
				trenutna_duz->Crtaj(Slika, clGreen);
			   //	ShowMessage("");
			}
			else { // kraj H
				aktivne.erase(trenutna_duz);
				trenutna_tacka.Crtaj(Slika, clGray);
				trenutna_duz->Crtaj(Slika, clRed);
			   //	ShowMessage("");
            }
		}
		else {  // vertikalna
			trenutna_duz->Crtaj(Slika, clGreen);
			//ShowMessage("");
			double gornji_y = trenutna_duz->A.y;
			double donji_y = trenutna_duz->B.y;
			Duz temp_d1 = Duz(Tacka(0, gornji_y), Tacka(0, gornji_y));
			Duz temp_d2 = Duz(Tacka(0, donji_y), Tacka(0, donji_y));

			auto it_gornji = aktivne.lower_bound(&temp_d1);
			auto it_donji = aktivne.upper_bound(&temp_d2);

		   //	ShowMessage("gornji_y");
		  //	ShowMessage(gornji_y);

		   //	ShowMessage("donji_y");
		  //	ShowMessage(donji_y);
		  //	ShowMessage("lower");
		  //	if (it_gornji != aktivne.end()) {
		  //		ShowMessage((*it_gornji)->A.y);
		  //	}


		   //	ShowMessage("upper");
			//if (it_donji != aktivne.end()) {
		   //		ShowMessage((*it_donji)->A.y);
		   //	}

			for(auto it = it_gornji; it != it_donji; it++) {
				Tacka presjek(trenutna_duz->A.x, (*it)->A.y);
				presjeci.push_back(presjek);
				presjek.Crtaj(Slika, clYellow, 10);
            }
		}

	}

	if (presjeci.size() == 0) {
        ShowMessage("Nema presjeka!");
	}
}
//---------------------------------------------------------------------------

