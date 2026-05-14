#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: Arbore binar de cautare cu carti
// Criteriu de inserare: id-ul cartii

struct StructuraCarte {
	int id;
	int anPublicare;
	float pret;
	char* titlu;
	char* autor;
	unsigned char codRaft;
};
typedef struct StructuraCarte Carte;

// Nod pentru arbore binar de cautare
struct Nod {
	Carte info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

// Citeste o carte din fisier
Carte citireCarteDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	Carte c;
	c.id = -1;
	c.anPublicare = 0;
	c.pret = 0;
	c.titlu = NULL;
	c.autor = NULL;
	c.codRaft = '-';

	if (fgets(buffer, 100, file) == NULL) {
		return c;
	}

	char* aux;

	aux = strtok(buffer, sep);
	if (aux == NULL) {
		return c;
	}

	c.id = atoi(aux);
	c.anPublicare = atoi(strtok(NULL, sep));
	c.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	c.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.titlu, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	c.autor = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.autor, strlen(aux) + 1, aux);

	c.codRaft = *strtok(NULL, sep);

	return c;
}

// Afiseaza o carte
void afisareCarte(Carte carte) {
	printf("Id: %d\n", carte.id);
	printf("An publicare: %d\n", carte.anPublicare);
	printf("Pret: %.2f\n", carte.pret);
	printf("Titlu: %s\n", carte.titlu);
	printf("Autor: %s\n", carte.autor);
	printf("Cod raft: %c\n\n", carte.codRaft);
}