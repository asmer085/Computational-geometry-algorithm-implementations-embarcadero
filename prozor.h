//---------------------------------------------------------------------------

#ifndef prozorH
#define prozorH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "pomocna.h"
#include <vector>
#include <list>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TImage *Slika;
	TButton *ButtonProstiMnogougao;
	TButton *ButtonOcisti;
	TButton *ButtonGenerisiTacke;
	TEdit *EditBrojTacaka;
	TRadioButton *RadioDodajTacku;
	TRadioButton *RadioDodajDuz;
	TRadioButton *RadioDodajTrokut;
	TButton *ButtonPresjekDuzi;
	TButton *ButtonUvijanjePoklona;
	TButton *ButtomGrahamScan;
	TRadioButton *RadioTackaUnutarKonveksnog;
	TRadioButton *RadioTangente;
	TButton *ButtonKonveksniInduktivno;
	TRadioButton *RadioCrtajPoligon;
	TButton *ButtonTriangulacija;
	TEdit *TextKoordinate;
	TButton *ButtonGenerisiHorVerDuzi;
	TButton *ButtonPresjekHorVerDuzi;
	void __fastcall SlikaMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall ButtonProstiMnogougaoClick(TObject *Sender);
	void __fastcall ButtonOcistiClick(TObject *Sender);
	void __fastcall ButtonGenerisiTackeClick(TObject *Sender);
	void __fastcall ButtonPresjekDuziClick(TObject *Sender);
	void __fastcall ButtonUvijanjePoklonaClick(TObject *Sender);
	void __fastcall ButtomGrahamScanClick(TObject *Sender);
	void __fastcall ButtonKonveksniInduktivnoClick(TObject *Sender);
	void __fastcall ButtonTriangulacijaClick(TObject *Sender);
	void __fastcall SlikaMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall ButtonGenerisiHorVerDuziClick(TObject *Sender);
	void __fastcall ButtonPresjekHorVerDuziClick(TObject *Sender);

private:	// User declarations
	vector<Tacka> tacke;
	vector<Tacka> konveksni_omotac;
	vector<Tacka> poligon;
	bool zavrsen_poligon;
	vector<Duz> duzi;
	vector<pair<int, int>> dijagonale;
	bool drugi_klik = false;
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
