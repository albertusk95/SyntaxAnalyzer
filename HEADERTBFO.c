#include "HEADERTBFO.h"

AddressStack AlokasiStack (char *X) {
	AddressStack PAlok;
	PAlok = (AddressStack) malloc (sizeof(ElmtStack));
	if (PAlok != Nil) {
		strcpy(InfoStack(PAlok), X);
		NextStack(PAlok) = Nil;
	}
	return PAlok;
}

void DealokasiStack (AddressStack *P) {
	free (*P);
}

void CreateStack (Stack *S) {
	Top(*S) = Nil;
}

boolean isStackEmpty (Stack S) {
	if (Top(S) == Nil) {
		return true;
	}
	else {
		return false;
	}
}

boolean isOneElmtStack(Stack S) {
	if (NextStack(Top(S)) == Nil) {
		return true;
	}
	else {
		return false;
	}
}

void Push (Stack *S, char *X) {
	AddressStack PAlok, tempS;
	PAlok = AlokasiStack(X);
	if (PAlok != Nil) {
		if (isStackEmpty(*S) == true) {
			Top(*S) = PAlok;
		}
		else {
			tempS = Top(*S);
			Top(*S) = PAlok;
			NextStack(PAlok) = tempS; 
		}
	}
	else {
		printf("Alokasi gagal [Push]\n");
	}
}

void Pop (Stack *S) {
	AddressStack tempS;
	
	tempS = NextStack(Top(*S));
	if (isOneElmtStack(*S) == true) {
		DealokasiStack(&Top(*S));
		Top(*S) = Nil;
	}
	else {
		DealokasiStack(&Top(*S));
		Top(*S) = tempS;
	}
}

Address Alokasi (char *str, int baris) {
/*	Mengirimkan Address hasil alokasi sebuah elemen
	Jika alokasi berhasil, maka Address tidak Nil, dan misalnya menghasilkan P, maka
	Info(P) = X, Next(P) = Nil
	Jika alokasi gagal, mengirimkan Nil
*/
	Address PAlok;
	PAlok = (Address) malloc (sizeof(ElmtList));
	if (PAlok != Nil) {
		strcpy(InfoToken(PAlok).namaToken, str);
		InfoToken(PAlok).barisToken = baris;
		NextToken(PAlok) = Nil;
	}
	
	return PAlok;
}

void Dealokasi (Address *P) {
/*	I.S. P terdefinisi
	F.S. P dikembalikan ke sistem
	Melakukan dealokasi/pengembalian Address P
*/
	free(*P);
}

boolean IsListEmpty (ListToken L) {
/* Mengirim true jika list kosong */
	if (FirstToken(L) == Nil) {
		return true;
	} else {
		return false;
	}
}

void CreateList (ListToken *L) {
/*	I.S. sembarang
	F.S. Terbentuk list kosong 
*/
	FirstToken(*L) = Nil;
}

void InsertLast (ListToken *L, Address X) {
/*	I.S. Sembarang, P sudah dialokasi
	F.S. P ditambahkan sebagai elemen terakhir yang baru
*/
	Address P;
	
	if (X != Nil) {
		if (IsListEmpty(*L) == true) {
			FirstToken(*L) = X;
		}
		else {
			P = FirstToken(*L);
			while (P != Nil) {
				if (NextToken(P) == Nil) {
					NextToken(P) = X;
					break;
				}
				P = NextToken(P);
			}
		}
	} else {
		printf("Alokasi gagal [InsertLast]\n");
	}
}