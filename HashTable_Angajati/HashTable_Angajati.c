#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: HashTable cu angajati
// Criteriu hash: id angajat
// Tratarea coliziunilor: chaining, adica liste simplu inlantuite

struct StructuraAngajat {
	int id;
	int vechime;
	float salariu;
	char* nume;
	char* departament;
	unsigned char cod;
};
typedef struct StructuraAngajat Angajat;

// Nod pentru lista simplu inlantuita
struct Nod {
	Angajat info;
	struct Nod* next;
};
typedef struct Nod Nod;

// HashTable = vector de liste
struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

// Citeste un angajat din fisier
Angajat citireAngajatDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	Angajat a;
	a.id = -1;
	a.vechime = 0;
	a.salariu = 0;
	a.nume = NULL;
	a.departament = NULL;
	a.cod = '-';

	if (fgets(buffer, 100, file) == NULL) {
		return a;
	}

	char* aux;

	aux = strtok(buffer, sep);
	if (aux == NULL) {
		return a;
	}

	a.id = atoi(aux);
	a.vechime = atoi(strtok(NULL, sep));
	a.salariu = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	a.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(a.nume, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	a.departament = (char*)malloc(strlen(aux) + 1);
	strcpy_s(a.departament, strlen(aux) + 1, aux);

	a.cod = *strtok(NULL, sep);

	return a;
}

// Afiseaza un angajat
void afisareAngajat(Angajat angajat) {
	printf("Id: %d\n", angajat.id);
	printf("Vechime: %d\n", angajat.vechime);
	printf("Salariu: %.2f\n", angajat.salariu);
	printf("Nume: %s\n", angajat.nume);
	printf("Departament: %s\n", angajat.departament);
	printf("Cod: %c\n\n", angajat.cod);
}
// Afiseaza lista simplu inlantuita
void afisareListaAngajati(Nod* cap) {
	while (cap) {
		afisareAngajat(cap->info);
		cap = cap->next;
	}
}

// Adauga angajat la finalul listei
void adaugaAngajatInLista(Nod** cap, Angajat angajatNou) {

	Nod* nou = (Nod*)malloc(sizeof(Nod));

	nou->info = angajatNou;
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		Nod* aux = *cap;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;
	}
}

// Initializare HashTable
HashTable initializareHashTable(int dimensiune) {

	HashTable ht;

	ht.dim = dimensiune;

	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);

	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}

	return ht;
}

// Calcul hash
int calculeazaHash(int id, int dimensiune) {

	// folosim modulo pentru a obtine pozitia
	return id % dimensiune;
}

// Inserare angajat in tabela
void inserareAngajatInTabela(HashTable ht, Angajat angajat) {

	int pozitie =
		calculeazaHash(angajat.id, ht.dim);

	adaugaAngajatInLista(
		&(ht.vector[pozitie]),
		angajat);
}

// Citire angajati din fisier
HashTable citireAngajatiDinFisier(
	const char* numeFisier) {

	HashTable ht =
		initializareHashTable(5);

	FILE* f = fopen(numeFisier, "r");

	if (f) {

		while (!feof(f)) {

			Angajat a =
				citireAngajatDinFisier(f);

			if (a.id != -1) {

				inserareAngajatInTabela(
					ht,
					a);
			}
		}

		fclose(f);
	}

	return ht;
}

// Afisare tabela
void afisareTabelaDeAngajati(HashTable ht) {

	for (int i = 0; i < ht.dim; i++) {

		printf("\nCluster %d:\n", i);

		afisareListaAngajati(
			ht.vector[i]);
	}
}
// Dezalocare tabela
void dezalocareTabelaDeAngajati(HashTable* ht) {

	for (int i = 0; i < ht->dim; i++) {

		Nod* cap = ht->vector[i];

		while (cap) {

			Nod* aux = cap;

			cap = cap->next;

			free(aux->info.nume);
			free(aux->info.departament);

			free(aux);
		}
	}

	free(ht->vector);

	ht->vector = NULL;
	ht->dim = 0;
}

// Calculeaza salariul mediu pe fiecare cluster
float* calculeazaSalariiMediiPerClustere(
	HashTable ht,
	int* nrClustere) {

	float* vectorMedii =
		(float*)malloc(sizeof(float) * ht.dim);

	*nrClustere = 0;

	for (int i = 0; i < ht.dim; i++) {

		float suma = 0;
		int nr = 0;

		Nod* cap = ht.vector[i];

		while (cap) {

			suma += cap->info.salariu;

			nr++;

			cap = cap->next;
		}

		if (nr > 0) {

			vectorMedii[*nrClustere] =
				suma / nr;

			(*nrClustere)++;
		}
	}

	return vectorMedii;
}

// Cauta angajat dupa id
Angajat getAngajatDupaID(
	HashTable ht,
	int idCautat) {

	Angajat a;

	a.id = -1;
	a.vechime = 0;
	a.salariu = 0;
	a.nume = NULL;
	a.departament = NULL;
	a.cod = '-';

	int pozitie =
		calculeazaHash(idCautat, ht.dim);

	Nod* cap = ht.vector[pozitie];

	while (cap) {

		if (cap->info.id == idCautat) {

			a = cap->info;

			a.nume =
				(char*)malloc(
					strlen(cap->info.nume) + 1);

			strcpy_s(
				a.nume,
				strlen(cap->info.nume) + 1,
				cap->info.nume);

			a.departament =
				(char*)malloc(
					strlen(cap->info.departament) + 1);

			strcpy_s(
				a.departament,
				strlen(cap->info.departament) + 1,
				cap->info.departament);

			return a;
		}

		cap = cap->next;
	}

	return a;
}

int main() {

	HashTable ht =
		citireAngajatiDinFisier(
			"angajati.txt");

	printf("Tabela hash:\n");

	afisareTabelaDeAngajati(ht);

	printf("\nAngajat cautat:\n");

	Angajat a =
		getAngajatDupaID(ht, 103);

	afisareAngajat(a);

	free(a.nume);
	free(a.departament);

	int nrClustere = 0;

	float* medii =
		calculeazaSalariiMediiPerClustere(
			ht,
			&nrClustere);

	printf("\nSalarii medii:\n");

	for (int i = 0; i < nrClustere; i++) {

		printf("%.2f\n", medii[i]);
	}

	free(medii);

	dezalocareTabelaDeAngajati(&ht);

	return 0;
}