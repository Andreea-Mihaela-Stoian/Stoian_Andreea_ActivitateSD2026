#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: Arbore binar de cautare cu filme
// Criteriu de inserare: id-ul filmului

struct StructuraFilm {
	int id;
	int anLansare;
	float incasari;
	char* titlu;
	char* regizor;
	unsigned char categorie;
};
typedef struct StructuraFilm Film;

// Structura pentru nodul arborelui
struct Nod {
	Film info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

// Citeste un film din fisier
Film citireFilmDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	Film f;

	f.id = -1;
	f.anLansare = 0;
	f.incasari = 0;
	f.titlu = NULL;
	f.regizor = NULL;
	f.categorie = '-';

	if (fgets(buffer, 100, file) == NULL) {
		return f;
	}

	char* aux;

	aux = strtok(buffer, sep);

	if (aux == NULL) {
		return f;
	}

	f.id = atoi(aux);

	f.anLansare = atoi(strtok(NULL, sep));

	f.incasari = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	f.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy_s(f.titlu, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	f.regizor = (char*)malloc(strlen(aux) + 1);
	strcpy_s(f.regizor, strlen(aux) + 1, aux);

	f.categorie = *strtok(NULL, sep);

	return f;
}

// Afiseaza un film
void afisareFilm(Film film) {
	printf("Id: %d\n", film.id);
	printf("An lansare: %d\n", film.anLansare);
	printf("Incasari: %.2f\n", film.incasari);
	printf("Titlu: %s\n", film.titlu);
	printf("Regizor: %s\n", film.regizor);
	printf("Categorie: %c\n\n", film.categorie);
}