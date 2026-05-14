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