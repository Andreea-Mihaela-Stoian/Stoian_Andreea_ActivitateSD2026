#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Aeroport Aeroport;
typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

// Nod pentru stiva
struct Nod {
	int ID;
	Nod* next;
	Nod* prev;
};

// Lista dubla folosita ca stiva
struct ListaDubla {
	Nod* first;
	Nod* last;
};

// Structura aeroport
struct Aeroport {
	int id;
	char* nume;
	float traficMilioane;
};

// Nod principal din graf
struct NodPrincipal {
	Aeroport info;
	NodPrincipal* next;
	NodSecundar* vecini;
};

// Nod secundar pentru lista de vecini
struct NodSecundar {
	NodPrincipal* info;
	NodSecundar* next;
};

// Adauga element in stiva
void push(ListaDubla* ld, int ID) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));

	nou->ID = ID;
	nou->next = ld->first;
	nou->prev = NULL;

	if (ld->first == NULL) {
		ld->first = ld->last = nou;
	}
	else {
		ld->first->prev = nou;
		ld->first = nou;
	}
}

// Extrage element din stiva
int pop(ListaDubla* ld) {
	int ID = -1;

	if (ld->first) {
		ID = ld->first->ID;

		Nod* aux = ld->first;

		ld->first = ld->first->next;

		if (ld->first == NULL) {
			ld->last = NULL;
		}

		free(aux);
	}

	return ID;
}

// Initializare aeroport
Aeroport initAeroport(int id, const char* nume, float traficMilioane) {
	Aeroport a;

	a.id = id;
	a.traficMilioane = traficMilioane;

	a.nume = (char*)malloc(strlen(nume) + 1);
	strcpy_s(a.nume, strlen(nume) + 1, nume);

	return a;
}

// Afisare aeroport
void afisareAeroport(Aeroport aeroport) {
	printf("\n%d. Aeroport: %s | Trafic: %.2f milioane",
		aeroport.id,
		aeroport.nume,
		aeroport.traficMilioane);
}