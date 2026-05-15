#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: MIN-HEAP cu evenimente
// Criteriu: pretul biletului
// Evenimentul cu pretul cel mai mic va fi in radacina heap-ului

struct StructuraEveniment {
	int id;
	int durataMinute;
	float pretBilet;
	char* nume;
	char* organizator;
	unsigned char cod;
};
typedef struct StructuraEveniment Eveniment;

// Structura Heap
// Contine lungimea maxima, vectorul si numarul de elemente existente
struct Heap {
	int lungime;
	Eveniment* evenimente;
	int nrElemente;
};
typedef struct Heap Heap;

// Citeste un eveniment din fisier
Eveniment citireEvenimentDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);

	char* aux;

	Eveniment e;

	aux = strtok(buffer, sep);
	e.id = atoi(aux);

	e.durataMinute = atoi(strtok(NULL, sep));

	e.pretBilet = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	e.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(e.nume, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	e.organizator = (char*)malloc(strlen(aux) + 1);
	strcpy_s(e.organizator, strlen(aux) + 1, aux);

	e.cod = *strtok(NULL, sep);

	return e;
}

// Afiseaza un eveniment
void afisareEveniment(Eveniment eveniment) {
	printf("Id: %d\n", eveniment.id);
	printf("Durata minute: %d\n", eveniment.durataMinute);
	printf("Pret bilet: %.2f\n", eveniment.pretBilet);
	printf("Nume: %s\n", eveniment.nume);
	printf("Organizator: %s\n", eveniment.organizator);
	printf("Cod: %c\n\n", eveniment.cod);
}

// Initializeaza heap-ul
Heap initializareHeap(int lungime) {
	Heap heap;

	heap.lungime = lungime;

	heap.evenimente = (Eveniment*)malloc(sizeof(Eveniment) * lungime);

	heap.nrElemente = 0;

	return heap;
}

// Reface proprietatea de MIN-HEAP
void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozStanga = 2 * pozitieNod + 1;
	int pozDreapta = 2 * pozitieNod + 2;

	int pozMin = pozitieNod;

	// verificam fiul stang
	if (pozStanga < heap.nrElemente &&
		heap.evenimente[pozMin].pretBilet > heap.evenimente[pozStanga].pretBilet) {
		pozMin = pozStanga;
	}

	// verificam fiul drept
	if (pozDreapta < heap.nrElemente &&
		heap.evenimente[pozMin].pretBilet > heap.evenimente[pozDreapta].pretBilet) {
		pozMin = pozDreapta;
	}

	// daca am gasit un copil cu pret mai mic, interschimbam
	if (pozMin != pozitieNod) {
		Eveniment aux = heap.evenimente[pozMin];
		heap.evenimente[pozMin] = heap.evenimente[pozitieNod];
		heap.evenimente[pozitieNod] = aux;

		filtreazaHeap(heap, pozMin);
	}
}

// Citeste evenimentele din fisier si construieste heap-ul
Heap citireHeapDeEvenimenteDinFisier(const char* numeFisier) {
	Heap heap = initializareHeap(10);

	FILE* f = fopen(numeFisier, "r");

	if (f) {
		while (!feof(f)) {
			heap.evenimente[heap.nrElemente++] =
				citireEvenimentDinFisier(f);
		}

		fclose(f);
	}

	// filtram de la ultimul parinte pana la radacina
	for (int i = (heap.nrElemente - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}

	return heap;
}

// Afiseaza elementele vizibile din heap
void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemente; i++) {
		afisareEveniment(heap.evenimente[i]);
	}
}

// Extrage evenimentul din radacina
Eveniment extrageEveniment(Heap* heap) {
	Eveniment eveniment;

	eveniment.id = -1;
	eveniment.durataMinute = 0;
	eveniment.pretBilet = 0;
	eveniment.nume = NULL;
	eveniment.organizator = NULL;
	eveniment.cod = '-';

	if (heap->nrElemente > 0) {
		eveniment = heap->evenimente[0];

		// mutam ultimul element pe prima pozitie
		heap->evenimente[0] =
			heap->evenimente[heap->nrElemente - 1];

		// punem elementul extras la final
		heap->evenimente[heap->nrElemente - 1] =
			eveniment;

		heap->nrElemente--;

		// refacem heap-ul
		for (int i = (heap->nrElemente - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);
		}
	}

	return eveniment;
}

// Dezaloca heap-ul
void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->evenimente[i].nume);
		free(heap->evenimente[i].organizator);
	}

	free(heap->evenimente);

	heap->evenimente = NULL;
	heap->lungime = 0;
	heap->nrElemente = 0;
}

int main() {
	Heap heap = citireHeapDeEvenimenteDinFisier("evenimente.txt");

	printf("Heap initial:\n");
	afisareHeap(heap);

	printf("Eveniment extras:\n");
	afisareEveniment(extrageEveniment(&heap));

	printf("Heap ramas:\n");
	afisareHeap(heap);

	dezalocareHeap(&heap);

	return 0;
}