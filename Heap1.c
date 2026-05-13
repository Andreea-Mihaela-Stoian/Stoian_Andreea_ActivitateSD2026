#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct StructuraTren {
	int id;
	int nrVagoane;
	float pretBilet;
	char* locomotiva;
	char* numeTren;
	unsigned char serie;
};
typedef struct StructuraTren Tren;
