#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void* concatena_stringa(void* buffer, int l1,  void* aggiunta, int l2) {

    // Calcoliamo la nuova lunghezza necessaria
    size_t lunghezza_iniziale = l1;
    size_t nuova_lunghezza = lunghezza_iniziale + l2 ; // +1 per il terminatore '\0'
    void* new_buffer= malloc (nuova_lunghezza);
    // Riallociamo la memoria per il buffer

    memcpy(new_buffer, buffer, nuova_lunghezza);
    if (new_buffer == NULL) {
        perror("Errore nella riallocazione della memoria");
        exit(EXIT_FAILURE);
    }

    // Se il buffer è stato appena creato (era NULL), dobbiamo assicurarci che contenga una stringa vuota
    /**  if (lunghezza_iniziale == 0) {
      *     buffer[0] = '\0'; // Impostiamo il terminatore di stringa iniziale
      * }
     */


    memcpy(&new_buffer[l1-1], aggiunta ,l2);

    return new_buffer;


}
int main(void)
{
    printf("Hello, World!\n");

    char*  format="{i,s,s}";
    char* stringa="{10, serpente, cane, 12.01 }";
    void* val=malloc(100);

   int total_length=0;

    for (int i=0;i<strlen(format); i++) {
        if (format[i]=='{' || format[i]=='}' || format[i]=='['
        || format[i]==']' ){
            stringa++; // Consuma anche il newline);
        }
        else if (format[i]=='i') {
            int temp=0;
            sscanf(stringa, "%d",&temp );
            if (val==NULL) {
                val=malloc(sizeof(int));
                memcpy(val, &temp, 4);
                total_length+=sizeof(int);
            }
            else {
                concatena_stringa(val, total_length, &temp, sizeof(int) );
            }
            stringa = strchr(stringa, ' ') + 1;
        }
        else if (format[i]=='r') {
            double temp=0;
            sscanf(stringa, "%lf",&temp );
            if (val==NULL) {
                val=malloc(sizeof(double));
                memcpy(val, &temp, 8);
                total_length+=sizeof(double);
            }
            else {
                concatena_stringa(val, total_length, &temp, sizeof(double) );
            }stringa = strchr(stringa, ' ') + 1;
        }
        else if (format[i]=='s') {
            char temp[100];
            sscanf(stringa, "%s",temp);
            if (val==NULL) {
                val=malloc(strlen(temp));
                memcpy(val, &temp, strlen(temp));
                total_length+=strlen(temp);
            }
            else {
                concatena_stringa(val, total_length, temp, strlen(temp) );
            }stringa = strchr(stringa, ' ') + 1;
        }
    }
//manca la funzione di write cosi faccio un po fatica a leggere

}
