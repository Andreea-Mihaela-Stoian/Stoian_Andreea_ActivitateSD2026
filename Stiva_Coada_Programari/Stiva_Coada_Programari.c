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
void pushStack(Stiva* stiva, Programare programare) {
	// Adauga o programare in varful stivei

	Nod* nou = (Nod*)malloc(sizeof(Nod));

	nou->info = programare;

	// Noul nod pointeaza catre fostul varf
	nou->next = stiva->top;

	// Noul nod devine varful stivei
	stiva->top = nou;
}

Programare popStack(Stiva* stiva) {
	// Extrage programarea din varful stivei

	Programare p;
	p.id = -1;
	p.durataMinute = 0;
	p.cost = 0;
	p.serviciu = NULL;
	p.pacient = NULL;
	p.cod = '-';

	if (stiva->top) {
		Nod* aux = stiva->top;

		p = aux->info;

		// Mutam varful pe urmatorul nod
		stiva->top = stiva->top->next;

		// Stergem doar nodul
		free(aux);
	}

	return p;
}

int emptyStack(Stiva stiva) {
	// Returneaza 1 daca stiva este goala

	return stiva.top == NULL;
}

Stiva citireStackProgramariDinFisier(const char* numeFisier) {
	// Citeste programarile din fisier si le pune in stiva

	Stiva stiva;
	stiva.top = NULL;

	FILE* f = fopen(numeFisier, "r");

	if (f) {
		while (!feof(f)) {
			Programare p = citireProgramareDinFisier(f);

			if (p.id != -1) {
				pushStack(&stiva, p);
			}
		}

		fclose(f);
	}
	else {
		printf("Fisierul nu a fost gasit!\n");
	}

	return stiva;
}

void afisareStiva(Stiva stiva) {
	// Afiseaza stiva fara sa o modifice

	while (stiva.top) {
		afisareProgramare(stiva.top->info);
		stiva.top = stiva.top->next;
	}
}

int sizeStack(Stiva stiva) {
	// Calculeaza numarul de elemente din stiva

	int nr = 0;

	while (stiva.top) {
		nr++;
		stiva.top = stiva.top->next;
	}

	return nr;
}

void dezalocareStivaDeProgramari(Stiva* stiva) {
	// Dezaloca toate programarile din stiva

	while (stiva->top) {
		Programare p = popStack(stiva);

		free(p.serviciu);
		free(p.pacient);
	}
}
void enqueue(Coada* coada, Programare programare) {
	// Adauga o programare la finalul cozii

	Nod* nou = (Nod*)malloc(sizeof(Nod));

	nou->info = programare;
	nou->next = NULL;

	if (coada->last) {
		// Daca exista ultim nod, il legam de nodul nou
		coada->last->next = nou;

		// Noul nod devine ultimul
		coada->last = nou;
	}
	else {
		// Daca lista este goala, primul si ultimul sunt acelasi nod
		coada->first = coada->last = nou;
	}
}

Programare dequeue(Coada* coada) {
	// Extrage prima programare din coada

	Programare p;
	p.id = -1;
	p.durataMinute = 0;
	p.cost = 0;
	p.serviciu = NULL;
	p.pacient = NULL;
	p.cod = '-';

	if (coada->first) {
		Nod* aux = coada->first;

		p = aux->info;

		// Mutam inceputul cozii
		coada->first = coada->first->next;

		if (coada->first == NULL) {
			// Daca nu mai exista noduri, coada devine goala
			coada->last = NULL;
		}

		free(aux);
	}

	return p;
}

Coada citireCoadaDeProgramariDinFisier(const char* numeFisier) {
	// Citeste programarile din fisier si le pune in coada

	Coada coada;
	coada.first = NULL;
	coada.last = NULL;

	FILE* f = fopen(numeFisier, "r");

	if (f) {
		while (!feof(f)) {
			Programare p = citireProgramareDinFisier(f);

			if (p.id != -1) {
				enqueue(&coada, p);
			}
		}

		fclose(f);
	}
	else {
		printf("Fisierul nu a fost gasit!\n");
	}

	return coada;
}

void afisareCoada(Coada coada) {
	// Afiseaza coada fara sa o modifice

	while (coada.first) {
		afisareProgramare(coada.first->info);
		coada.first = coada.first->next;
	}
}

void dezalocareCoadaDeProgramari(Coada* coada) {
	// Dezaloca toate programarile din coada

	while (coada->first) {
		Programare p = dequeue(coada);

		free(p.serviciu);
		free(p.pacient);
	}
}

Programare getProgramareByID(Coada coada, int id) {
	// Cauta o programare dupa id in coada

	Programare p;
	p.id = -1;
	p.durataMinute = 0;
	p.cost = 0;
	p.serviciu = NULL;
	p.pacient = NULL;
	p.cod = '-';

	while (coada.first) {
		if (coada.first->info.id == id) {
			p = coada.first->info;

			// Copiere profunda pentru sirurile alocate dinamic
			p.serviciu = (char*)malloc(strlen(coada.first->info.serviciu) + 1);
			strcpy_s(p.serviciu, strlen(coada.first->info.serviciu) + 1, coada.first->info.serviciu);

			p.pacient = (char*)malloc(strlen(coada.first->info.pacient) + 1);
			strcpy_s(p.pacient, strlen(coada.first->info.pacient) + 1, coada.first->info.pacient);

			return p;
		}

		coada.first = coada.first->next;
	}

	return p;
}

float calculeazaCostTotal(Coada coada) {
	// Calculeaza suma costurilor din coada

	float suma = 0;

	while (coada.first) {
		suma += coada.first->info.cost;
		coada.first = coada.first->next;
	}

	return suma;
}

int main() {
	Stiva stiva = citireStackProgramariDinFisier("programari.txt");

	printf("Afisare stiva:\n");
	afisareStiva(stiva);

	printf("Numar elemente stiva: %d\n", sizeStack(stiva));

	printf("\nPop din stiva:\n");
	Programare p1 = popStack(&stiva);
	afisareProgramare(p1);

	free(p1.serviciu);
	free(p1.pacient);

	Coada coada = citireCoadaDeProgramariDinFisier("programari.txt");

	printf("\nAfisare coada:\n");
	afisareCoada(coada);

	printf("\nProgramare cautata:\n");
	Programare p2 = getProgramareByID(coada, 3);
	afisareProgramare(p2);

	free(p2.serviciu);
	free(p2.pacient);

	printf("Cost total programari: %.2f\n",
		calculeazaCostTotal(coada));

	dezalocareStivaDeProgramari(&stiva);
	dezalocareCoadaDeProgramari(&coada);

	return 0;
}