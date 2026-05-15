#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct Bilet Bilet;
typedef struct Nod Nod;

// Structura pentru un bilet
struct Bilet {
	int nrLocuri;
	char* destinatie;
};

// Nod pentru lista simpla inlantuita
struct Nod {
	Bilet info;
	Nod* next;
};

// Functie pt crearea unui bilet
Bilet createBilet(const char* destinatie, int nrLocuri) {
	Bilet bilet; 
	bilet.destinatie = (char*)malloc(sizeof(char) * (strlen(destinatie) + 1)); // alocam memorie pentru destinatie
	strcpy(bilet.destinatie, destinatie); // copiem destinatia
	bilet.nrLocuri = nrLocuri; // salvam numarul de locuri
	return bilet;
}

// Functie pentru crearea unui nod
Nod* createNod(Bilet bilet, Nod* next) {
	Nod* nou = (Nod*)malloc(sizeof(Nod)); // alocam memorie pentru nod
	nou->info = bilet; // salvam informatia in nod
	nou->next = next; // legam nodul de urmatorul nod
	return nou;
}

// Functie pentru inserarea unui bilet la sfarsitul listei
Nod* inserareSfarsit(Nod* cap, Bilet bilet) {
	Nod* nou = createNod(bilet, NULL);
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

// Functie pentru inserarea unui bilet la inceputul listei
Nod* inserareInceput(Nod* cap, Bilet bilet) {
	Nod* nou = createNod(bilet, cap);
	return nou;
}

// Functie pentru afisarea unui bilet
void afisareBilet(Bilet bilet) {
	printf("Biletul spre %s are %d locuri rezervate.\n",
		bilet.destinatie, bilet.nrLocuri);
}

// Functie pentru afisarea tuturor biletelor din lista
void afisareListaBilete(Nod* cap) {
	while (cap) {
		afisareBilet(cap->info); // afisam biletul curent
		cap = cap->next; // trecem la urmatorul nod
	}
}

// Functie pentru calcularea mediei locurilor rezervate
float medieLocuri(Nod* cap) {
	// verificam daca lista nu este goala
	if (cap) {
		int nr = 0;
		int suma = 0;
		// parcurgem lista, calculam suma locurilor rezervate si numarul de bilete
		while (cap) {
			nr++;
			suma += cap->info.nrLocuri;
			cap = cap->next;
		}

		return (float)suma / nr;
	}
	else {
		return 0;
	}
}

// Dezalocare lista
void stergereLista(Nod* cap) {
	while (cap) {
		Nod* aux = cap;
		cap = cap->next;

		free(aux->info.destinatie);
		free(aux);
	}
}

int main() {
	Nod* cap = NULL; // initializam lista ca fiind goala
	// adaugam cateva bilete in lista
	cap = inserareSfarsit(cap, createBilet("Paris", 2));
	cap = inserareSfarsit(cap, createBilet("Roma", 4));
	cap = inserareSfarsit(cap, createBilet("Madrid", 3));

	printf("Lista initiala:\n");
	afisareListaBilete(cap);

	// adaugam un bilet la inceputul listei
	cap = inserareInceput(cap, createBilet("Londra", 5));

	printf("\nLista dupa inserare la inceput:\n");
	afisareListaBilete(cap);

	printf("\nMedia locurilor rezervate: %.2f\n", medieLocuri(cap));

	stergereLista(cap);

	return 0;
}