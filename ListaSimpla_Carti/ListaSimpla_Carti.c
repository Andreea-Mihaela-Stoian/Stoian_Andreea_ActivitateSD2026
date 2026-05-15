#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct Carte Carte;
typedef struct Nod Nod;

// Structura pentru o carte
struct Carte {
	int nrPagini;
	char* titlu;
};

// Nod pentru lista simpla inlantuita
struct Nod {
	Carte info;
	Nod* next;
};

// Functie pt crearea unei carti
Carte createCarte(const char* titlu, int nrPagini) {
	Carte carte;
	carte.titlu = (char*)malloc(sizeof(char) * (strlen(titlu) + 1)); // alocam memorie pentru titlu
	strcpy(carte.titlu, titlu); // copiem titlul
	carte.nrPagini = nrPagini; //salvam numarul de pagini
	return carte;
}

// Functie pentru crearea unui nod
Nod* createNod(Carte carte, Nod* next) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = carte; // salvam informatia in nod
	nou->next = next; // legam nodul de urmatorul nod
	return nou;
}

// Functie pentru inserarea unei carti la sfarsitul listei
Nod* inserareSfarsit(Nod* cap, Carte carte) {
	Nod* nou = createNod(carte, NULL);
	// daca lista nu este goala, parcurgem pana la final si adaugam nodul nou
	if (cap) {
		Nod* temp = cap;

		while (temp->next) {
			temp = temp->next;
		}

		temp->next = nou;
	}
	else {
		cap = nou;
	}

	return cap;
}

// Functie pentru inserarea unei carti la inceputul listei
Nod* inserareInceput(Nod* cap, Carte carte) {
	Nod* nou = createNod(carte, cap);
	return nou;
}

// Functie pentru afisarea unei carti
void afisareCarte(Carte carte) {
	printf("Cartea %s are %d pagini.\n", carte.titlu, carte.nrPagini);
}

// Functie pentru afisarea tuturor cartilor din lista
void afisareListaCarti(Nod* cap) {
	while (cap) {
		afisareCarte(cap->info);
		cap = cap->next;
	}
}

// Functie pentru calcularea mediei paginilor
float mediePagini(Nod* cap) {
	// verificam  daca lista exista
	if (cap) {
		int nr = 0;
		int suma = 0;
		// parcurgem lista si calculam suma paginilor si numarul de carti
		while (cap) {
			nr++;
			suma += cap->info.nrPagini;
			cap = cap->next;
		}
		// returnam media
		return (float)suma / nr;
	}
	else {
		return 0;
	}
}

// Dezalocare lista
void stergereLista(Nod* cap) {
	while (cap) {
		Nod* aux = cap; // salvam nodul curent
		cap = cap->next; // trecem la urmatorul nod

		free(aux->info.titlu); // dezalocam memoria pentru titlu
		free(aux);
	}
}

int main() { 
	Nod* cap = NULL; //initializam lista ca fiind goala

	// inseram carti in lista
	cap = inserareSfarsit(cap, createCarte("Ion", 350));
	cap = inserareSfarsit(cap, createCarte("Morometii", 420));
	cap = inserareSfarsit(cap, createCarte("Baltagul", 180));

	printf("Lista initiala:\n");
	afisareListaCarti(cap);

	// inseram o carte la inceputul listei
	cap = inserareInceput(cap, createCarte("Enigma Otiliei", 300));

	printf("\nLista dupa inserare la inceput:\n");
	afisareListaCarti(cap);

	printf("\nMedia paginilor: %.2f\n", mediePagini(cap));

	stergereLista(cap);

	return 0;
}