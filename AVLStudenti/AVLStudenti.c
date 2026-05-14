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