#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: Arbore binar de cautare cu retete
// Criteriu de inserare: id-ul retetei

struct StructuraReteta {
	int id;
	int timpPreparare;
	float cost;
	char* denumire;
	char* taraOrigine;
	unsigned char cod;
};
typedef struct StructuraReteta Reteta;

// Nod pentru arbore binar de cautare
struct Nod {
	Reteta info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

// Citire reteta din fisier
Reteta citireRetetaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);

	char* aux;
	Reteta r;

	aux = strtok(buffer, sep);
	r.id = atoi(aux);

	r.timpPreparare = atoi(strtok(NULL, sep));

	r.cost = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	r.denumire = (char*)malloc(strlen(aux) + 1);
	strcpy_s(r.denumire, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	r.taraOrigine = (char*)malloc(strlen(aux) + 1);
	strcpy_s(r.taraOrigine, strlen(aux) + 1, aux);

	r.cod = *strtok(NULL, sep);

	return r;
}

// Afisare reteta
void afisareReteta(Reteta reteta) {
	printf("Id: %d\n", reteta.id);
	printf("Timp preparare: %d\n", reteta.timpPreparare);
	printf("Cost: %.2f\n", reteta.cost);
	printf("Denumire: %s\n", reteta.denumire);
	printf("Tara origine: %s\n", reteta.taraOrigine);
	printf("Cod: %c\n\n", reteta.cod);
}

// Inserare in arbore dupa id
void adaugaRetetaInArbore(Nod** rad, Reteta retetaNoua) {
	if (*rad == NULL) {
		// daca arborele este gol, cream nod nou
		Nod* nod = (Nod*)malloc(sizeof(Nod));

		nod->info = retetaNoua;
		nod->stanga = NULL;
		nod->dreapta = NULL;

		*rad = nod;
	}
	else {
		// daca id-ul nou este mai mic, mergem in stanga
		if ((*rad)->info.id > retetaNoua.id) {
			adaugaRetetaInArbore(&(*rad)->stanga, retetaNoua);
		}

		// daca id-ul nou este mai mare, mergem in dreapta
		if ((*rad)->info.id < retetaNoua.id) {
			adaugaRetetaInArbore(&(*rad)->dreapta, retetaNoua);
		}
	}
}

// Citire arbore din fisier
Nod* citireArboreDeReteteDinFisier(const char* numeFisier) {
	Nod* rad = NULL;

	FILE* f = fopen(numeFisier, "r");

	if (f) {
		while (!feof(f)) {
			Reteta r = citireRetetaDinFisier(f);
			adaugaRetetaInArbore(&rad, r);
		}

		fclose(f);
	}

	return rad;
}

// Afisare in ordine: stanga - radacina - dreapta
void afisareReteteInordine(Nod* rad) {
	if (rad) {
		afisareReteteInordine(rad->stanga);
		afisareReteta(rad->info);
		afisareReteteInordine(rad->dreapta);
	}
}

// Afisare preordine: radacina - stanga - dreapta
void afisarePreordine(Nod* rad) {
	if (rad) {
		afisareReteta(rad->info);
		afisarePreordine(rad->stanga);
		afisarePreordine(rad->dreapta);
	}
}

// Cautare reteta dupa id
Reteta getRetetaByID(Nod* rad, int id) {
	Reteta r;

	r.id = -1;
	r.timpPreparare = 0;
	r.cost = 0;
	r.denumire = NULL;
	r.taraOrigine = NULL;
	r.cod = '-';

	if (rad) {
		if (rad->info.id == id) {
			r = rad->info;

			// copiere profunda pentru campurile char*
			r.denumire = (char*)malloc(strlen(rad->info.denumire) + 1);
			strcpy_s(r.denumire, strlen(rad->info.denumire) + 1, rad->info.denumire);

			r.taraOrigine = (char*)malloc(strlen(rad->info.taraOrigine) + 1);
			strcpy_s(r.taraOrigine, strlen(rad->info.taraOrigine) + 1, rad->info.taraOrigine);
		}

		if (id < rad->info.id) {
			r = getRetetaByID(rad->stanga, id);
		}

		if (id > rad->info.id) {
			r = getRetetaByID(rad->dreapta, id);
		}
	}

	return r;
}

// Numar total de noduri
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

// Calculeaza costul total al retetelor
float calculeazaCostTotal(Nod* rad) {
	if (rad) {
		return rad->info.cost
			+ calculeazaCostTotal(rad->stanga)
			+ calculeazaCostTotal(rad->dreapta);
	}

	return 0;
}

// Calculeaza timpul total pentru retetele dintr-o tara
int calculeazaTimpReteteDinTara(Nod* rad, const char* tara) {
	if (rad) {
		int suma = calculeazaTimpReteteDinTara(rad->stanga, tara)
			+ calculeazaTimpReteteDinTara(rad->dreapta, tara);

		if (strcmp(rad->info.taraOrigine, tara) == 0) {
			suma += rad->info.timpPreparare;
		}

		return suma;
	}

	return 0;
}

// Dezalocare arbore
void dezalocareArboreDeRetete(Nod** rad) {
	if (*rad) {
		dezalocareArboreDeRetete(&(*rad)->stanga);
		dezalocareArboreDeRetete(&(*rad)->dreapta);

		free((*rad)->info.denumire);
		free((*rad)->info.taraOrigine);

		free(*rad);

		*rad = NULL;
	}
}

int main() {
	Nod* rad = citireArboreDeReteteDinFisier("retete.txt");

	printf("Afisare preordine:\n");
	afisarePreordine(rad);

	printf("Afisare inordine:\n");
	afisareReteteInordine(rad);

	printf("Reteta cautata:\n");
	Reteta retetaCautata = getRetetaByID(rad, 8);
	afisareReteta(retetaCautata);

	free(retetaCautata.denumire);
	free(retetaCautata.taraOrigine);

	printf("Numar noduri: %d\n", determinaNumarNoduri(rad));

	printf("Cost total: %.2f\n", calculeazaCostTotal(rad));

	printf("Timp total retete din Italia: %d\n",
		calculeazaTimpReteteDinTara(rad, "Italia"));

	dezalocareArboreDeRetete(&rad);

	return 0;
}