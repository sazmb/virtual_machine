#include "vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_OBJECTS 1000000
int const SIZE_INT = 4;
int const SIZE_REAL = 8;
int const SIZE_STRING = 8;
Pstat prog;
int pc;
Object *vars;
Object ostack[MAX_OBJECTS];
Activation_record astack[MAX_OBJECTS];
void *istack;
int current_assign_var[100];
int vp = 0;
int op = 0;
int ip;
int ap = 0;
int n_var = 0;
char *test_pointer;

void error(char *msg) {
    printf("%s", msg);
    exit(EXIT_FAILURE);
}

void *void_concat(void *buffer, int l1, void *aggiunta, int l2) {
    // Calcoliamo la nuova lunghezza necessaria
    size_t lunghezza_iniziale = l1;
    size_t nuova_lunghezza = lunghezza_iniziale + l2;
    void *new_buffer = malloc(nuova_lunghezza);
    // Riallociamo la memoria per il buffer

    memcpy(new_buffer, buffer, lunghezza_iniziale);
    if (new_buffer == NULL) {
        perror("Errore nella riallocazione della memoria");
        exit(EXIT_FAILURE);
    }

    memcpy(&new_buffer[l1], aggiunta, l2);

    return new_buffer;
}

char *estrai_sottostringa(const char *sorgente, int inizio, int fine) {
    // Calcola la lunghezza della sottostringa
    int lunghezza = fine - inizio + 1;

    // Alloca memoria per la sottostringa (+1 per il terminatore '\0')
    char *sottostringa = (char *) malloc(lunghezza + 1);
    if (sottostringa == NULL) {
        perror("Errore di allocazione memoria");
        return NULL;
    }

    // Copia la sottostringa nella nuova memoria
    memcpy(sottostringa, sorgente + inizio, lunghezza);

    // Aggiunge il terminatore alla fine
    sottostringa[lunghezza] = '\0';

    return sottostringa;
}

void *reverse_memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *) dest;
    const unsigned char *s = (const unsigned char *) src;

    // Inizia la copia dall'ultimo byte
    for (size_t i = 0; i < n; i++) {
        d[n - 1 - i] = s[n - 1 - i];
    }

    return dest;
}

void move_mem(void *start, int offset) {
    reverse_memcpy(start + offset, start, &istack[ip] - start);
    ip += offset;
}

void reassign_addr(int offset, int is_local) {
    if (is_local) {
        for (int i = current_assign_var[vp] + 1; i < astack[ap - 1].param_num; i++)
            ostack[op - i - 1].addr += offset;
    } else {
        for (int i = current_assign_var[vp] + 1; i < n_var; i++)
            vars[i].addr = (void *) ((char *) vars[i].addr + offset);
    }
}

void gestisci_resize(int size, int num, Object obj, void *val) {
    if (obj.addr == NULL)
        // caso array non ancora assegnato
        obj.addr = istack;

    int offset = (num - obj.num) * size;
    move_mem(obj.addr, offset);
    // passo l'indice della pila degli activation record per sapere se
    // è un ambiente globale se ap=0
    // oppure un ambiente locale se ap!=0
    reassign_addr(offset, ap);
    //vp viene aggiornato ogni volta che un indirizzo viene caricato sulla pila(LODA)
    // tiene traccia dell'ultimo indirizzo caricato(funziona anche per funzioni)
    // nel caso di ambienti locali vp rappresenta l'indice di ostack di dove è il
    // corrispettivo param
    // non gestisce il caso vengano caricati due load consecutivi senza stor
    if (!ap) {
        int index = current_assign_var[vp];
        vars[index].num = num;
        vars[index].size = size;
        vars[index].addr = obj.addr;
    } else {
        int index = current_assign_var[vp];
        obj.size = ostack[index].size;
        obj.num = ostack[index].num;
        obj.addr = ostack[index].addr;
    }
    memcpy(obj.addr, val, size * num);
}

void read_simple(int oid, char *format) {
    if (strcmp(format, "i") == 0) {
        int i;
        scanf("%d", &i);
        *(int *) vars[oid].addr = i;
    } else if (strcmp(format, "r") == 0) {
        double r;
        scanf("%lf", &r);
        *(double *) vars[oid].addr = r;
    } else if (strcmp(format, "s") == 0) {
        char *s = malloc(100 * sizeof(char));
        scanf("%s", s);
        *(char **) vars[oid].addr = s;
    } else if (strcmp(format, "b") == 0) {
        char *s = malloc(100 * sizeof(char));
        scanf("%s", s);
        int value = 0;
        if (strcmp(s, "false") == 0) value = 0;
        else if (strcmp(s, "true") == 0) value = 1;
        else error(("il valore inserito non e' booleano"));
        *(int *) vars[oid].addr = value;
    }
}

void read_complex(int oid, char *format) {
    char *stringa = malloc(100 * sizeof(char));

    gets(stringa);
    int is_record = 0;
    int read_name_field = 0;
    void *val = NULL;
    int total_length = 0;
    int j = 0;
    int format_length = strlen(format);
    if (format[0] == '[') {
        stringa++;
        j = 1;
    }
    do {
        for (int i = j; i < format_length; i++) {
            /*  if (format[i]=='{'  ) {  i=strchr(format, ':')-format; is_record=1;stringa++;}
              else if ( format[i]==',')  { if (is_record ) i=strchr(format, ':')-format; strchr(format, ',')+1;  }
               else if (format[i]=='}' ) { stringa=strchr(stringa, '}');is_record=0;}
               else if (format[i]==':' ) { stringa=strchr(stringa, ':') +1 ;}
               else if (format[i]==']' ) {
                   while (stringa[0]!= ',' && stringa[0]!= ']')
                       stringa++;
                   if(stringa[0]== ',') stringa++;
               }
            */
            if (format[i] == '{') {
                is_record = 1;
                read_name_field = 1;
                stringa = strchr(stringa, '{') + 1;
            } else if (format[i] == ',') {
                if (is_record) read_name_field = 1;
                stringa = strchr(stringa, ',') + 1;
            } else if (format[i] == '}') {
                stringa = strchr(stringa, '}');
                is_record = 0;
            } else if (format[i] == ':') {
                if (is_record) read_name_field = 0;
                stringa = strchr(stringa, ':') + 1;
            } else if (format[i] == ']') {
                while (stringa[0] != ',' && stringa[0] != ']')
                    stringa++;
                if (stringa[0] == ',') stringa++;
            } else if (is_record && read_name_field) {
                if (format[i] != stringa[0])
                    error("i nomi dei campi non coincidono");
                else stringa++;
            } else if (format[i] == 'i') {
                int temp = 0;
                sscanf(stringa, "%d", &temp);
                if (val == NULL) {
                    val = malloc(sizeof(int));
                    memcpy(val, &temp, 4);
                    total_length += sizeof(int);
                } else {
                    val = void_concat(val, total_length, &temp, sizeof(int));
                    total_length += sizeof(int);
                }
            } else if (format[i] == 'r') {
                double temp = 0;
                sscanf(stringa, "%lf", &temp);
                if (val == NULL) {
                    val = malloc(sizeof(double));
                    memcpy(val, &temp, 8);
                    total_length += sizeof(double);
                } else {
                    val = void_concat(val, total_length, &temp, sizeof(double));
                    total_length += sizeof(double);
                }
            }
            else if (format[i] == 's') {
                char *temp = malloc(100 * sizeof(char));
                char **pointer = malloc(8);
                stringa++;
                strcpy(temp, stringa);
                int index = strchr(stringa, '\"') - stringa;
                temp[index] = '\0';
                pointer = &temp;
                if (val == NULL) {
                    val = malloc(SIZE_STRING);
                    memcpy(val, &temp, SIZE_STRING);
                    total_length += SIZE_STRING;
                } else {
                    val = void_concat(val, total_length, pointer, SIZE_STRING);
                    total_length += SIZE_STRING;
                }
            } else if (format[i] == 'b') {
                char *temp = malloc(100 * sizeof(char));
                stringa++;
                strcpy(temp, stringa);
                int index = strchr(stringa, '\"') - stringa;
                temp[index] = '\0';
                int value = 0;
                if (strcmp(temp, "false") == 0) value = 0;
                else if (strcmp(temp, "true") == 0) value = 1;
                else error(("il valore inserito non e' booleano"));
                if (val == NULL) {
                    val = malloc(SIZE_INT);
                    memcpy(val, &value, SIZE_INT);
                    total_length += SIZE_INT;
                } else {
                    val = void_concat(val, total_length, &value, SIZE_STRING);
                    total_length += SIZE_INT;
                }
            }
        }
    } while (format[0] == '[' && stringa[0] != ']');
    if (format[0] == '[') {
        int size = vars[oid].size;
        int num = vars[oid].num;
        int new_num = total_length / size;
        if (new_num > num) {
            gestisci_resize(size, new_num, vars[oid], val);
        } else {
            mempcpy(vars[oid].addr, val, total_length);
            vars[oid].num = new_num;
        }
    } else mempcpy(vars[oid].addr, val, total_length);
}


void *extract_val(void *source, int offset, int size) {
    void *val = malloc(size);
    memcpy(val, &source[offset], size);
    return val;
}

void push_obj(Object obj) {
    if (op == MAX_OBJECTS) error("Object stack overflow");
    ostack[op++] = obj;
}

void push_record(Activation_record act) {
    if (ap == MAX_OBJECTS) error("Activation record stack overflow");
    astack[ap++] = act;
    vp++;
    // indice di array di variabili che memorizzano l'oid dell'ultimo indirizzo caricato con load per gestire resize di varibili
}

int total_size(Object obj) {
    return (obj.size * obj.num);
}

Object pop_obj() {
    if (op == 0)
        error("Pop from empty object stack");
    ip -= total_size(ostack[op - 1]);
    return ostack[--op];
}

Activation_record pop_record() {
    if (ap == 0)
        error("Pop from empty activation_record stack");
    vp--;
    return astack[--ap];
}

void *pop_val(int size, int num) {
    void *val;
    val = (&istack[ip - (num * size)]);
    pop_obj();
    return val;
}

char *pop_string() {
    void *temp = malloc(SIZE_STRING);
    memcpy(temp, &istack[ip - SIZE_STRING], SIZE_STRING);
    char *s = *(char **) temp;
    pop_obj();
    return s;
}

int pop_int() {
    int n;
    n = *(int *) &istack[ip - SIZE_INT];
    pop_obj();
    return n;
}

double pop_real() {
    double n;
    n = *(double *) &istack[ip - SIZE_REAL];
    pop_obj();
    return n;
}

void push_val(void *val, int size, int num) {
    Object obj;
    obj.size = size;
    obj.num = num;
    obj.addr = &istack[ip];
    memcpy(obj.addr, val, size * num);
    ip += size * num;
    push_obj(obj);
}

void push_int(int n) {
    Object obj;
    obj.size = SIZE_INT;
    obj.num = 1;
    obj.addr = &istack[ip];
    *(int *) obj.addr = n;
    ip += SIZE_INT;
    push_obj(obj);
}

void push_string(char *s) {
    /** i have to include the '/0' terminator character to future reading without
      * knowing before the length of the string
      */
    char *ps = malloc(strlen(s) + 1);
    strcpy(ps, s);
    Object obj;
    obj.size = SIZE_STRING;
    obj.num = 1;
    obj.addr = &istack[ip];
    *(char **) obj.addr = ps;
    ip += SIZE_STRING;
    push_obj(obj);
}

void push_real(double n) {
    Object obj;
    obj.size = SIZE_REAL;
    obj.num = 1;
    obj.addr = &istack[ip];
    *(double *) obj.addr = n;
    ip += SIZE_REAL;
    push_obj(obj);
}

void write_simple(char *format) {
    if (format[0] == 'i') {
        int n = pop_int();
        printf("%d", n);
    }
    else if (format[0] == 'r') {
        double n = pop_real();
        printf("%lf", n);
    }
    else if (format[0] == 's') {
        char *s = pop_string();
        printf("%s", s);
    }
    else if (format[0] == 'b') {
        int i = pop_int();
        i ? printf("true") : printf("false");
    }
}

void write_complex(char *format) {
    int size = ostack[op - 1].size;
    int num = ostack[op - 1].num;
    int is_record = 0;
    int write_name_field = 0;
    void *val = pop_val(size, num);
    for (int j = 0; j < num; j++) {
        int offset = 0;
        for (int i = 0; i < strlen(format); i++) {
            if (j == 0 && format[i] == '[')
                printf("%c ", format[i]);
            else if (j > 0 && format[i] == '[')
                printf(", ");
            else if (j == num - 1 && format[i] == ']')
                printf("%c ", format[i]);
            else if (format[i] == '{') {
                printf("%c ", format[i]);
                is_record = 1;
                write_name_field = 1;
            } else if (format[i] == '}') {
                is_record = 0;
                printf("%c ", format[i]);
            } else if (format[i] == ':') {
                write_name_field = 0;
                printf("%c ", format[i]);
            } else if (format[i] == ',') {
                write_name_field = 1;
                printf("%c ", format[i]);
            } else if (is_record && write_name_field) {
                printf("%c", format[i]);
            } else if (format[i] == 'i') {
                int n = *(int *) extract_val(val, j * size + offset, SIZE_INT);
                printf("%d ", n);
                offset += SIZE_INT;
            } else if (format[0] == 'b') {
                int b = *(int *) extract_val(val, j * size + offset, SIZE_INT);
                b ? printf("true ") : printf("false ");
                offset += SIZE_INT;
            } else if (format[i] == 'r') {
                double n = *(double *) extract_val(val, j * size + offset, SIZE_REAL);
                printf("%lf ", n);
                offset += SIZE_REAL;
            } else if (format[i] == 's') {
                char **p = extract_val(val, j * size + offset, SIZE_STRING);
                char *s = *p;
                printf("%s ", s);
                offset += SIZE_STRING;
            }
        }
    }
}

void exec_addi() {
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m + n);
}

void exec_addr() {
    double n, m;
    n = pop_real();
    m = pop_real();
    push_real(m + n);
}

void exec_apop() {
    pop_record();
}

void exec_card() {
    int num = ostack[op - 1].num;
    int size = ostack[op - 1].size;
    if (num) pop_val(size, num);
    else pop_obj();
    push_int(num);
}

void exec_cidx() {
    int index = pop_int();
    if (index >= ostack[op - 1].num)
        error("index out of bound");
    else push_int(index);
}


void exec_conc() {
    /**i due vettori vengono letti in ordine inverso dalla pila: viene letta prima la coda e poi la testa**/
    Object obj1 = ostack[op - 1];
    Object obj2 = ostack[op - 2];
    int dim1 = obj1.size * obj1.num;
    int dim2 = obj2.size * obj2.num;
    void *val1 = NULL;
    void *val2 = NULL;
    if (dim1 != 0) val1 = pop_val(obj1.size, obj1.num);
    else pop_obj();
    if (dim2 != 0) val2 = pop_val(obj2.size, obj2.num);
    else pop_obj();
    void *newVal = malloc(dim1 + dim2);
    if (dim1 && dim2) {
        memcpy(&newVal[0], val2, dim2);
        memcpy(&newVal[dim2], val1, dim1);
        push_val(newVal, obj1.size, (dim1 + dim2) / obj1.size);
    }
    //considerando i due casi in cui si sta concatenando l'array vuoto
    else if (dim2 != 0) {
        memcpy(&newVal[0], val2, dim2);
        push_val(newVal, obj1.size, (dim1 + dim2) / obj1.size);
    } else if (dim1 != 0) {
        memcpy(&newVal[0], val1, dim1);
        push_val(newVal, obj1.size, (dim1 + dim2) / obj1.size);
    }
}

void exec_divi() {
    int n, m;
    n = pop_int();
    if (!n) error("la divisione per zero non è ammessa");
    m = pop_int();
    push_int(m / n);
}

void exec_divr() {
    double n, m;
    n = pop_real();
    if (n == 0.0) error("la divisione per zero non è ammessa");
    m = pop_real();
    push_real((m / n));
}

void exec_empt() {
    int num = ostack[op - 1].num;
    pop_obj();
    push_int(num ? 0 : 1);
}

void exec_equa() {
    Object obj1 = ostack[op - 1];
    Object obj2 = ostack[op - 2];
    int dim1 = obj1.size * obj1.num;
    int dim2 = obj2.size * obj2.num;
    void *val1 = pop_val(obj1.size, obj1.num);
    void *val2 = pop_val(obj2.size, obj2.num);
    (dim1 != dim2) ? push_int(0) : push_int(!(memcmp(val1, val2, (dim1 <= dim2) ? dim1 : dim2)));
}

void exec_geqi() {
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m >= n );
}

void exec_geqr() {
    double n, m;
    n = pop_real();
    m = pop_real();
    push_int(m >= n );
}

void exec_geqs() {
    char *n, *m;
    n = pop_string();
    m = pop_string();
    push_int((strcmp(m, n) >= 0));
}

void exec_gthi() {
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m > n );
}

void exec_gthr() {
    double n, m;
    n = pop_real();
    m = pop_real();
    push_int(m > n);
}

void exec_gths() {
    char *n, *m;
    n = pop_string();
    m = pop_string();
    push_int((strcmp(m, n) > 0));
}

void exec_head() {
    Object obj = ostack[op - 1];
    if (obj.num) {
        void *val = pop_val(obj.size, obj.num);
        void *head = extract_val(val, 0, obj.size);
        push_val(head, obj.size, 1);
    }
}

void exec_indl(int offset, int size) {
    Object obj = ostack[op - 1];
    /*
     *non posso usare direttamente pop_val se no decrementa anche ip
     *devo accedere alla istack tramite l'obj.addr che sta sopra ostack
     */
    op--;
    void *val = obj.addr;
    void *field = extract_val(val, offset, size);
    push_val(field, size, 1);
}

void exec_ixad(int scale) {
    int offset = pop_int();
    ostack[op - 1].addr += offset * scale;
}

void exec_jump(int addr) { pc = addr; }

void exec_leqi() {
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m <= n);
}

void exec_leqr() {
    double n, m;
    n = pop_real();
    m = pop_real();
    push_int(m <= n );
}

void exec_leqs() {
    char *n, *m;
    n = pop_string();
    m = pop_string();
    push_int((strcmp(m, n) <= 0));
}

void exec_load(int env, int oid) {
    if (!env) {
        push_val(vars[oid].addr, vars[oid].size, vars[oid].num);
    } else {
        int size = ostack[astack[ap - 1].obj + oid].size;
        int num = ostack[astack[ap - 1].obj + oid].num;
        void *val = malloc(size * num);
        mempcpy(val, ostack[astack[ap - 1].obj + oid].addr, size * num);
        push_val(val, size, num);
    }
}

void exec_loci(int const_i) {
    push_int(const_i);
}

void exec_locr(double const_r) {
    push_real(const_r);
}

void exec_locs(char *const_s) {
    // le stringhe vengono passate come "stringa" e quindi leggendo ho "\"stringa\""
    char *temp = malloc(strlen(const_s));
    temp = memcpy(temp, const_s,strlen(const_s));
    temp[strlen(const_s) - 1] = '\0';
    push_string(&temp[1]);
}

void exec_loda(int env, int oid) {
    Object obj;
    if (!env) {
        obj.size = vars[oid].size;
        obj.num = vars[oid].num;
        obj.addr = vars[oid].addr;
        current_assign_var[vp] = oid;
    } else {
        int index = astack[ap - 1].obj + oid - 1;
        obj.size = ostack[index].size;
        obj.num = ostack[index].num;
        obj.addr = ostack[index].addr;
        current_assign_var[vp] = index;
    }
    push_obj(obj);
}

void exec_lthi() {
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m < n );
}

void exec_lthr() {
    double n, m;
    n = pop_real();
    m = pop_real();
    push_int(m < n);
}

void exec_lths() {
    char *n, *m;
    n = pop_string();
    m = pop_string();
    push_int((strcmp(m, n) < 0));
}

void exec_memb() {
    Object obj = ostack[op - 1];

    int found = 1;
    if (!obj.num)
        error("stai cercando di applicare op is_member su un array vuoto");
    void *array_val = pop_val(obj.size, obj.num);
    void *val = pop_val(obj.size, 1);
    for (int i = 0; i < obj.size * obj.num; i += obj.size)
        if (memcmp(val, &array_val[i], obj.size) == 0) {
            found = 1;
            break;
        }
    push_int(found);
}

void exec_muli() {
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m * n);
}

void exec_mulr() {
    double n, m;
    n = pop_real();
    m = pop_real();
    push_real(m * n);
}

void exec_negb() {
    int m;
    m = pop_int();
    push_int(m ? 0 : 1);
}

void exec_negi() {
    int m;
    m = pop_int();
    push_int(-m);
}

void exec_negr() {
    double m;
    m = pop_real();
    push_real(-m);
}

void exec_nequ() {
    Object obj1 = ostack[op - 1];
    Object obj2 = ostack[op - 2];
    int dim1 = obj1.size * obj1.num;
    int dim2 = obj2.size * obj2.num;
    void *val1 = pop_val(obj1.size, obj1.num);
    void *val2 = pop_val(obj2.size, obj2.num);
    //memcmp return 0 if equals
    dim1 != dim2 ? push_int(1) : push_int(memcmp(val1, val2, dim1 <= dim2 ? dim1 : dim2));
}

void exec_newo(int size, int num) {
    Object var;
    var.size = size;
    var.num = num;
    if (num) {
        var.addr = &istack[ip];
        ip += size;
    } else {
        var.addr = NULL;
    }
    vars[current_assign_var[vp]++] = var;
}

void exec_pack(int num, int size, int card) {
    void *concat;
    int concat_size = 0;
    // card è il numero di elementi che compongono l'array e se record vale sempre 1
    // num è il numero di elementi da impacchettare
    // size è la dimensione del singolo elemento
    for (int i = 0; i < num; i++) {
        if (i) {
            int current_size = ostack[op - 1].size;
            int current_num = ostack[op - 1].num;

            concat = void_concat(pop_val(current_size, current_num), current_size, concat, concat_size);
            concat_size += current_size * current_num;
        } else {
            int s = ostack[op - 1].size;
            int n = ostack[op - 1].num;
            concat_size = 0;
            concat = malloc((s * n));
            memcpy(concat, pop_val(s, n), s * n);
            concat_size += s * n;
        }
    }
    if (!(num == 0 && size == 0 && card == 0))
        push_val(concat, size / card, card);
    else {
        Object obj;
        obj.size = size;
        obj.num = num;
        obj.addr = NULL;
        push_obj(obj);
    }
}

void exec_push(int n) {
    Activation_record act;
    act.obj = op - n;
    act.param_num = n;
    act.ret_addr = pc + 1;
    push_record(act);
}

void exec_read(int oid, char *format) {
    current_assign_var[vp] = oid;
    if (format[0] != '{' && format[0] != '[')
        read_simple(oid, format);
    else read_complex(oid, format);
}

void exec_retn() {
    int size = ostack[op - 1].size;
    int num = ostack[op - 1].num;
    Object obj = ostack[op - 1];
    //getting the return value
    void *val = malloc(size * num);
    if (num)
        val = pop_val(size, num);
    else pop_obj();

    //remoction of all local variables and value on the istack and ostack
    for (int i = 0; i < astack[ap - 1].param_num; i++)
        pop_val(ostack[op - 1].size, ostack[op - 1].num);

    pc = astack[ap - 1].ret_addr;
    if (num) push_val(val, size, num);
    else push_obj(obj);
}

void exec_skip(int offset) {
    pc += offset - 1;
}

void exec_skpf(int offset) {
    //offset -1 per bilanciare incremento pc che avviene a ogni ciclo
    //vale per tutte le istr che modificano direttamente il pc
    if (!pop_int()) pc += offset - 1;
}

void exec_stor() {
    int size = ostack[op - 1].size;
    int num = ostack[op - 1].num;
    // caso normale
    if (!(size == 0 && num == 0)) {
        void *val1 = pop_val(size, num);
        Object obj = ostack[op - 1];
        op--;

        void *temp = malloc(size * num);
        memcpy(temp, val1, size * num);

        // caso array ridimensionato (non  viene gestito il caso in cui l'array è un parametro
        //di una  funzione
        if (obj.num < num)
            gestisci_resize(size, num, obj, temp);
        else memcpy(obj.addr, temp, size * num);
    }
    //caso array[]
    else {
        // rimuovo l'oggetto vuoto e il loda sottostante
        op -= 2;
    }
}

void exec_subi() {
    int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m - n);
}

void exec_subr() {
    double n, m;
    n = pop_real();
    m = pop_real();
    push_real(m - n);
}

void exec_tail() {
    Object obj;
    int size = ostack[op - 1].size;
    int num = ostack[op - 1].num;
    if (!size)
        error("array is empty, cant access to the tail");
    void *val = pop_val(size, num);
    if (num > 1) {
        void *tail = extract_val(val, size, size * num);
        push_val(tail, size, (num - 1));
    } else {
        obj.size = size;
        obj.num = 0;
        obj.addr = NULL;
        push_obj(obj);
    }
}

void exec_toin() {
    double n = pop_real();
    push_int((int) n);
}

void exec_tore() {
    int n = pop_int();
    push_real(n);
}

void exec_vars(int num) {
    vars = (Object *) malloc((sizeof(Object)) * num);
    n_var = num;
}

void exec_writ(char *format) {
    if (strlen(format) == 1)
        write_simple(format);
    else
        write_complex(format);
}



void execute(Pstat istr) {
    switch (istr->op) {
        case ADDI: exec_addi();
            break;
        case ADDR: exec_addr();
            break;
        case APOP: exec_apop();
            break;
        case CARD: exec_card();
            break;
        case CIDX: exec_cidx();
            break;
        case CODE:
            break;
        case CONC: exec_conc();
            break;
        case DIVI: exec_divi();
            break;
        case DIVR: exec_divr();
            break;
        case EMPT: exec_empt();
            break;
        case EQUA: exec_equa();
            break;
        case FUNC: break;
        case GEQI: exec_geqi();
            break;
        case GEQR: exec_geqr();
            break;
        case GEQS: exec_geqs();
            break;
        case GTHI: exec_gthi();
            break;
        case GTHR: exec_gthr();
            break;
        case GTHS: exec_gths();
            break;
        case HALT: break;
        case HEAD: exec_head();
            break;
        case INDL: exec_indl(istr->arg1.ival, istr->arg2.ival);
            break;
        case IXAD: exec_ixad(istr->arg1.ival);
            break;
        case JUMP: exec_jump(istr->arg1.ival);
            break;
        case LEQI: exec_leqi();
            break;
        case LEQR: exec_leqr();
            break;
        case LEQS: exec_leqs();
            break;
        case LOAD: exec_load(istr->arg1.ival, istr->arg2.ival);
            break;
        case LOCI: exec_loci(istr->arg1.ival);
            break;
        case LOCR: exec_locr(istr->arg1.rval);
            break;
        case LOCS: exec_locs(istr->arg1.sval);
            break;
        case LODA: exec_loda(istr->arg1.ival, istr->arg2.ival);
            break;
        case LTHI: exec_lthi();
            break;
        case LTHR: exec_lthr();
            break;
        case LTHS: exec_lths();
            break;
        case MEMB: exec_memb();
            break;
        case MULI: exec_muli();
            break;
        case MULR: exec_mulr();
            break;
        case NEGB: exec_negb();
            break;
        case NEGI: exec_negi();
            break;
        case NEGR: exec_negr();
            break;
        case NEQU: exec_nequ();
            break;
        case NEWO: exec_newo(istr->arg1.ival, istr->arg2.ival);
            break;
        case PACK: exec_pack(istr->arg1.ival, istr->arg2.ival, istr->arg3.ival);
            break;
        case PUSH: exec_push(istr->arg1.ival);
            break;
        case READ: exec_read(istr->arg1.ival, istr->arg2.sval);
            break;
        case RETN: exec_retn();
            break;
        case SKIP: exec_skip(istr->arg1.ival);
            break;
        case SKPF: exec_skpf(istr->arg1.ival);
            break;
        case STOR: exec_stor();
            break;
        case SUBI: exec_subi();
            break;
        case SUBR: exec_subr();
            break;
        case TAIL: exec_tail();
            break;
        case TOIN: exec_toin();
            break;
        case TORE: exec_tore();
            break;
        case VARS: exec_vars(istr->arg1.ival);
            break;
        case WRIT: exec_writ(istr->arg1.sval);
            break;
        default: printf("Operazione sconosciuta\n");
            break;
    }
}

void start_execution(Pstat program) {
    prog = program;
    pc = 0;
    istack = malloc(100000);

    while (prog[pc].op != HALT) {
        execute(&prog[pc]);
        pc++;
    }
}
