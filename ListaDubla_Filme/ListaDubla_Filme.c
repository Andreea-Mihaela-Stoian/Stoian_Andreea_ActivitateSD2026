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
// Conversie lista in vector
void conversieListaVector(
	nodld* cap,
	film** vect,
	int* nr) {

	nodld* temp = cap;

	while (temp != NULL) {

		// alocam element nou in vector
		vect[*nr] = (film*)malloc(sizeof(film));

		// copiem codul
		vect[*nr]->cod =
			(int*)malloc(sizeof(int));

		*(vect[*nr]->cod) =
			*(temp->inf->cod);

		// copiem titlul
		vect[*nr]->titlu =
			(char*)malloc(
				(strlen(temp->inf->titlu) + 1)
				* sizeof(char));

		strcpy(
			vect[*nr]->titlu,
			temp->inf->titlu);

		// copiem valorile simple
		vect[*nr]->rating =
			temp->inf->rating;

		vect[*nr]->durata =
			temp->inf->durata;

		(*nr)++;

		temp = temp->next;
	}
}


// Dezalocare lista
void dezalocare(nodld* cap) {

	nodld* temp = cap;

	while (temp != NULL) {

		nodld* temp2 = temp->next;

		free(temp->inf->titlu);

		free(temp->inf->cod);

		free(temp->inf);

		free(temp);

		temp = temp2;
	}
}


int main() {

	int n;

	printf("Nr filme=");
	scanf("%d", &n);

	nodld* cap = NULL;
	nodld* coada = NULL;

	film* f;

	char buffer[100];

	for (int i = 0; i < n; i++) {

		f = (film*)malloc(sizeof(film));

		f->cod = (int*)malloc(sizeof(int));

		printf("\nCod=");
		scanf("%d", f->cod);

		printf("Titlu=");
		scanf("%s", buffer);

		f->titlu =
			(char*)malloc(
				(strlen(buffer) + 1)
				* sizeof(char));

		strcpy(f->titlu, buffer);

		printf("Rating=");
		scanf("%f", &f->rating);

		printf("Durata=");
		scanf("%f", &f->durata);

		// inseram filmul in lista
		cap = inserare(cap, &coada, f);

		// dezalocam obiectul temporar
		free(f->titlu);

		free(f->cod);

		free(f);
	}

	printf("\n--- Traversare normala ---");
	traversare(cap);

	printf("\n--- Traversare inversa ---");
	traversareInvers(coada);

	printf("\n------- Conversie in vector -----------");

	film** vect =
		(film**)malloc(n * sizeof(film*));

	int nr = 0;

	conversieListaVector(cap, vect, &nr);

	for (int i = 0; i < nr; i++) {

		printf("\nCod=%d, Titlu=%s, Rating=%5.2f, Durata=%5.2f",
			*(vect[i]->cod),
			vect[i]->titlu,
			vect[i]->rating,
			vect[i]->durata);
	}

	// dezalocare vector
	for (int i = 0; i < nr; i++) {

		free(vect[i]->titlu);

		free(vect[i]->cod);

		free(vect[i]);
	}

	free(vect);

	// dezalocare lista
	dezalocare(cap);

	return 0;
}