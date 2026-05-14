#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: stiva si coada cu comenzi
// Stiva functioneaza LIFO: ultimul intrat este primul scos
// Coada functioneaza FIFO: primul intrat este primul scos

struct StructuraComanda {
	int id;
	int nrProduse;
	float valoare;
	char* produs;
	char* client;
	unsigned char cod;
};
typedef struct StructuraComanda Comanda;

// Nodul este folosit atat pentru stiva, cat si pentru coada
struct Nod {
	Comanda info;
	struct Nod* next;
};
typedef struct Nod Nod;

// Structura pentru stiva
// Retinem doar varful stivei
struct Stiva {
	Nod* top;
};
typedef struct Stiva Stiva;

// Structura pentru coada
// Retinem primul si ultimul element
struct Coada {
	Nod* first;
	Nod* last;
};
typedef struct Coada Coada;

// Citeste o comanda din fisier
Comanda citireComandaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	// Initializam o comanda invalida
	// O folosim daca citirea nu reuseste
	Comanda c;
	c.id = -1;
	c.nrProduse = 0;
	c.valoare = 0;
	c.produs = NULL;
	c.client = NULL;
	c.cod = '-';

	// Citim o linie din fisier
	if (fgets(buffer, 100, file) == NULL) {
		return c;
	}

	// Spargem linia dupa virgula
	char* aux = strtok(buffer, sep);

	// Daca linia este goala, returnam comanda invalida
	if (aux == NULL) {
		return c;
	}

	// Citim id-ul
	c.id = atoi(aux);

	// Citim numarul de produse
	c.nrProduse = atoi(strtok(NULL, sep));

	// Citim valoarea comenzii
	c.valoare = atof(strtok(NULL, sep));

	// Citim numele produsului
	aux = strtok(NULL, sep);
	c.produs = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.produs, strlen(aux) + 1, aux);

	// Citim numele clientului
	aux = strtok(NULL, sep);
	c.client = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.client, strlen(aux) + 1, aux);

	// Citim codul
	c.cod = *strtok(NULL, sep);

	return c;
}

// Afiseaza o comanda
void afisareComanda(Comanda comanda) {
	printf("Id: %d\n", comanda.id);
	printf("Nr produse: %d\n", comanda.nrProduse);
	printf("Valoare: %.2f\n", comanda.valoare);
	printf("Produs: %s\n", comanda.produs);
	printf("Client: %s\n", comanda.client);
	printf("Cod: %c\n\n", comanda.cod);
}
void pushStack(Stiva* stiva, Comanda comanda) {
	// Adauga o comanda in varful stivei

	// Cream un nod nou
	Nod* nou = (Nod*)malloc(sizeof(Nod));

	// Punem comanda in nod
	nou->info = comanda;

	// Noul nod pointeaza catre fostul varf
	nou->next = stiva->top;

	// Noul nod devine varful stivei
	stiva->top = nou;
}

Comanda popStack(Stiva* stiva) {
	// Extrage comanda din varful stivei

	Comanda c;
	c.id = -1;
	c.nrProduse = 0;
	c.valoare = 0;
	c.produs = NULL;
	c.client = NULL;
	c.cod = '-';

	// Verificam daca stiva nu este goala
	if (stiva->top) {
		// Salvam nodul din varf
		Nod* aux = stiva->top;

		// Salvam informatia din nod
		c = aux->info;

		// Mutam varful pe urmatorul nod
		stiva->top = stiva->top->next;

		// Stergem doar nodul
		// Informatia va fi returnata si dezalocata ulterior
		free(aux);
	}

	return c;
}

int emptyStack(Stiva stiva) {
	// Returneaza 1 daca stiva este goala
	// Returneaza 0 daca exista elemente

	return stiva.top == NULL;
}

Stiva citireStackComenziDinFisier(const char* numeFisier) {
	// Citeste comenzile din fisier si le pune in stiva

	Stiva stiva;
	stiva.top = NULL;

	FILE* f = fopen(numeFisier, "r");

	if (f) {
		while (!feof(f)) {
			// Citim o comanda
			Comanda c = citireComandaDinFisier(f);

			// Daca citirea a fost valida, o adaugam in stiva
			if (c.id != -1) {
				pushStack(&stiva, c);
			}
		}

		fclose(f);
	}
	else {
		printf("Fisierul nu a fost gasit!\n");
	}

	return stiva;
}

void afisareStiva(Stiva stiva) {
	// Afiseaza stiva fara sa o modifice
	// Parametrul este transmis prin valoare, deci lucram pe o copie

	while (stiva.top) {
		afisareComanda(stiva.top->info);
		stiva.top = stiva.top->next;
	}
}

int sizeStack(Stiva stiva) {
	// Calculeaza numarul de elemente din stiva

	int nr = 0;

	while (stiva.top) {
		nr++;
		stiva.top = stiva.top->next;
	}

	return nr;
}

void dezalocareStivaDeComenzi(Stiva* stiva) {
	// Dezaloca toate comenzile din stiva

	while (stiva->top) {
		// Scoatem o comanda din stiva
		Comanda c = popStack(stiva);

		// Dezalocam campurile alocate dinamic
		free(c.produs);
		free(c.client);
	}
}