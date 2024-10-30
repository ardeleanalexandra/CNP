#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUCKETS 997
#define NUM_PERS 1000000

// Definire nume
const char *nume_f[] = { "Adelina", "Adina", "Ana", "Andra", "Aurora", "Bianca", "Camelia", "Carina", "Crina", "Carmen", "Cristina", "Claudia", "Daria", "Diana", "Daniela", "Elena", "Eliza", "Ema", "Emilia", "Gabriela", "Georgiana", "Gina", "Ioana", "Iulia", "Izabela", "Iris", "Laura", "Lavinia", "Larisa", "Lidia", "Luiza",  "Madalina",  "Mara",  "Maria",  "Melania",  "Mihaela",  "Mirela",  "Monica",  "Mariana",  "Marina",  "Nadia",  "Nicoleta",  "Nina",  "Oana",  "Otilia",  "Olivia",  "Paula",  "Raluca",  "Ramona",  "Rodica",  "Roxana",  "Ruxandra",  "Sabina",  "Silvia",  "Stefania",  "Teodora",  "Valentina",  "Violeta",  "Tamara",  "Zoe" };
const char *nume_m[] = { "Adelin", "Anton", "Alexandru", "Andrei", "Bogdan", "Adrian", "Catalin", "Cristian", "Cosmin", "Costin", "Daniel", "Claudiu", "David", "Dragos", "Eduard", "Emilian", "Emanuel", "Florin", "Felix", "Gabriel", "George", "Iulian", "Ivan", "Laurentiu", "Liviu", "Lucian", "Madalin", "Marius", "Octavian", "Ovidiu",  "Paul",  "Pavel",  "Raul",  "Robert",  "Dorin",  "Sabin",  "Sebastian",  "Stefan",  "Sorin",  "Teodor",  "Valentin",  "Victor",  "Vlad",  "Cezar",  "Doru",  "Flaviu",  "Eugen",  "Grigore",  "Horatiu",  "Horia",  "Iacob",  "Iustin",  "Leonard",  "Marcel",  "Nelu",  "Rares",  "Serban",  "Sergiu",  "Tudor" };
const char *nume_fam[] = { "Abaza", "Adamescu", "Adoc", "Albu", "Baciu", "Badea", "Barbu", "Candea", "Caragiu", "Cernea", "Chitu", "Conea", "Danciu", "Deac", "Diaconu", "Doinas", "Enache", "Ene", "Erbiceanu", "Filimon", "Florea", "Frosin", "Fulga", "Ganea", "Georgescu", "Ghinea", "Goga", "Hasdeu", "Herlea", "Hoban", "Iacobescu",  "Ionescu",  "Irimia",  "Josan",  "Kiazim",  "Lambru",  "Lascu",  "Lipa",  "Lucan", "Lungu",  "Lupu",  "Manea",  "Manolescu",  "Marinescu",  "Mugur",  "Neagu",  "Nechita",  "Negrescu",  "Nita", "Oancea",  "Olaru",  "Onciu",  "Pascu",  "Parvu",  "Radulescu",  "Stan",  "Tamas",  "Tudoran" };

// Structura de date pentru persoana
typedef struct {
    char cnp[14];
    char nume[100];
} date_personale;

date_personale persoane[NUM_PERS];

typedef struct Node {
    date_personale date;
    struct Node *next;
} Node;

Node* hash_table[BUCKETS] = {NULL};

// Generarea unui CNP aleatoriu
void generare_cnp(char *cnp) {
    int s = (rand() % 2) + 1; // generează 1 sau 2
    int aa = rand() % 100;
    int ll = rand() % 12 + 1;
    int zz = rand() % 31 + 1;
    int jj = rand() % 48 + 1;
    int nnn = rand() % 1000;
    int c = rand() % 10;

    sprintf(cnp, "%d%02d%02d%02d%02d%03d%d", s, aa, ll, zz, jj, nnn, c);
}

// Generarea unui nume aleatoriu pe baza CNP-ului
void generare_nume(char *nume, char *cnp) {
    int sex = cnp[0] - '0';
    const char *prenume1, *prenume2, *nume_familie;

    if (sex == 1) { // Masculin
        prenume1 = nume_m[rand() % 60];
        prenume2 = nume_m[rand() % 60];
    } else if (sex == 2) { // Feminin
        prenume1 = nume_f[rand() % 60];
        prenume2 = nume_f[rand() % 60];
    }

    nume_familie = nume_fam[rand() % 60];

    sprintf(nume, "%s %s %s", prenume1, prenume2, nume_familie);
}

// Functie de hash
int hashFunction(char *cnp) {
    unsigned long long int index = strtoull(cnp, NULL, 10);
    return index % BUCKETS;
}

// Inserare în tabelul hash
void insertItem(date_personale date) {
    int index = hashFunction(date.cnp);
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->date = date;
    newNode->next = hash_table[index];
    hash_table[index] = newNode;
}

// Funcție de căutare în tabelul hash
int searchItem(date_personale date) {
    int index = hashFunction(date.cnp);
    Node *current = hash_table[index];
    int iteratii = 1;

    while (current != NULL) {
        if (strcmp(current->date.cnp, date.cnp) == 0) {
            return iteratii;
        }
        current = current->next;
        iteratii++;
    }
    return iteratii;
}

// Crearea unei persoane aleatorii
date_personale creeare_persoana() {
    date_personale persoana;
    generare_cnp(persoana.cnp);
    generare_nume(persoana.nume, persoana.cnp);
    return persoana;
}

int main() {
    srand(time(NULL));

    printf("Generare persoane...\n");
    for (int i = 0; i < NUM_PERS; i++) {
        persoane[i] = creeare_persoana();
    }

    printf("Construire tabel hash...\n");
    for (int i = 0; i < NUM_PERS; i++) {
        insertItem(persoane[i]);
    }

    printf("Căutare în hash table...\n");
    FILE *fout = fopen("result.txt", "w");
    FILE *fout_stats = fopen("statistici.txt", "w");

    int total_iter = 0;
    int og_iter = 0;
    for (int i = 0; i < 1000; i++) {
        int ind = rand() % NUM_PERS;
        date_personale pers = persoane[ind];
        int iter = searchItem(pers);
        total_iter += iter;
        fprintf(fout, "%s, %s - pozitie originala: %d, hash table: %d iteratii.\n", pers.cnp, pers.nume, ind, iter);
        og_iter += ind;
    }

    fprintf(fout_stats, "Pentru cautarea a 1000 de persoane:\n");
    fprintf(fout_stats, "total iteratii in tabela hash: %d\n", total_iter);
    fprintf(fout_stats, "total iteratii in structura originala: %d\n", og_iter);
    fprintf(fout_stats, "medie iteratii in tabela hash: %d\n", total_iter / 1000);
    fprintf(fout_stats, "medie iteratii in structura originala: %d\n", og_iter / 1000);
    float y = (100.0 * (og_iter - total_iter)) / og_iter;
    fprintf(fout_stats, "Rezultat: cu %.2f%% mai putine iteratii.\n", y);

    fclose(fout);
    fclose(fout_stats);

    return 0;
}
