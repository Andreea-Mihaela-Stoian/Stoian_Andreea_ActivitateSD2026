#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: Graf neorientat cu orase
// Nodurile principale sunt orasele
// Muchiile reprezinta drumuri intre orase

typedef struct Oras Oras;
typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

// Nod pentru lista dubla folosita ca stiva
struct Nod {
	int ID;
	Nod* next;
	Nod* prev;
};

// Lista dubla
struct ListaDubla {
	Nod* first;
	Nod* last;
};

// Structura pentru oras
struct Oras {
	int id;
	char* nume;
	float populatie;
};

// Nod principal din graf
struct NodPrincipal {
	Oras info;
	NodPrincipal* next;
	NodSecundar* vecini;
};

// Nod secundar pentru lista de vecini
struct NodSecundar {
	NodPrincipal* info;
	NodSecundar* next;
};

// Adauga un id la inceputul listei
// O folosim pentru stiva
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

// Extrage primul element din lista
int pop(ListaDubla* ld) {
	int ID = -1;

	if (ld->first) {
		ID = ld->first->ID;

		Nod* aux = ld->first;
		ld->first = ld->first->next;

		if (ld->first == NULL) {
			ld->last = NULL;
		}
		else {
			ld->first->prev = NULL;
		}

		free(aux);
	}

	return ID;
}

// Creeaza un oras
Oras initOras(int id, const char* nume, float populatie) {
	Oras oras;

	oras.id = id;
	oras.populatie = populatie;

	oras.nume = (char*)malloc(strlen(nume) + 1);
	strcpy_s(oras.nume, strlen(nume) + 1, nume);

	return oras;
}

// Afiseaza un oras
void afisareOras(Oras oras) {
	printf("\n%d. Oras: %s, populatie: %.2f milioane",
		oras.id, oras.nume, oras.populatie);
}
// Insereaza un oras in lista principala a grafului
void inserareListaPrincipala(NodPrincipal** cap, Oras oras) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));

	nou->info = oras;
	nou->next = NULL;
	nou->vecini = NULL;

	if (*cap) {
		NodPrincipal* aux = *cap;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;
	}
	else {
		*cap = nou;
	}
}

// Insereaza un vecin in lista secundara
void inserareListaSecundara(NodSecundar** cap, NodPrincipal* info) {
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));

	nou->info = info;
	nou->next = NULL;

	if (*cap) {
		NodSecundar* aux = *cap;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;
	}
	else {
		*cap = nou;
	}
}

// Cauta un oras dupa id
NodPrincipal* cautareNodDupaId(NodPrincipal* graf, int id) {
	while (graf != NULL && graf->info.id != id) {
		graf = graf->next;
	}

	return graf;
}

// Adauga muchie intre doua orase
// Graful este neorientat, deci adaugam legatura in ambele sensuri
void adaugaMuchie(NodPrincipal* graf, int id1, int id2) {
	NodPrincipal* nod1 = cautareNodDupaId(graf, id1);
	NodPrincipal* nod2 = cautareNodDupaId(graf, id2);

	if (nod1 != NULL && nod2 != NULL) {
		inserareListaSecundara(&nod1->vecini, nod2);
		inserareListaSecundara(&nod2->vecini, nod1);
	}
}

// Afiseaza graful cu lista de vecini
void afisareGraf(NodPrincipal* graf) {
	while (graf != NULL) {
		afisareOras(graf->info);

		printf("\n Vecini:");

		NodSecundar* vecini = graf->vecini;

		while (vecini) {
			afisareOras(vecini->info->info);
			vecini = vecini->next;
		}

		printf("\n");
		graf = graf->next;
	}
}