#ifndef HEADERTBFO_H
#define HEADERTBFO_H

#include "boolean.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Nil NULL

/** LIST TOKEN **/
typedef struct {
	char namaToken[100];
	int barisToken;
} Infotype;

typedef struct tElmtList *Address;
typedef struct tElmtList {
	Infotype info;
	Address next;
} ElmtList;

typedef struct {
	Address first;
} ListToken;

/** STACK **/
typedef struct tElmtStack *AddressStack;
typedef struct tElmtStack {
	char isiStack[100];
	AddressStack nextStack;
} ElmtStack;

typedef struct {
	AddressStack firstStack;
} Stack;

/** DEFINE LIST TOKEN **/
#define FirstToken(L) (L).first
#define NextToken(P) (P)->next
#define InfoToken(P) (P)->info

/** DEFINE STACK **/
#define Top(S) (S).firstStack
#define NextStack(P) (P)->nextStack
#define InfoStack(P) (P)->isiStack

/** PRIMITIF STACK **/
AddressStack AlokasiStack (char *X);

void DealokasiStack (AddressStack *P);

void CreateStack (Stack *S);

boolean isStackEmpty (Stack S);

boolean isOneElmtStack(Stack S);

void Push (Stack *S, char *X);

void Pop (Stack *S);

/** PRIMITIF LIST TOKEN **/
Address Alokasi (char *str, int baris);
/*	Mengirimkan Address hasil alokasi sebuah elemen
	Jika alokasi berhasil, maka Address tidak Nil, dan misalnya menghasilkan P, maka
	Info(P) = X, Next(P) = Nil
	Jika alokasi gagal, mengirimkan Nil
*/

void Dealokasi (Address *P);
/*	I.S. P terdefinisi
	F.S. P dikembalikan ke sistem
	Melakukan dealokasi/pengembalian Address P
*/

boolean IsListEmpty (ListToken L);
/* Mengirim true jika list kosong */

void CreateList (ListToken *L);
/*	I.S. sembarang
	F.S. Terbentuk list kosong 
*/

void InsertLast (ListToken *L, Address X);
/*	I.S. Sembarang, P sudah dialokasi
	F.S. P ditambahkan sebagai elemen terakhir yang baru
*/

#endif
