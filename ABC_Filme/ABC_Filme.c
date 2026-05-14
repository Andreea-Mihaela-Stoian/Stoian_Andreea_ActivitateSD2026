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
void adaugaFilmInArbore(Nod** rad, Film filmNou) {
	// adauga un film in arbore dupa id

	if (*rad == NULL) {
		Nod* nod = (Nod*)malloc(sizeof(Nod));

		nod->info = filmNou;
		nod->stanga = NULL;
		nod->dreapta = NULL;

		*rad = nod;
	}
	else {
		if ((*rad)->info.id > filmNou.id) {
			adaugaFilmInArbore(&(*rad)->stanga, filmNou);
		}

		if ((*rad)->info.id < filmNou.id) {
			adaugaFilmInArbore(&(*rad)->dreapta, filmNou);
		}
	}
}

Nod* citireArboreDeFilmeDinFisier(const char* numeFisier) {
	// citeste filmele din fisier si construieste arborele

	Nod* rad = NULL;

	FILE* f = fopen(numeFisier, "r");

	if (f) {
		while (!feof(f)) {
			Film film = citireFilmDinFisier(f);

			if (film.id != -1) {
				adaugaFilmInArbore(&rad, film);
			}
		}

		fclose(f);
	}

	return rad;
}

// Parcurgere inordine
void afisareFilmeInordine(Nod* rad) {
	if (rad) {
		afisareFilmeInordine(rad->stanga);
		afisareFilm(rad->info);
		afisareFilmeInordine(rad->dreapta);
	}
}

// Parcurgere preordine
void afisarePreordine(Nod* rad) {
	if (rad) {
		afisareFilm(rad->info);
		afisarePreordine(rad->stanga);
		afisarePreordine(rad->dreapta);
	}
}void dezalocareArboreDeFilme(Nod** rad) {
	// dezalocam toate nodurile

	if (*rad) {
		dezalocareArboreDeFilme(&(*rad)->stanga);
		dezalocareArboreDeFilme(&(*rad)->dreapta);

		free((*rad)->info.titlu);
		free((*rad)->info.regizor);

		free(*rad);

		*rad = NULL;
	}
}

Film getFilmByID(Nod* rad, int id) {
	Film film;

	film.id = -1;
	film.anLansare = 0;
	film.incasari = 0;
	film.titlu = NULL;
	film.regizor = NULL;
	film.categorie = '-';

	if (rad) {
		if (rad->info.id == id) {
			film = rad->info;

			film.titlu = (char*)malloc(strlen(rad->info.titlu) + 1);
			strcpy_s(film.titlu, strlen(rad->info.titlu) + 1, rad->info.titlu);

			film.regizor = (char*)malloc(strlen(rad->info.regizor) + 1);
			strcpy_s(film.regizor, strlen(rad->info.regizor) + 1, rad->info.regizor);
		}
		else if (id < rad->info.id) {
			film = getFilmByID(rad->stanga, id);
		}
		else {
			film = getFilmByID(rad->dreapta, id);
		}
	}

	return film;
}

int determinaNumarNoduri(Nod* rad) {
	if (rad) {
		return determinaNumarNoduri(rad->stanga)
			+ determinaNumarNoduri(rad->dreapta)
			+ 1;
	}

	return 0;
}

int maxim(int a, int b) {
	return a > b ? a : b;
}

int calculeazaInaltimeArbore(Nod* rad) {
	if (rad) {
		return maxim(
			calculeazaInaltimeArbore(rad->stanga),
			calculeazaInaltimeArbore(rad->dreapta)
		) + 1;
	}

	return 0;
}

float calculeazaIncasariTotale(Nod* rad) {
	// calculeaza incasarile tuturor filmelor

	if (rad) {
		return rad->info.incasari
			+ calculeazaIncasariTotale(rad->stanga)
			+ calculeazaIncasariTotale(rad->dreapta);
	}

	return 0;
}

int numaraFilmeRegizor(Nod* rad, const char* regizor) {
	// numara filmele unui regizor

	if (rad) {
		int nr = numaraFilmeRegizor(rad->stanga, regizor)
			+ numaraFilmeRegizor(rad->dreapta, regizor);

		if (strcmp(rad->info.regizor, regizor) == 0) {
			nr++;
		}

		return nr;
	}

	return 0;
}

int main() {
	Nod* rad = citireArboreDeFilmeDinFisier("filme.txt");

	printf("Afisare preordine:\n");
	afisarePreordine(rad);

	printf("Afisare inordine:\n");
	afisareFilmeInordine(rad);

	printf("Film cautat:\n");
	Film film = getFilmByID(rad, 8);

	afisareFilm(film);

	free(film.titlu);
	free(film.regizor);

	printf("Numar noduri: %d\n", determinaNumarNoduri(rad));

	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(rad));

	printf("Incasari totale: %.2f\n", calculeazaIncasariTotale(rad));

	printf("Filme Christopher Nolan: %d\n",
		numaraFilmeRegizor(rad, "Christopher Nolan"));

	dezalocareArboreDeFilme(&rad);

	return 0;
}