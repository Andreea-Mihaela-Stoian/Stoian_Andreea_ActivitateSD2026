#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: AVL cu medicamente
// Criteriu de inserare: id-ul medicamentului

struct StructuraMedicament {
	int id;
	int stoc;
	float pret;
	char* denumire;
	char* producator;
	unsigned char cod;
};
typedef struct StructuraMedicament Medicament;

// Nod pentru arbore AVL
struct Nod {
	Medicament info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

// Citire medicament din fisier
Medicament citireMedicamentDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);

	char* aux;

	Medicament m;

	aux = strtok(buffer, sep); 
	m.id = atoi(aux);

	m.stoc = atoi(strtok(NULL, sep));

	m.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m.denumire = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m.denumire, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m.producator = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m.producator, strlen(aux) + 1, aux);

	m.cod = *strtok(NULL, sep);

	return m;
}

// Afisare medicament
void afisareMedicament(Medicament medicament) {
	printf("Id: %d\n", medicament.id);
	printf("Stoc: %d\n", medicament.stoc);
	printf("Pret: %.2f\n", medicament.pret);
	printf("Denumire: %s\n", medicament.denumire);
	printf("Producator: %s\n", medicament.producator);
	printf("Cod: %c\n\n", medicament.cod);
}

// Calculeaza maximul dintre doua valori
int maxim(int a, int b) {
	return a > b ? a : b;
}

// Calculeaza inaltimea arborelui
int calculeazaInaltimeArbore(Nod* rad) {
	if (rad) {
		return maxim(
			calculeazaInaltimeArbore(rad->stanga),
			calculeazaInaltimeArbore(rad->dreapta)
		) + 1;
	}

	return 0;
}

// Diferenta dintre inaltimea din stanga si cea din dreapta
int calculDiferentaInaltimi(Nod* rad) {
	if (rad) {
		return calculeazaInaltimeArbore(rad->stanga)
			- calculeazaInaltimeArbore(rad->dreapta);
	}
	else {
		return 0;
	}
}

// Rotire la stanga
void rotireLaStanga(Nod** rad) {
	Nod* aux = (*rad)->dreapta;

	(*rad)->dreapta = aux->stanga;

	aux->stanga = (*rad);

	(*rad) = aux;
}

// Rotire la dreapta
void rotireLaDreapta(Nod** rad) {
	Nod* aux = (*rad)->stanga;

	(*rad)->stanga = aux->dreapta;

	aux->dreapta = (*rad);

	(*rad) = aux;
}

// Inserare medicament in AVL
void adaugaMedicamentInArbore(Nod** rad, Medicament medicamentNou) {
	// inserare normala ca intr-un ABC

	if (*rad == NULL) {
		Nod* nod = (Nod*)malloc(sizeof(Nod));

		nod->info = medicamentNou;
		nod->stanga = NULL;
		nod->dreapta = NULL;

		*rad = nod;
	}
	else {
		if ((*rad)->info.id > medicamentNou.id) {
			adaugaMedicamentInArbore(&((*rad)->stanga), medicamentNou);
		}

		if ((*rad)->info.id < medicamentNou.id) {
			adaugaMedicamentInArbore(&((*rad)->dreapta), medicamentNou);
		}
	}

	// dupa inserare verificam dezechilibrul
	int diferentaInaltimi = calculDiferentaInaltimi(*rad);

	if (diferentaInaltimi == 2) {
		// dezechilibru pe stanga

		if (calculDiferentaInaltimi((*rad)->stanga) == -1) {
			// caz stanga-dreapta
			rotireLaStanga(&(*rad)->stanga);
		}

		// caz stanga-stanga
		rotireLaDreapta(rad);
	}

	if (diferentaInaltimi == -2) {
		// dezechilibru pe dreapta

		if (calculDiferentaInaltimi((*rad)->dreapta) == 1) {
			// caz dreapta-stanga
			rotireLaDreapta(&(*rad)->dreapta);
		}

		// caz dreapta-dreapta
		rotireLaStanga(rad);
	}
}

// Citire arbore AVL din fisier
Nod* citireArboreDeMedicamenteDinFisier(const char* numeFisier) {
	Nod* rad = NULL;

	FILE* f = fopen(numeFisier, "r");

	if (f) {
		while (!feof(f)) {
			Medicament m = citireMedicamentDinFisier(f);
			adaugaMedicamentInArbore(&rad, m);
		}

		fclose(f);
	}

	return rad;
}