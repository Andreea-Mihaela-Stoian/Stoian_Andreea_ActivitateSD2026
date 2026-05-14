#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: stiva si coada cu programari medicale
// Stiva = LIFO: ultimul intrat, primul iesit
// Coada = FIFO: primul intrat, primul iesit

struct StructuraProgramare {
	int id;
	int durataMinute;
	float cost;
	char* serviciu;
	char* pacient;
	unsigned char cod;
};
typedef struct StructuraProgramare Programare;

// Nod folosit si pentru stiva, si pentru coada
struct Nod {
	Programare info;
	struct Nod* next;
};
typedef struct Nod Nod;

// Stiva retine doar varful
struct Stiva {
	Nod* top;
};
typedef struct Stiva Stiva;

// Coada retine primul si ultimul nod
struct Coada {
	Nod* first;
	Nod* last;
};
typedef struct Coada Coada;

// Citeste o programare din fisier
Programare citireProgramareDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	Programare p;
	p.id = -1;
	p.durataMinute = 0;
	p.cost = 0;
	p.serviciu = NULL;
	p.pacient = NULL;
	p.cod = '-';

	if (fgets(buffer, 100, file) == NULL) {
		return p;
	}

	char* aux = strtok(buffer, sep);

	if (aux == NULL) {
		return p;
	}

	p.id = atoi(aux);
	p.durataMinute = atoi(strtok(NULL, sep));
	p.cost = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	p.serviciu = (char*)malloc(strlen(aux) + 1);
	strcpy_s(p.serviciu, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	p.pacient = (char*)malloc(strlen(aux) + 1);
	strcpy_s(p.pacient, strlen(aux) + 1, aux);

	p.cod = *strtok(NULL, sep);

	return p;
}

// Afiseaza o programare
void afisareProgramare(Programare p) {
	printf("Id: %d\n", p.id);
	printf("Durata minute: %d\n", p.durataMinute);
	printf("Cost: %.2f\n", p.cost);
	printf("Serviciu: %s\n", p.serviciu);
	printf("Pacient: %s\n", p.pacient);
	printf("Cod: %c\n\n", p.cod);
}