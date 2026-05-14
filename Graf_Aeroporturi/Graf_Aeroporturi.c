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
// Inserare nod principal
void inserareListaPrincipala(NodPrincipal** cap, Aeroport aeroport) {
	NodPrincipal* nou =
		(NodPrincipal*)malloc(sizeof(NodPrincipal));

	nou->info = aeroport;
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

// Inserare nod secundar
void inserareListaSecundara(
	NodSecundar** cap,
	NodPrincipal* info) {

	NodSecundar* nou =
		(NodSecundar*)malloc(sizeof(NodSecundar));

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

// Cautare dupa id
NodPrincipal* cautareNodDupaId(
	NodPrincipal* graf,
	int id) {

	while (graf != NULL &&
		graf->info.id != id) {

		graf = graf->next;
	}

	return graf;
}

// Adauga muchie intre aeroporturi
void adaugaMuchie(
	NodPrincipal* graf,
	int id1,
	int id2) {

	NodPrincipal* nod1 =
		cautareNodDupaId(graf, id1);

	NodPrincipal* nod2 =
		cautareNodDupaId(graf, id2);

	if (nod1 && nod2) {
		inserareListaSecundara(
			&nod1->vecini,
			nod2);

		inserareListaSecundara(
			&nod2->vecini,
			nod1);
	}
}

// Afisare graf
void afisareGraf(NodPrincipal* graf) {
	while (graf) {

		afisareAeroport(graf->info);

		printf("\nVecini:");

		NodSecundar* vecini = graf->vecini;

		while (vecini) {
			afisareAeroport(vecini->info->info);

			vecini = vecini->next;
		}

		printf("\n");

		graf = graf->next;
	}
}
// DFS folosind stiva
void parcurgereInAdancime(
	NodPrincipal* graf,
	int idStart,
	int dim) {

	int* vizitat =
		(int*)malloc(sizeof(int) * dim);

	for (int i = 0; i < dim; i++) {
		vizitat[i] = 0;
	}

	ListaDubla stiva;
	stiva.first = NULL;
	stiva.last = NULL;

	push(&stiva, idStart);

	vizitat[idStart - 1] = 1;

	while (stiva.first != NULL) {

		int idExtras = pop(&stiva);

		NodPrincipal* extras =
			cautareNodDupaId(graf, idExtras);

		if (extras) {

			afisareAeroport(extras->info);

			NodSecundar* vecini =
				extras->vecini;

			while (vecini) {

				int idVecin =
					vecini->info->info.id;

				if (vizitat[idVecin - 1] == 0) {

					push(&stiva, idVecin);

					vizitat[idVecin - 1] = 1;
				}

				vecini = vecini->next;
			}
		}
	}

	free(vizitat);
}

// Dezalocare graf
void dezalocareGraf(NodPrincipal** graf) {

	while (*graf) {

		NodPrincipal* aux = *graf;

		NodSecundar* vecin = aux->vecini;

		while (vecin) {

			NodSecundar* temp = vecin;

			vecin = vecin->next;

			free(temp);
		}

		free(aux->info.nume);

		*graf = (*graf)->next;

		free(aux);
	}
}

int main() {

	NodPrincipal* graf = NULL;

	inserareListaPrincipala(
		&graf,
		initAeroport(1, "Otopeni", 14.7));

	inserareListaPrincipala(
		&graf,
		initAeroport(2, "Heathrow", 61.6));

	inserareListaPrincipala(
		&graf,
		initAeroport(3, "CharlesDeGaulle", 57.4));

	inserareListaPrincipala(
		&graf,
		initAeroport(4, "Fiumicino", 29.3));

	inserareListaPrincipala(
		&graf,
		initAeroport(5, "Schiphol", 52.5));

	inserareListaPrincipala(
		&graf,
		initAeroport(6, "Barajas", 50.6));

	adaugaMuchie(graf, 1, 2);
	adaugaMuchie(graf, 1, 3);
	adaugaMuchie(graf, 2, 5);
	adaugaMuchie(graf, 3, 4);
	adaugaMuchie(graf, 4, 6);
	adaugaMuchie(graf, 5, 6);

	printf("Afisare graf:\n");
	afisareGraf(graf);

	printf("\nDFS:\n");

	parcurgereInAdancime(graf, 1, 6);

	dezalocareGraf(&graf);

	return 0;
}