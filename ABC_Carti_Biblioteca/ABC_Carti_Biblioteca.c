#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: Arbore binar de cautare cu carti
// Criteriu de inserare: id-ul cartii

struct StructuraCarte {
	int id;
	int anPublicare;
	float pret;
	char* titlu;
	char* autor;
	unsigned char codRaft;
};
typedef struct StructuraCarte Carte;

// Nod pentru arbore binar de cautare
struct Nod {
	Carte info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

// Citeste o carte din fisier
Carte citireCarteDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	Carte c;
	c.id = -1;
	c.anPublicare = 0;
	c.pret = 0;
	c.titlu = NULL;
	c.autor = NULL;
	c.codRaft = '-';

	if (fgets(buffer, 100, file) == NULL) {
		return c;
	}

	char* aux;

	aux = strtok(buffer, sep);
	if (aux == NULL) {
		return c;
	}

	c.id = atoi(aux);
	c.anPublicare = atoi(strtok(NULL, sep));
	c.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	c.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.titlu, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	c.autor = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.autor, strlen(aux) + 1, aux);

	c.codRaft = *strtok(NULL, sep);

	return c;
}

// Afiseaza o carte
void afisareCarte(Carte carte) {
	printf("Id: %d\n", carte.id);
	printf("An publicare: %d\n", carte.anPublicare);
	printf("Pret: %.2f\n", carte.pret);
	printf("Titlu: %s\n", carte.titlu);
	printf("Autor: %s\n", carte.autor);
	printf("Cod raft: %c\n\n", carte.codRaft);
}
void adaugaCarteInArbore(Nod** rad, Carte carteNoua) {
	// adauga o carte in arbore dupa id

	if (*rad == NULL) {
		Nod* nod = (Nod*)malloc(sizeof(Nod));

		nod->info = carteNoua;
		nod->stanga = NULL;
		nod->dreapta = NULL;

		*rad = nod;
	}
	else {
		if ((*rad)->info.id > carteNoua.id) {
			adaugaCarteInArbore(&(*rad)->stanga, carteNoua);
		}

		if ((*rad)->info.id < carteNoua.id) {
			adaugaCarteInArbore(&(*rad)->dreapta, carteNoua);
		}
	}
}

Nod* citireArboreDeCartiDinFisier(const char* numeFisier) {
	// citeste cartile din fisier si le adauga in arbore

	Nod* rad = NULL;

	FILE* f = fopen(numeFisier, "r");

	if (f) {
		while (!feof(f)) {
			Carte c = citireCarteDinFisier(f);

			if (c.id != -1) {
				adaugaCarteInArbore(&rad, c);
			}
		}

		fclose(f);
	}
	else {
		printf("Fisierul nu a fost gasit!\n");
	}

	return rad;
}

void afisareCartiInordine(Nod* rad) {
	// inordine: stanga - radacina - dreapta
	// afiseaza cartile crescator dupa id

	if (rad) {
		afisareCartiInordine(rad->stanga);
		afisareCarte(rad->info);
		afisareCartiInordine(rad->dreapta);
	}
}

void afisarePreordine(Nod* rad) {
	// preordine: radacina - stanga - dreapta

	if (rad) {
		afisareCarte(rad->info);
		afisarePreordine(rad->stanga);
		afisarePreordine(rad->dreapta);
	}
}