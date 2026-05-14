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
void afisareListaHoteluri(Nod* cap) {
	while (cap) {
		afisareHotel(cap->info);
		cap = cap->next;
	}
}

void adaugaHotelInLista(Nod** cap, Hotel hotelNou) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));

	nou->info = hotelNou;
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		Nod* aux = *cap;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;
	}
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;

	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);

	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}

	return ht;
}

int calculeazaHash(int id, int dimensiune) {
	return id % dimensiune;
}

void inserareHotelInTabela(HashTable ht, Hotel hotel) {
	int pozitie = calculeazaHash(hotel.id, ht.dim);

	adaugaHotelInLista(&(ht.vector[pozitie]), hotel);
}

HashTable citireHoteluriDinFisier(const char* numeFisier) {
	HashTable ht = initializareHashTable(5);

	FILE* f = fopen(numeFisier, "r");

	if (f) {
		while (!feof(f)) {
			Hotel h = citireHotelDinFisier(f);

			if (h.id != -1) {
				inserareHotelInTabela(ht, h);
			}
		}

		fclose(f);
	}
	else {
		printf("Fisierul nu a fost gasit!\n");
	}

	return ht;
}

void afisareTabelaDeHoteluri(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("\nCluster %d:\n", i);
		afisareListaHoteluri(ht.vector[i]);
	}
}
void dezalocareTabelaDeHoteluri(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		Nod* cap = ht->vector[i];

		while (cap) {
			Nod* aux = cap;
			cap = cap->next;

			free(aux->info.nume);
			free(aux->info.oras);
			free(aux);
		}
	}

	free(ht->vector);

	ht->vector = NULL;
	ht->dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	float* vectorMedii = (float*)malloc(sizeof(float) * ht.dim);

	*nrClustere = 0;

	for (int i = 0; i < ht.dim; i++) {
		float suma = 0;
		int nr = 0;

		Nod* cap = ht.vector[i];

		while (cap) {
			suma += cap->info.pretNoapte;
			nr++;
			cap = cap->next;
		}

		if (nr > 0) {
			vectorMedii[*nrClustere] = suma / nr;
			(*nrClustere)++;
		}
	}

	return vectorMedii;
}

Hotel getHotelDupaID(HashTable ht, int idCautat) {
	Hotel h;

	h.id = -1;
	h.nrStele = 0;
	h.pretNoapte = 0;
	h.nume = NULL;
	h.oras = NULL;
	h.cod = '-';

	int pozitie = calculeazaHash(idCautat, ht.dim);

	Nod* cap = ht.vector[pozitie];

	while (cap) {
		if (cap->info.id == idCautat) {
			h = cap->info;

			h.nume = (char*)malloc(strlen(cap->info.nume) + 1);
			strcpy_s(h.nume, strlen(cap->info.nume) + 1, cap->info.nume);

			h.oras = (char*)malloc(strlen(cap->info.oras) + 1);
			strcpy_s(h.oras, strlen(cap->info.oras) + 1, cap->info.oras);

			return h;
		}

		cap = cap->next;
	}

	return h;
}

int numaraHoteluriDinOras(HashTable ht, const char* oras) {
	int nr = 0;

	for (int i = 0; i < ht.dim; i++) {
		Nod* cap = ht.vector[i];

		while (cap) {
			if (strcmp(cap->info.oras, oras) == 0) {
				nr++;
			}

			cap = cap->next;
		}
	}

	return nr;
}

int main() {
	HashTable ht = citireHoteluriDinFisier("hoteluri.txt");

	printf("Tabela hash:\n");
	afisareTabelaDeHoteluri(ht);

	printf("\nHotel cautat:\n");
	Hotel h = getHotelDupaID(ht, 103);
	afisareHotel(h);

	free(h.nume);
	free(h.oras);

	int nrClustere = 0;
	float* medii = calculeazaPreturiMediiPerClustere(ht, &nrClustere);

	printf("\nPreturi medii per cluster:\n");

	for (int i = 0; i < nrClustere; i++) {
		printf("%.2f\n", medii[i]);
	}

	free(medii);

	printf("\nHoteluri din Brasov: %d\n",
		numaraHoteluriDinOras(ht, "Brasov"));

	dezalocareTabelaDeHoteluri(&ht);

	return 0;
}