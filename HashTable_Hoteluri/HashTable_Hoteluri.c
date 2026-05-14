#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: HashTable cu hoteluri
// Criteriu hash: id hotel
// Coliziuni: chaining

struct StructuraHotel {
	int id;
	int nrStele;
	float pretNoapte;
	char* nume;
	char* oras;
	unsigned char cod;
};
typedef struct StructuraHotel Hotel;

struct Nod {
	Hotel info;
	struct Nod* next;
};
typedef struct Nod Nod;

struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

Hotel citireHotelDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	Hotel h;
	h.id = -1;
	h.nrStele = 0;
	h.pretNoapte = 0;
	h.nume = NULL;
	h.oras = NULL;
	h.cod = '-';

	if (fgets(buffer, 100, file) == NULL) {
		return h;
	}

	char* aux = strtok(buffer, sep);

	if (aux == NULL) {
		return h;
	}

	h.id = atoi(aux);
	h.nrStele = atoi(strtok(NULL, sep));
	h.pretNoapte = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	h.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(h.nume, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	h.oras = (char*)malloc(strlen(aux) + 1);
	strcpy_s(h.oras, strlen(aux) + 1, aux);

	h.cod = *strtok(NULL, sep);

	return h;
}

void afisareHotel(Hotel hotel) {
	printf("Id: %d\n", hotel.id);
	printf("Nr stele: %d\n", hotel.nrStele);
	printf("Pret/noapte: %.2f\n", hotel.pretNoapte);
	printf("Nume: %s\n", hotel.nume);
	printf("Oras: %s\n", hotel.oras);
	printf("Cod: %c\n\n", hotel.cod);
}