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