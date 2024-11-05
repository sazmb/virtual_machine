#include <stdio.h>
#include "vm.h"
#include <stdlib.h>
#include <string.h>




// Funzione per creare un nuovo nodo Stat e aggiungerlo alla lista
Stat crea_stat(int addr, int op, Value arg1, Value arg2, Value arg3) {
   Stat new_stat;
    new_stat.addr = addr;
    new_stat.op= op;
    new_stat.arg1 = arg1;
    new_stat.arg2 = arg2;
    new_stat.arg3 = arg3;
    new_stat.next = NULL;
    return new_stat;
}

// Funzione per leggere il file e creare la lista di Stat
Stat* carica_file(FILE *file) {
    Stat *head = NULL, *tail = NULL;

    int addr;
    int op;
    Value arg1 = {0}, arg2 = {0}, arg3 = {0};
    int argCount;
    Stat *code;
    int i =0;
    while (!feof(file)) {
        // Inizializza i valori di arg1, arg2 e arg3 per evitare residui dalla lettura precedente
        arg1.ival = arg2.ival = arg3.ival = 0;
        arg1.rval = arg2.rval = arg3.rval = 0;
        strcpy(arg1.sval, "");
        strcpy(arg2.sval, "");
        strcpy(arg3.sval, "");

        // Leggi l'indirizzo e l'operazione
        fscanf(file, "%d %d", &addr, &op);

            switch (op) {
                case LOCI:
                case IXAD:
                case SKIP:
                case SKPF:
                case PUSH:
                case JUMP:
                case CODE:
                case VARS:
                    fscanf(file, "%d", arg1.ival);
                break;

                case FUNC:
                fscanf(file, "%s", arg1.sval);
                break;
                case LOCS:
                case WRIT:
                    fscanf(file, "%s", arg1.sval);
                break;
                case LOCR:
                    fscanf(file, "%lf", arg1.rval);
                break;
                case NEWO:

                case INDL:
                    fscanf(file, "%d %d", arg1.ival, arg2.ival);
                break;
                case LOAD:
                case LODA:
                    fscanf(file, "%d %d", arg1.ival, arg2.ival);
                break;
                case READ:
                    fscanf(file, "%d %s", arg1.ival, arg2.sval );

                break;
                case PACK:
                    fprintf(file, "%d %d %d ", arg1.ival, arg2.ival, arg3.ival);
                break;
                break;
                default: fprintf(file, "errore lettura");
                break;
            }

        if (op==CODE) {
           code = (Stat*) malloc( sizeof(Stat) * arg1.ival);
        }
        // Crea un nuovo nodo Stat con i valori letti
        code[i]= crea_stat(addr, op, arg1, arg2, arg3);
        i++;
        }

    return code;
    }




// Funzione di esempio per stampare la lista di Stat


int main(int argc, char* argv[]) {
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

    int lenght=0;
    start_execution(prog, lenght);

    // Chiudi il file se non è stdin
    if (file != stdin) {
        fclose(file);
    }

    return 0;
}


