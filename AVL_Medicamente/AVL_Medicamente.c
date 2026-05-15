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
// Afisare in ordine: stanga - radacina - dreapta
void afisareMedicamenteInordine(Nod* rad) {
	if (rad) {
		afisareMedicamenteInordine(rad->stanga);
		afisareMedicament(rad->info);
		afisareMedicamenteInordine(rad->dreapta);
	}
}

// Afisare preordine: radacina - stanga - dreapta
void afisarePreordine(Nod* rad) {
	if (rad) {
		afisareMedicament(rad->info);
		afisarePreordine(rad->stanga);
		afisarePreordine(rad->dreapta);
	}
}

// Cautare medicament dupa id
Medicament getMedicamentByID(Nod* rad, int id) {
	Medicament m;

	m.id = -1;
	m.stoc = 0;
	m.pret = 0;
	m.denumire = NULL;
	m.producator = NULL;
	m.cod = '-';
	
	if (rad) {
		if (rad->info.id == id) {
			m = rad->info;

			m.denumire = (char*)malloc(strlen(rad->info.denumire) + 1);
			strcpy_s(m.denumire, strlen(rad->info.denumire) + 1, rad->info.denumire);

			m.producator = (char*)malloc(strlen(rad->info.producator) + 1);
			strcpy_s(m.producator, strlen(rad->info.producator) + 1, rad->info.producator);
		}

		if (id < rad->info.id) {
			m = getMedicamentByID(rad->stanga, id);
		}

		if (id > rad->info.id) {
			m = getMedicamentByID(rad->dreapta, id);
		}
	}

	return m;
}

// Determina numarul total de noduri
int determinaNumarNoduri(Nod* rad) {
	if (rad) {
		return determinaNumarNoduri(rad->stanga)
			+ determinaNumarNoduri(rad->dreapta)
			+ 1;
	}
	else {
		return 0;
	}
}

// Calculeaza valoarea totala a medicamentelor
float calculeazaValoareTotala(Nod* rad) {
	if (rad) {
		return rad->info.pret
			+ calculeazaValoareTotala(rad->stanga)
			+ calculeazaValoareTotala(rad->dreapta);
	}

	return 0;
}

// Calculeaza stocul total pentru un producator
int calculeazaStocPentruProducator(Nod* rad, const char* producator) {
	if (rad) {
		int suma = calculeazaStocPentruProducator(rad->stanga, producator)
			+ calculeazaStocPentruProducator(rad->dreapta, producator);

		if (strcmp(rad->info.producator, producator) == 0) {
			suma += rad->info.stoc;
		}

		return suma;
	}

	return 0;
}

// Dezalocare arbore AVL
void dezalocareArboreDeMedicamente(Nod** rad) {
	if (*rad) {
		dezalocareArboreDeMedicamente(&(*rad)->stanga);
		dezalocareArboreDeMedicamente(&(*rad)->dreapta);

		free((*rad)->info.denumire);
		free((*rad)->info.producator);

		free(*rad);

		*rad = NULL;
	}
}

int main() {
	Nod* rad = citireArboreDeMedicamenteDinFisier("medicamente.txt");

	printf("Afisare preordine:\n");
	afisarePreordine(rad);

	printf("Afisare inordine:\n");
	afisareMedicamenteInordine(rad);

	printf("Medicament cautat:\n");
	Medicament medicamentCautat = getMedicamentByID(rad, 8);
	afisareMedicament(medicamentCautat);

	free(medicamentCautat.denumire);
	free(medicamentCautat.producator);

	printf("Numar noduri: %d\n", determinaNumarNoduri(rad));

	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(rad));

	printf("Valoare totala: %.2f\n", calculeazaValoareTotala(rad));

	printf("Stoc total pentru Zentiva: %d\n",
		calculeazaStocPentruProducator(rad, "Zentiva"));

	dezalocareArboreDeMedicamente(&rad);

	return 0;
}