#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: HashTable cu animale
// Criteriu hash: id animal

struct StructuraAnimal {
	int id;
	int varsta;
	float greutate;
	char* specie;
	char* continent;
	unsigned char cod;
};
typedef struct StructuraAnimal Animal;

// Nod pentru lista simpla inlantuita
struct Nod {
	Animal info;
	struct Nod* next;
};
typedef struct Nod Nod;

// Structura HashTable
struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

// Citire animal din fisier
Animal citireAnimalDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);

	char* aux;

	Animal a;

	aux = strtok(buffer, sep);
	a.id = atoi(aux);

	a.varsta = atoi(strtok(NULL, sep));

	a.greutate = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	a.specie = (char*)malloc(strlen(aux) + 1);
	strcpy_s(a.specie, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	a.continent = (char*)malloc(strlen(aux) + 1);
	strcpy_s(a.continent, strlen(aux) + 1, aux);

	a.cod = *strtok(NULL, sep);

	return a;
}

// Afisare animal
void afisareAnimal(Animal animal) {
	printf("Id: %d\n", animal.id);
	printf("Varsta: %d\n", animal.varsta);
	printf("Greutate: %.2f\n", animal.greutate);
	printf("Specie: %s\n", animal.specie);
	printf("Continent: %s\n", animal.continent);
	printf("Cod: %c\n\n", animal.cod);
}

// Afisare lista animale
void afisareListaAnimale(Nod* cap) {
	while (cap) {
		afisareAnimal(cap->info);
		cap = cap->next;
	}
}

// Inserare animal la finalul listei
void adaugaAnimalInLista(Nod** cap, Animal animalNou) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));

	nou->info = animalNou;
	nou->next = NULL;

	if (*cap) {
		Nod* aux = *cap;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;
	}
	else {
		*cap = nou;
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

// Calcul hash dupa id
int calculeazaHash(int id, int dimensiune) {
	return id % dimensiune;
}

// Inserare animal in tabela
void inserareAnimalInTabela(HashTable hash, Animal animal) {
	int pozitie = calculeazaHash(animal.id, hash.dim);

	adaugaAnimalInLista(&(hash.vector[pozitie]), animal);
}

// Citire tabela hash din fisier
HashTable citireAnimaleDinFisier(const char* numeFisier) {
	HashTable ht = initializareHashTable(5);

	FILE* f = fopen(numeFisier, "r");

	if (f) {
		while (!feof(f)) {
			Animal a = citireAnimalDinFisier(f);

			inserareAnimalInTabela(ht, a);
		}

		fclose(f);
	}

	return ht;
}

// Afisare tabela hash
void afisareTabelaDeAnimale(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("\nCluster %d:\n", i);

		afisareListaAnimale(ht.vector[i]);
	}
}

// Cautare animal dupa id
Animal getAnimalDupaID(HashTable ht, int idCautat) {
	Animal a;

	a.id = -1;
	a.varsta = 0;
	a.greutate = 0;
	a.specie = NULL;
	a.continent = NULL;
	a.cod = '-';

	int pozitie = calculeazaHash(idCautat, ht.dim);

	Nod* cap = ht.vector[pozitie];

	while (cap) {
		if (cap->info.id == idCautat) {

			a = cap->info;

			a.specie = (char*)malloc(strlen(cap->info.specie) + 1);
			strcpy_s(a.specie, strlen(cap->info.specie) + 1, cap->info.specie);

			a.continent = (char*)malloc(strlen(cap->info.continent) + 1);
			strcpy_s(a.continent, strlen(cap->info.continent) + 1, cap->info.continent);

			return a;
		}

		cap = cap->next;
	}

	return a;
}

// Calculeaza greutatea totala
float calculeazaGreutateTotala(HashTable ht) {
	float suma = 0;

	for (int i = 0; i < ht.dim; i++) {
		Nod* cap = ht.vector[i];

		while (cap) {
			suma += cap->info.greutate;

			cap = cap->next;
		}
	}

	return suma;
}

// Dezalocare tabela hash
void dezalocareTabelaDeAnimale(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {

		Nod* cap = ht->vector[i];

		while (cap) {

			Nod* aux = cap;

			cap = cap->next;

			free(aux->info.specie);
			free(aux->info.continent);

			free(aux);
		}
	}

	free(ht->vector);

	ht->vector = NULL;
	ht->dim = 0;
}

int main() {

	HashTable ht =citireAnimaleDinFisier("animale.txt");

	printf("Tabela hash:\n");

	afisareTabelaDeAnimale(ht);

	printf("\nAnimal cautat:\n");

	Animal a =getAnimalDupaID(ht, 103);

	afisareAnimal(a);

	free(a.specie);
	free(a.continent);

	printf("Greutate totala: %.2f\n",calculeazaGreutateTotala(ht));

	dezalocareTabelaDeAnimale(&ht);

	return 0;
}