#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: MAX-HEAP cu studenti
// Criteriu: media studentului
// Studentul cu media cea mai mare va fi primul in heap

struct StructuraStudent {
	int id;
	int anStudiu;
	float medie;
	char* nume;
	char* specializare;
	unsigned char grupa;
};
typedef struct StructuraStudent Student;

struct Heap {
	int lungime;
	Student* studenti;
	int nrElemente;
};
typedef struct Heap Heap;

// Citeste un student din fisier
Student citireStudentDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);

	char* aux;
	Student s;

	// citire id
	aux = strtok(buffer, sep);
	s.id = atoi(aux);

	// citire an studiu
	s.anStudiu = atoi(strtok(NULL, sep));

	// citire medie
	s.medie = atof(strtok(NULL, sep));

	// citire nume
	aux = strtok(NULL, sep);
	s.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(s.nume, strlen(aux) + 1, aux);

	// citire specializare
	aux = strtok(NULL, sep);
	s.specializare = (char*)malloc(strlen(aux) + 1);
	strcpy_s(s.specializare, strlen(aux) + 1, aux);

	// citire grupa
	s.grupa = *strtok(NULL, sep);

	return s;
}

// Afiseaza un student
void afisareStudent(Student student) {
	printf("Id: %d\n", student.id);
	printf("An studiu: %d\n", student.anStudiu);
	printf("Medie: %.2f\n", student.medie);
	printf("Nume: %s\n", student.nume);
	printf("Specializare: %s\n", student.specializare);
	printf("Grupa: %c\n\n", student.grupa);
}
Heap initializareHeap(int lungime) {
	// initializam un heap gol, dar cu spatiu alocat

	Heap heap;

	// capacitatea maxima a heap-ului
	heap.lungime = lungime;

	// vectorul in care stocam studentii
	heap.studenti = (Student*)malloc(sizeof(Student) * lungime);

	// initial nu avem studenti in heap
	heap.nrElemente = 0;

	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	// functia reface proprietatea de MAX-HEAP
	// criteriul folosit este media studentului

	int pozStanga = 2 * pozitieNod + 1;
	int pozDreapta = 2 * pozitieNod + 2;

	// presupunem ca nodul curent are media cea mai mare
	int pozMax = pozitieNod;

	// verificam fiul stang
	if (pozStanga < heap.nrElemente &&
		heap.studenti[pozMax].medie < heap.studenti[pozStanga].medie) {
		pozMax = pozStanga;
	}

	// verificam fiul drept
	if (pozDreapta < heap.nrElemente &&
		heap.studenti[pozMax].medie < heap.studenti[pozDreapta].medie) {
		pozMax = pozDreapta;
	}

	// daca un fiu are media mai mare, facem interschimbarea
	if (pozMax != pozitieNod) {
		Student aux = heap.studenti[pozMax];
		heap.studenti[pozMax] = heap.studenti[pozitieNod];
		heap.studenti[pozitieNod] = aux;

		// continuam filtrarea pe pozitia unde a ajuns nodul mutat
		filtreazaHeap(heap, pozMax);
	}
}

Heap citireHeapDeStudentiDinFisier(const char* numeFisier) {
	// citim studentii din fisier
	// apoi transformam vectorul intr-un MAX-HEAP dupa medie

	Heap heap = initializareHeap(10);

	FILE* f = fopen(numeFisier, "r");

	if (f) {
		while (!feof(f)) {
			heap.studenti[heap.nrElemente++] = citireStudentDinFisier(f);
		}

		fclose(f);
	}

	// filtram de la ultimul parinte pana la radacina
	for (int i = (heap.nrElemente - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}

	return heap;
}