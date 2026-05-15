#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structura pentru un abonament fitness
typedef struct {
	int* cod;
	char* numeClient;
	float pret;
	float durataLuni;
} abonament;

// Nod pentru lista dublu inlantuita
typedef struct nodld {
	abonament* inf;
	struct nodld* next;
	struct nodld* prev;
} nodld;

// Inserare la final in lista
nodld* inserare(nodld* cap, nodld** coada, abonament* a) {
	nodld* nou = (nodld*)malloc(sizeof(nodld));

	// alocam memorie pentru informatia din nod
	nou->inf = (abonament*)malloc(sizeof(abonament));

	// copiem codul
	nou->inf->cod = (int*)malloc(sizeof(int));
	*(nou->inf->cod) = *(a->cod);

	// copiem numele clientului
	nou->inf->numeClient = (char*)malloc((strlen(a->numeClient) + 1) * sizeof(char));
	strcpy(nou->inf->numeClient, a->numeClient);

	// copiem valorile simple
	nou->inf->pret = a->pret;
	nou->inf->durataLuni = a->durataLuni;

	// initial nodul nu este legat de alte noduri
	nou->next = NULL;
	nou->prev = NULL;

	// daca lista este goala
	if (cap == NULL) {
		cap = nou;
		*coada = nou;
	}
	else {
		// legam nodul nou la final
		(*coada)->next = nou;
		nou->prev = *coada;
		*coada = nou;
	}

	return cap;
}
// Traversare de la inceput spre final
void traversare(nodld* cap) {
	nodld* temp = cap;

	while (temp != NULL) {
		printf("\nCod=%d, Client=%s, Pret=%5.2f, Durata=%5.2f luni",
			*(temp->inf->cod),
			temp->inf->numeClient,
			temp->inf->pret,
			temp->inf->durataLuni);

		temp = temp->next;
	}
}

// Traversare inversa, de la final spre inceput
void traversareInvers(nodld* coada) {
	nodld* temp = coada;

	while (temp != NULL) {
		printf("\nCod=%d, Client=%s, Pret=%5.2f, Durata=%5.2f luni",
			*(temp->inf->cod),
			temp->inf->numeClient,
			temp->inf->pret,
			temp->inf->durataLuni);

		temp = temp->prev;
	}
}

// Conversie lista dubla in vector
void conversieListaVector(nodld* cap, abonament** vect, int* nr) {
	nodld* temp = cap;

	while (temp != NULL) {
		// alocam element nou in vector
		vect[*nr] = (abonament*)malloc(sizeof(abonament));

		// copiem codul
		vect[*nr]->cod = (int*)malloc(sizeof(int));
		*(vect[*nr]->cod) = *(temp->inf->cod);

		// copiem numele clientului
		vect[*nr]->numeClient = (char*)malloc((strlen(temp->inf->numeClient) + 1) * sizeof(char));
		strcpy(vect[*nr]->numeClient, temp->inf->numeClient);

		// copiem campurile simple
		vect[*nr]->pret = temp->inf->pret;
		vect[*nr]->durataLuni = temp->inf->durataLuni;

		(*nr)++;
		temp = temp->next;
	}
}
// Dezalocare lista dubla
void dezalocare(nodld* cap) {
	nodld* temp = cap;

	while (temp != NULL) {
		nodld* temp2 = temp->next;

		// eliberam campurile alocate dinamic
		free(temp->inf->numeClient);
		free(temp->inf->cod);

		// eliberam obiectul si nodul
		free(temp->inf);
		free(temp);

		temp = temp2;
	}
}

int main() {
	int n;

	printf("Nr abonamente=");
	scanf("%d", &n);

	nodld* cap = NULL;
	nodld* coada = NULL;

	abonament* a;
	char buffer[100];

	for (int i = 0; i < n; i++) {
		a = (abonament*)malloc(sizeof(abonament));

		a->cod = (int*)malloc(sizeof(int));

		printf("Cod=");
		scanf("%d", a->cod);

		printf("Nume client=");
		scanf("%s", buffer);

		a->numeClient = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(a->numeClient, buffer);

		printf("Pret=");
		scanf("%f", &a->pret);

		printf("Durata luni=");
		scanf("%f", &a->durataLuni);

		// inseram abonamentul in lista
		cap = inserare(cap, &coada, a);

		// dezalocam obiectul temporar
		free(a->numeClient);
		free(a->cod);
		free(a);
	}

	printf("\n--- Traversare normala ---");
	traversare(cap);

	printf("\n--- Traversare inversa ---");
	traversareInvers(coada);

	printf("\n------- Conversie in vector -----------");

	abonament** vect = (abonament**)malloc(n * sizeof(abonament*));
	int nr = 0;

	conversieListaVector(cap, vect, &nr);

	for (int i = 0; i < nr; i++) {
		printf("\nCod=%d, Client=%s, Pret=%5.2f, Durata=%5.2f luni",
			*(vect[i]->cod),
			vect[i]->numeClient,
			vect[i]->pret,
			vect[i]->durataLuni);
	}

	// dezalocare vector
	for (int i = 0; i < nr; i++) {
		free(vect[i]->numeClient);
		free(vect[i]->cod);
		free(vect[i]);
	}

	free(vect);

	// dezalocare lista
	dezalocare(cap);

	return 0;
}