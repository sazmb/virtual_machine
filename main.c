#include <stdio.h>
#include "vm.h"
#include <stdlib.h>
#include <string.h>

void print_stat_to_file(int i, Stat stat) {
    printf("%d\t", i);
    printf("%d\t", stat.op);
    switch (stat.op) {
        case LOCI:
        case IXAD:
        case SKIP:
        case SKPF:
        case PUSH:
        case JUMP:
        case FUNC:
        case CODE:
        case VARS:
            printf("%d\n", stat.arg1.ival);
            break;
        case LOCS:
        case WRIT:
            printf("%s\n", stat.arg1.sval);
            break;
        case LOCR:
            printf("%lf\n", stat.arg1.rval);
            break;
        case NEWO:
        case LOAD:
        case LODA:
        case INDL:
            printf("%d\t", stat.arg1.ival);
            printf("%d\n", stat.arg2.ival);
            break;
        case READ:
            printf("%d\t", stat.arg1.ival);
            printf("%s\n", stat.arg2.sval);
            break;
        case PACK:
            printf("%d\t", stat.arg1.ival);
            printf("%d\t", stat.arg2.ival);
            printf("%d\n", stat.arg3.ival);
            break;
        default: printf("\n");
            break;
    }
}


// Funzione per creare un nuovo nodo Stat e aggiungerlo alla lista
Stat crea_stat(int addr, int op, Value arg1, Value arg2, Value arg3) {
    Stat new_stat;
    new_stat.addr = addr;
    new_stat.op = op;
    new_stat.arg1 = arg1;
    new_stat.arg2 = arg2;
    new_stat.arg3 = arg3;
    return new_stat;
}

// Funzione per leggere il file e creare la lista di Stat
Stat *carica_file(FILE *file) {
    char c;
    int addr;
    int op;
    char str[100];
    Value arg1, arg2, arg3;
    Stat *code = NULL;
    int i = 0;
    int j = 0;


    // Leggi il file riga per riga fino alla fine
    while (!feof(file)) {
        // Inizializza i valori di arg1, arg2 e arg3 per evitare residui dalla lettura precedente
        arg1.ival = arg2.ival = arg3.ival = 0;
        arg1.rval = arg2.rval = arg3.rval = 0;
        //arg1.sval = arg2.sval = arg3.sval = "";
        str[0] = "";



        // Leggi l'indirizzo e l'operazione
        fscanf(file, "%d\t%d\t", &addr, &op);

        switch (op) {
            case LOCI:
            case IXAD:
            case SKIP:
            case SKPF:
            case PUSH:
            case JUMP:
            case CODE:
            case VARS:
            case FUNC:
                fscanf(file, "%d ", &arg1.ival);
                break;

            case LOCS:
            case WRIT:
                j = 0;
                while ((c = fgetc(file)) != '\n' && c != EOF && j < sizeof(str) - 1) {
                    str[j++] = c; // Memorizza il carattere nel buffer
                }
                str[j] = '\0';
                break;

            case LOCR:
                fscanf(file, "%lf ", &arg1.rval);
                break;
            case NEWO:
            case INDL:
                fscanf(file, "%d\t%d ", &arg1.ival, &arg2.ival);
                break;
            case LOAD:
            case LODA:
                fscanf(file, "%d\t%d ", &arg1.ival, &arg2.ival);
                break;
            case READ:
                fscanf(file, "%d\t", &arg1.ival);
                j = 0;
                while ((c = fgetc(file)) != '\n' && c != EOF && j < sizeof(str) - 1) {
                    str[j++] = c; // Memorizza il carattere nel buffer
                }
                str[j] = '\0';

                break;
            case PACK:
                fscanf(file, "%d\t%d\t%d ", &arg1.ival, &arg2.ival, &arg3.ival);
                break;
            default:
                break;
        }

        if (op == CODE) {
            code = (Stat *) malloc(sizeof(Stat) * (arg1.ival + 1));
        }
        // Crea un nuovo nodo Stat con i valori letti
        if (op == LOCS || op == WRIT) {
            arg1.sval = malloc(sizeof(char) * (j + 1));
            arg1.sval = strcpy(arg1.sval, str);
        }
        if (op == READ) {
            arg2.sval = malloc(sizeof(char) * (j + 1));
            arg2.sval = strcpy(arg2.sval, str);
        }

        code[i] = crea_stat(addr, op, arg1, arg2, arg3);
        i++;
    }

    return code;
}


// Funzione di esempio per stampare la lista di Stat


int main(int argc, char *argv[]) {
    FILE *file;
    if (argc > 1) {
        file = fopen(argv[1], "r");
        if (!file) {
            perror("Errore nell'apertura del file");
            return EXIT_FAILURE;
        }
    } else {
        file = stdin;
    }

    // Carica le istruzioni dal file
    Stat *prog = carica_file(file);

    /*
    for (int i=0; i<prog[0].arg1.ival+1; i++ ) {
        print_stat_to_file(i , prog[i]);
    }
  */

    start_execution(prog);

    // Chiudi il file se non è stdin
    if (file != stdin) {
        fclose(file);
    }

    return 0;
}
