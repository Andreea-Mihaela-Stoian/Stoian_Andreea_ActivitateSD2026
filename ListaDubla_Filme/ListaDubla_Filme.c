#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structura pentru un film
typedef struct {
	int* cod;
	char* titlu;
	float rating;
	float durata;
} film;

// Nod pentru lista dublu inlantuita
typedef struct nodld {
	film* inf;
	struct nodld* next;
	struct nodld* prev;
} nodld;


// Inserare la final in lista
nodld* inserare(nodld* cap, nodld** coada, film* f) {

	// cream nod nou
	nodld* nou = (nodld*)malloc(sizeof(nodld));

	// alocam memorie pentru film
	nou->inf = (film*)malloc(sizeof(film));

	// copiem codul
	nou->inf->cod = (int*)malloc(sizeof(int));
	*(nou->inf->cod) = *(f->cod);

	// copiem titlul
	nou->inf->titlu =
		(char*)malloc((strlen(f->titlu) + 1) * sizeof(char));

	strcpy(nou->inf->titlu, f->titlu);

	// copiem valorile simple
	nou->inf->rating = f->rating;
	nou->inf->durata = f->durata;

	// initializam legaturile
	nou->next = NULL;
	nou->prev = NULL;

	// daca lista este goala
	if (cap == NULL) {

		cap = nou;
		*coada = nou;
	}
	else {

		// adaugam la final
		(*coada)->next = nou;

		nou->prev = *coada;

		*coada = nou;
	}

	return cap;
}


// Traversare normala
void traversare(nodld* cap) {

	nodld* temp = cap;

	while (temp != NULL) {

		printf("\nCod=%d, Titlu=%s, Rating=%5.2f, Durata=%5.2f",
			*(temp->inf->cod),
			temp->inf->titlu,
			temp->inf->rating,
			temp->inf->durata);

		temp = temp->next;
	}
}


// Traversare inversa
void traversareInvers(nodld* coada) {

	nodld* temp = coada;

	while (temp != NULL) {

		printf("\nCod=%d, Titlu=%s, Rating=%5.2f, Durata=%5.2f",
			*(temp->inf->cod),
			temp->inf->titlu,
			temp->inf->rating,
			temp->inf->durata);

		temp = temp->prev;
	}
}