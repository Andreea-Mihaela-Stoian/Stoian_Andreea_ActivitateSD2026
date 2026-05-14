#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exercitiu: MAX-HEAP cu studenti
// Criteriu: media studentului
// Studentul cu media cea mai mare va fi primul in heap

struct StructuraStudent {
	int id;
	int anStudiu;
	float medie;
	char* nume;
	char* specializare;
	unsigned char grupa;
};
typedef struct StructuraStudent Student;

struct Heap {
	int lungime;
	Student* studenti;
	int nrElemente;
};
typedef struct Heap Heap;

// Citeste un student din fisier
Student citireStudentDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);

	char* aux;
	Student s;

	// citire id
	aux = strtok(buffer, sep);
	s.id = atoi(aux);

	// citire an studiu
	s.anStudiu = atoi(strtok(NULL, sep));

	// citire medie
	s.medie = atof(strtok(NULL, sep));

	// citire nume
	aux = strtok(NULL, sep);
	s.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(s.nume, strlen(aux) + 1, aux);

	// citire specializare
	aux = strtok(NULL, sep);
	s.specializare = (char*)malloc(strlen(aux) + 1);
	strcpy_s(s.specializare, strlen(aux) + 1, aux);

	// citire grupa
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