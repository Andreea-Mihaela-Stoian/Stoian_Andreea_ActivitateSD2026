#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: AVL cu studenti
// Criteriu de inserare: id-ul studentului
// Arborele se va echilibra automat dupa inserari

struct StructuraStudent {
	int id;
	int anStudiu;
	float medie;
	char* nume;
	char* specializare;
	unsigned char grupa;
};
typedef struct StructuraStudent Student;

// Structura pentru un nod din arborele AVL
struct Nod {
	Student info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

// Citeste un student din fisier
Student citireStudentDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	Student s;

	// valori default, utile daca citirea esueaza
	s.id = -1;
	s.anStudiu = 0;
	s.medie = 0;
	s.nume = NULL;
	s.specializare = NULL;
	s.grupa = '-';

	// citim o linie din fisier
	if (fgets(buffer, 100, file) == NULL) {
		return s;
	}

	char* aux;

	// citim id-ul
	aux = strtok(buffer, sep);
	if (aux == NULL) {
		return s;
	}

	s.id = atoi(aux);

	// citim anul de studiu
	s.anStudiu = atoi(strtok(NULL, sep));

	// citim media
	s.medie = atof(strtok(NULL, sep));

	// citim numele
	aux = strtok(NULL, sep);
	s.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(s.nume, strlen(aux) + 1, aux);

	// citim specializarea
	aux = strtok(NULL, sep);
	s.specializare = (char*)malloc(strlen(aux) + 1);
	strcpy_s(s.specializare, strlen(aux) + 1, aux);

	// citim grupa
	s.grupa = *strtok(NULL, sep);

	return s;
}

// Afiseaza un student
void afisareStudent(Student student) {
	printf("Id: %d\n", student.id);
	printf("An studiu: %d\n", student.anStudiu);
	printf("Medie: %.2f\n", student.medie);
	printf("Nume: %s\n", student.nume);
	printf("Specializare: %s\n", student.specializare);
	printf("Grupa: %c\n\n", student.grupa);
}

// Creeaza un student manual, fara fisier
Student initStudent(int id, int anStudiu, float medie, const char* nume, const char* specializare, unsigned char grupa) {
	Student s;

	s.id = id;
	s.anStudiu = anStudiu;
	s.medie = medie;
	s.grupa = grupa;

	s.nume = (char*)malloc(strlen(nume) + 1);
	strcpy_s(s.nume, strlen(nume) + 1, nume);

	s.specializare = (char*)malloc(strlen(specializare) + 1);
	strcpy_s(s.specializare, strlen(specializare) + 1, specializare);

	return s;
}
int calculeazaInaltimeArbore(Nod* rad) {
	// daca nodul este NULL, inaltimea este 0
	if (rad == NULL) {
		return 0;
	}

	// calculam inaltimea subarborelui stang
	int inaltimeStanga = calculeazaInaltimeArbore(rad->stanga);

	// calculam inaltimea subarborelui drept
	int inaltimeDreapta = calculeazaInaltimeArbore(rad->dreapta);

	// inaltimea arborelui este maximul dintre cele doua + radacina
	if (inaltimeStanga > inaltimeDreapta) {
		return inaltimeStanga + 1;
	}
	else {
		return inaltimeDreapta + 1;
	}
}

int calculDiferentaInaltimi(Nod* rad) {
	// diferenta = inaltime stanga - inaltime dreapta
	if (rad) {
		return calculeazaInaltimeArbore(rad->stanga) - calculeazaInaltimeArbore(rad->dreapta);
	}
	else {
		return 0;
	}
}

void rotireLaStanga(Nod** rad) {
	// rotire folosita cand arborele este dezechilibrat pe dreapta

	Nod* aux = (*rad)->dreapta;

	(*rad)->dreapta = aux->stanga;

	aux->stanga = (*rad);

	(*rad) = aux;
}

void rotireLaDreapta(Nod** rad) {
	// rotire folosita cand arborele este dezechilibrat pe stanga

	Nod* aux = (*rad)->stanga;

	(*rad)->stanga = aux->dreapta;

	aux->dreapta = (*rad);

	(*rad) = aux;
}

void adaugaStudentInArbore(Nod** rad, Student studentNou) {
	// adauga un student in arbore dupa id
	// dupa inserare, arborele este echilibrat ca AVL

	if (*rad == NULL) {
		// daca pozitia este libera, cream nod nou

		Nod* nod = (Nod*)malloc(sizeof(Nod));

		nod->info = studentNou;
		nod->stanga = NULL;
		nod->dreapta = NULL;

		*rad = nod;
	}
	else {
		// daca id-ul nou este mai mic, mergem in stanga
		if ((*rad)->info.id > studentNou.id) {
			adaugaStudentInArbore(&((*rad)->stanga), studentNou);
		}

		// daca id-ul nou este mai mare, mergem in dreapta
		if ((*rad)->info.id < studentNou.id) {
			adaugaStudentInArbore(&((*rad)->dreapta), studentNou);
		}
	}

	// verificam daca arborele s-a dezechilibrat
	int diferentaInaltimi = calculDiferentaInaltimi(*rad);

	if (diferentaInaltimi == 2) {
		// dezechilibru pe stanga

		if (calculDiferentaInaltimi((*rad)->stanga) == -1) {
			// caz stanga-dreapta
			rotireLaStanga(&(*rad)->stanga);
		}

		// caz stanga-stanga
		rotireLaDreapta(rad);
	}

	if (diferentaInaltimi == -2) {
		// dezechilibru pe dreapta

		if (calculDiferentaInaltimi((*rad)->dreapta) == 1) {
			// caz dreapta-stanga
			rotireLaDreapta(&(*rad)->dreapta);
		}

		// caz dreapta-dreapta
		rotireLaStanga(rad);
	}
}

Nod* citireArboreDeStudentiDinFisier(const char* numeFisier) {
	// citeste studentii din fisier si ii adauga in arborele AVL

	Nod* rad = NULL;

	FILE* f = fopen(numeFisier, "r");

	if (f) {
		while (!feof(f)) {
			Student s = citireStudentDinFisier(f);

			if (s.id != -1) {
				adaugaStudentInArbore(&rad, s);
			}
		}

		fclose(f);
	}
	else {
		printf("Fisierul nu a fost gasit!\n");
	}

	return rad;
}

void afisareStudentiInordine(Nod* rad) {
	// parcurgere inordine: stanga - radacina - dreapta
	// afiseaza studentii ordonati crescator dupa id

	if (rad) {
		afisareStudentiInordine(rad->stanga);
		afisareStudent(rad->info);
		afisareStudentiInordine(rad->dreapta);
	}
}

void afisarePreordine(Nod* rad) {
	// parcurgere preordine: radacina - stanga - dreapta

	if (rad) {
		afisareStudent(rad->info);
		afisarePreordine(rad->stanga);
		afisarePreordine(rad->dreapta);
	}
}
void dezalocareArboreDeStudenti(Nod** rad) {
	// dezalocam toate nodurile si campurile alocate dinamic

	if (*rad) {
		dezalocareArboreDeStudenti(&(*rad)->stanga);
		dezalocareArboreDeStudenti(&(*rad)->dreapta);

		free((*rad)->info.nume);
		free((*rad)->info.specializare);

		free(*rad);
		*rad = NULL;
	}
}

Student getStudentByID(Nod* rad, int id) {
	// cauta un student dupa id si returneaza o copie

	Student s;
	s.id = -1;
	s.anStudiu = 0;
	s.medie = 0;
	s.nume = NULL;
	s.specializare = NULL;
	s.grupa = '-';

	if (rad) {
		if (rad->info.id == id) {
			s = rad->info;

			s.nume = (char*)malloc(strlen(rad->info.nume) + 1);
			strcpy_s(s.nume, strlen(rad->info.nume) + 1, rad->info.nume);

			s.specializare = (char*)malloc(strlen(rad->info.specializare) + 1);
			strcpy_s(s.specializare, strlen(rad->info.specializare) + 1, rad->info.specializare);
		}
		else if (id < rad->info.id) {
			s = getStudentByID(rad->stanga, id);
		}
		else {
			s = getStudentByID(rad->dreapta, id);
		}
	}

	return s;
}

int determinaNumarNoduri(Nod* rad) {
	// calculeaza cate noduri sunt in arbore

	if (rad) {
		return determinaNumarNoduri(rad->stanga)
			+ determinaNumarNoduri(rad->dreapta)
			+ 1;
	}

	return 0;
}

float calculeazaSumaMediilor(Nod* rad) {
	// aduna mediile tuturor studentilor din arbore

	if (rad) {
		return rad->info.medie
			+ calculeazaSumaMediilor(rad->stanga)
			+ calculeazaSumaMediilor(rad->dreapta);
	}

	return 0;
}

float calculeazaMediaGenerala(Nod* rad) {
	// calculeaza media mediilor studentilor

	int nrStudenti = determinaNumarNoduri(rad);

	if (nrStudenti > 0) {
		return calculeazaSumaMediilor(rad) / nrStudenti;
	}

	return 0;
}

int numaraStudentiDinSpecializare(Nod* rad, const char* specializare) {
	// numara studentii care au o anumita specializare

	if (rad) {
		int nr = numaraStudentiDinSpecializare(rad->stanga, specializare)
			+ numaraStudentiDinSpecializare(rad->dreapta, specializare);

		if (strcmp(rad->info.specializare, specializare) == 0) {
			nr++;
		}

		return nr;
	}

	return 0;
}
int main() {
	Nod* rad = NULL;

	rad = citireArboreDeStudentiDinFisier("studenti_avl.txt");

	printf("Afisare inordine:\n");
	afisareStudentiInordine(rad);

	printf("Afisare preordine:\n");
	afisarePreordine(rad);

	printf("Student cautat:\n");
	Student studentCautat = getStudentByID(rad, 8);
	afisareStudent(studentCautat);

	free(studentCautat.nume);
	free(studentCautat.specializare);

	printf("Numar noduri: %d\n", determinaNumarNoduri(rad));

	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(rad));

	printf("Media generala: %.2f\n", calculeazaMediaGenerala(rad));

	printf("Numar studenti Informatica: %d\n",
		numaraStudentiDinSpecializare(rad, "Informatica"));

	dezalocareArboreDeStudenti(&rad);

	return 0;
}

