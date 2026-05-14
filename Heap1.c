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
Heap initializareHeap(int lungime) {
	// cream un heap gol, dar cu spatiu pentru mai multe produse
	Heap heap;

	// lungimea maxima a vectorului
	heap.lungime = lungime;

	// alocam memorie pentru vectorul de produse
	heap.produse = (Produs*)malloc(sizeof(Produs) * lungime);

	// la inceput nu avem produse in heap
	heap.nrElemente = 0;

	return heap;
}
void filtreazaHeap(Heap heap, int pozitieNod) {
	// functia reface proprietatea de MIN-HEAP
	// criteriul folosit este pretul produsului
	// produsul cu pretul cel mai mic trebuie sa fie mai sus in heap

	int pozStanga = 2 * pozitieNod + 1;
	int pozDreapta = 2 * pozitieNod + 2;

	// presupunem initial ca nodul curent are pretul minim
	int pozMin = pozitieNod;

	// verificam daca fiul stang exista si are pret mai mic
	if (pozStanga < heap.nrElemente &&
		heap.produse[pozMin].pret > heap.produse[pozStanga].pret) {
		pozMin = pozStanga;
	}

	// verificam daca fiul drept exista si are pret mai mic
	if (pozDreapta < heap.nrElemente &&
		heap.produse[pozMin].pret > heap.produse[pozDreapta].pret) {
		pozMin = pozDreapta;
	}

	// daca unul dintre fii are pret mai mic, interschimbam produsele
	if (pozMin != pozitieNod) {
		Produs aux = heap.produse[pozMin];
		heap.produse[pozMin] = heap.produse[pozitieNod];
		heap.produse[pozitieNod] = aux;

		// continuam filtrarea pe pozitia unde a ajuns produsul mutat
		filtreazaHeap(heap, pozMin);
	}
}
void filtreazaHeap(Heap heap, int pozitieNod) {
	// functia reface proprietatea de MIN-HEAP
	// criteriul folosit este pretul produsului
	// produsul cu pretul cel mai mic trebuie sa fie mai sus in heap

	int pozStanga = 2 * pozitieNod + 1;
	int pozDreapta = 2 * pozitieNod + 2;

	// presupunem initial ca nodul curent are pretul minim
	int pozMin = pozitieNod;

	// verificam daca fiul stang exista si are pret mai mic
	if (pozStanga < heap.nrElemente &&
		heap.produse[pozMin].pret > heap.produse[pozStanga].pret) {
		pozMin = pozStanga;
	}

	// verificam daca fiul drept exista si are pret mai mic
	if (pozDreapta < heap.nrElemente &&
		heap.produse[pozMin].pret > heap.produse[pozDreapta].pret) {
		pozMin = pozDreapta;
	}

	// daca unul dintre fii are pret mai mic, interschimbam produsele
	if (pozMin != pozitieNod) {
		Produs aux = heap.produse[pozMin];
		heap.produse[pozMin] = heap.produse[pozitieNod];
		heap.produse[pozitieNod] = aux;

		// continuam filtrarea pe pozitia unde a ajuns produsul mutat
		filtreazaHeap(heap, pozMin);
	}
}
Heap citireHeapDeProduseDinFisier(const char* numeFisier) {
	// citim produsele din fisier si le punem intr-un vector
	// apoi transformam vectorul intr-un MIN-HEAP dupa pret

	Heap heap = initializareHeap(10);

	FILE* f = fopen(numeFisier, "r");

	if (f) {
		// citim cat timp mai exista linii in fisier
		while (!feof(f)) {
			heap.produse[heap.nrElemente++] = citireProdusDinFisier(f);
		}

		fclose(f);
	}

	// incepem filtrarea de la ultimul parinte pana la radacina
	for (int i = (heap.nrElemente - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}

	return heap;
}