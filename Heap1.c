#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Structura pentru un produs
struct StructuraProdus {

	int id;
	int stoc;
	float pret;
	char* nume;
	char* categorie;
	unsigned char cod;
};

typedef struct StructuraProdus Produs;

// Structura Heap
struct Heap {
	int lungime;
	Produs* produse;
	int nrElemente;
};

typedef struct Heap Heap;


// Functie pentru citirea unui produs din fisier
Produs citireProdusDinFisier(FILE* file) {

	// buffer folosit pentru citirea unui rand
	char buffer[100];

	// separatorii folositi in fisier
	char sep[3] = ",\n";

	// citim o linie din fisier
	fgets(buffer, 100, file);

	char* aux;

	// cream produsul
	Produs p;

	// citim id-ul
	aux = strtok(buffer, sep);
	p.id = atoi(aux);

	// citim stocul
	p.stoc = atoi(strtok(NULL, sep));

	// citim pretul
	p.pret = atof(strtok(NULL, sep));

	// citim numele produsului
	aux = strtok(NULL, sep);

	// alocam memorie pentru sir
	p.nume = (char*)malloc(strlen(aux) + 1);

	// copiem textul
	strcpy_s(p.nume, strlen(aux) + 1, aux);

	// citim categoria
	aux = strtok(NULL, sep);

	// alocare memorie
	p.categorie = (char*)malloc(strlen(aux) + 1);

	// copiere categorie
	strcpy_s(p.categorie, strlen(aux) + 1, aux);

	// citim codul
	p.cod = *strtok(NULL, sep);

	// returnam produsul
	return p;
}


// Functie pentru afisarea unui produs
void afisareProdus(Produs produs) {

	printf("Id: %d\n", produs.id);

	printf("Stoc: %d\n", produs.stoc);

	printf("Pret: %.2f\n", produs.pret);

	printf("Nume: %s\n", produs.nume);

	printf("Categorie: %s\n", produs.categorie);

	printf("Cod: %c\n\n", produs.cod);
}