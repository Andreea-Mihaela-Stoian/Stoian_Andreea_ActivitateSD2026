#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: HashTable cu angajati
// Criteriu hash: id angajat
// Tratarea coliziunilor: chaining, adica liste simplu inlantuite

struct StructuraAngajat {
	int id;
	int vechime;
	float salariu;
	char* nume;
	char* departament;
	unsigned char cod;
};
typedef struct StructuraAngajat Angajat;

// Nod pentru lista simplu inlantuita
struct Nod {
	Angajat info;
	struct Nod* next;
};
typedef struct Nod Nod;

// HashTable = vector de liste
struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

// Citeste un angajat din fisier
Angajat citireAngajatDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	Angajat a;
	a.id = -1;
	a.vechime = 0;
	a.salariu = 0;
	a.nume = NULL;
	a.departament = NULL;
	a.cod = '-';

	if (fgets(buffer, 100, file) == NULL) {
		return a;
	}

	char* aux;

	aux = strtok(buffer, sep);
	if (aux == NULL) {
		return a;
	}

	a.id = atoi(aux);
	a.vechime = atoi(strtok(NULL, sep));
	a.salariu = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	a.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(a.nume, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	a.departament = (char*)malloc(strlen(aux) + 1);
	strcpy_s(a.departament, strlen(aux) + 1, aux);

	a.cod = *strtok(NULL, sep);

	return a;
}

// Afiseaza un angajat
void afisareAngajat(Angajat angajat) {
	printf("Id: %d\n", angajat.id);
	printf("Vechime: %d\n", angajat.vechime);
	printf("Salariu: %.2f\n", angajat.salariu);
	printf("Nume: %s\n", angajat.nume);
	printf("Departament: %s\n", angajat.departament);
	printf("Cod: %c\n\n", angajat.cod);
}