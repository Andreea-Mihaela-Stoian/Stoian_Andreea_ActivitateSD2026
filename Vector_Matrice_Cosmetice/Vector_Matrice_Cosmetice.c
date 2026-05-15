#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

// Structura pentru un produs cosmetic
typedef struct {
	int* cod;
	char* denumire;
	float pret;
	float gramaj;
} cosmetic;

// Citire vector de produse cosmetice
void citireVectorCosmetice(cosmetic* vc, int nr) {
	char buffer[50];
	// citim codul, denumirea, pretul si gramajul pentru fiecare produs cosmetic
	for (int i = 0; i < nr; i++) {
		printf("\nCod=");
		vc[i].cod = (int*)malloc(sizeof(int)); // alocam memorie pentru cod
		scanf("%d", vc[i].cod); 

		printf("Denumire=");
		scanf("%s", buffer); 
		vc[i].denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char)); 
		strcpy(vc[i].denumire, buffer);

		printf("Pret=");
		scanf("%f", &vc[i].pret);

		printf("Gramaj=");
		scanf("%f", &vc[i].gramaj);
	}
}

// Afisare vector
void afisareVectorCosmetice(cosmetic* vc, int nr) {
	for (int i = 0; i < nr; i++) {
		printf("\nCod=%d, Denumire=%s, Pret=%5.2f, Gramaj=%5.2f",
			*(vc[i].cod),
			vc[i].denumire,
			vc[i].pret,
			vc[i].gramaj);
	}
}

// Dezalocare vector
void dezalocareVectorCosmetice(cosmetic* vc, int nr) {
	for (int i = 0; i < nr; i++) {
		free(vc[i].cod);
		free(vc[i].denumire);
	}

	free(vc);
}

// Citire matrice
// mat[i][0] = cod
// mat[i][1] = pret
// mat[i][2] = gramaj
// denumiri[i] = denumirea produsului
void citireMatrice(float** mat, char** denumiri, int nr) {
	char buffer[50];

	for (int i = 0; i < nr; i++) {
		printf("\nCod=");
		scanf("%f", &mat[i][0]);

		printf("Denumire=");
		scanf("%s", buffer);

		denumiri[i] = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(denumiri[i], buffer);

		printf("Pret=");
		scanf("%f", &mat[i][1]);

		printf("Gramaj=");
		scanf("%f", &mat[i][2]);
	}
}

// Afisare matrice
void afisareMatrice(float** mat, char** denumiri, int nr) {
	for (int i = 0; i < nr; i++) {
		printf("\nCod=%5.2f, Denumire=%s, Pret=%5.2f, Gramaj=%5.2f",
			mat[i][0],
			denumiri[i],
			mat[i][1],
			mat[i][2]);
	}
}

// Dezalocare matrice
void dezalocareMatrice(float** mat, char** denumiri, int nr) {
	for (int i = 0; i < nr; i++) {
		free(mat[i]);
		free(denumiri[i]);
	}

	free(mat);
	free(denumiri);
}

int main() {
	int nrProduse;

	printf("Nr produse cosmetice=");
	scanf("%d", &nrProduse);

	// VARIANTA 1: vector de structuri
	cosmetic* vectorCosmetice = (cosmetic*)malloc(nrProduse * sizeof(cosmetic));

	printf("\nCitire vector cosmetice:\n");
	citireVectorCosmetice(vectorCosmetice, nrProduse);

	printf("\nAfisare vector cosmetice:\n");
	afisareVectorCosmetice(vectorCosmetice, nrProduse);

	dezalocareVectorCosmetice(vectorCosmetice, nrProduse);

	// VARIANTA 2: matrice + vector de denumiri

	float** mat = (float**)malloc(nrProduse * sizeof(float*));

	for (int i = 0; i < nrProduse; i++) {
		mat[i] = (float*)malloc(3 * sizeof(float));
	}

	char** denumiri = (char**)malloc(nrProduse * sizeof(char*));

	printf("\n\nCitire matrice cosmetice:\n");
	citireMatrice(mat, denumiri, nrProduse);

	printf("\nAfisare matrice cosmetice:\n");
	afisareMatrice(mat, denumiri, nrProduse);

	dezalocareMatrice(mat, denumiri, nrProduse);

	return 0;
}