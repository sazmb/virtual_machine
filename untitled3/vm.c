
#include "vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_OBJECTS 1000000
int const SIZE_INT=4;
int const SIZE_REAL=8;
int const SIZE_STRING=4;
Pstat prog;
int pc;
Object *vars;
Object ostack [MAX_OBJECTS];
Activation_record astack[MAX_OBJECTS];
void* istack;
int vp=0;
int op=0;
int ip;
int ap;
void error(char* msg) {
    printf("%s", msg);
    exit (EXIT_FAILURE);
}
char* concatena_stringa(char* buffer,  char* aggiunta) {

    // Calcoliamo la nuova lunghezza necessaria
    size_t lunghezza_iniziale = (buffer ? strlen(buffer) : 0);
    size_t nuova_lunghezza = lunghezza_iniziale + strlen(aggiunta) + 1; // +1 per il terminatore '\0'
    char* new_buffer=(char*) malloc (nuova_lunghezza);
    // Riallociamo la memoria per il buffer

    strcpy(new_buffer, buffer);
    if (new_buffer == NULL) {
        perror("Errore nella riallocazione della memoria");
        exit(EXIT_FAILURE);
    }

    // Se il buffer è stato appena creato (era NULL), dobbiamo assicurarci che contenga una stringa vuota
    if (lunghezza_iniziale == 0) {
        buffer[0] = '\0'; // Impostiamo il terminatore di stringa iniziale
    }


    strcat(new_buffer, aggiunta);

    return new_buffer;


}
void read_simple(int oid , char* format) {
    if (strcmp(format, "i")) {
        int i;
        scanf("%d",&i );
        *(int *) vars[oid].addr=i;
    }
    if (strcmp(format, "r")) {
        double r;
        scanf("%lf",&r );
        *(double *) vars[oid].addr=r;
    }
    if (strcmp(format, "s")) { // non basta cosi perchè è un puntatore, va salvato in un altro modo
        char *s;
        scanf("%s",s );
        vars[oid].addr=s;
    }
}
void read_complex(int oid , char* format) {
    //record
    if (format[0]=='{') {
        //va modificata la stringa per rimuovere le virgole in piu bisogna capire se prendere come char
        //un numero e poi castarlo basta oppure devo fare atoi e poi tornare a trasformalro in car
        char *s;
        scanf("%s",s );
        vars[oid].addr=s;
    }
    //array di record
    else if (format[0]=='['&& format[1]=='{') {
        //
        //
        char *s;
        scanf("%s",s );
        vars[oid].addr=s;
    }
    //record normali
    else {
        char *s;
        scanf("%s",s );
        vars[oid].addr=s;
    }

}

/**
 * non ricordo bene ma mi sembra che serva a estrarre il campo di un record
 * o l'elemento di un vettore
 * @param source
 * @param start
 * @param end
 * @return
 */
void* extract_val(char * source, int start, int end) {
    void* val [end-start] ;
    for (int i=start; i<end; i++)
        val[i]=source[start+i];
    return val;
}

void push_obj(Object obj)
{
    if(op == MAX_OBJECTS) error("Object stack overflow");
    ostack[op++] = obj;
}
int total_size(Object obj) {
    return (obj.size * obj.num);
}
Object pop_obj()
{
    if(op == 0)
        error("Pop from empty object stack");
    ip -= total_size(ostack[op-1]);
    return ostack[--op];
}
void* pop_val(int size, int num) {
    void* val;
    val= (&istack[ip - (num*size)]);
    pop_obj();
    return val;
}
char* pop_string() {
  return   pop_val(SIZE_STRING,1);
}
int pop_int()
{   int n;
    n = * (int *) &istack[ip - SIZE_INT];
    pop_obj();
    return n;
}
double pop_real(){
    double n;
    n = * (double *) &istack[ip - SIZE_REAL];
    pop_obj();
    return n;
}
void push_val(void* val , int dim , int num) {
    Object obj;
    obj.size = dim;
    obj.num = num;
    obj.addr = &istack[ip];
    memcpy(obj.addr, val,dim);
    ip += dim;
    push_obj(obj);
}
void push_int(int n)
{
    Object obj;
    obj.size = SIZE_INT;
    obj.num = 1;
    obj.addr = &istack[ip];
    * (int *) obj.addr = n;//non funziona non converte l'int in bit ma semplicemente assegna il valore intero a un char che lo legge come ASCII
    ip += SIZE_INT;
    push_obj(obj);
}
void push_string(char *s) {
    push_val(s, SIZE_STRING, 1);
}
void push_real(double n)
{ Object obj;
    obj.size = SIZE_REAL;
    obj.num = 1;
    obj.addr = &istack[ip];
    * (double *) obj.addr = n;
    ip += SIZE_REAL;
    push_obj(obj);
}
void write_simple(char *format) {
    if(format[0]=='i') {
        int n=pop_int();
        printf("%d",n);
    }
    else if(format[0]=='r') {
        double n=pop_real();
        printf("%lf",n);
    }if(format[0]=='s') {
        char* s=pop_string();
        printf("%s",s);
    }

}
void write_complex(char *format) {
    char* val=pop_val(ostack[op].size);
    for (int i=0; i<strlen(format); i++) {
        if (format[i]=='[' ||  format[i]==',' || format[i]==']' || format[i]=='{' || format[i]=='}')
            printf("%c",format[i]);
        else if (format[i]=='i'){
            int n= *(int*) extract_val(val, i , i+SIZE_INT);
            printf("%d",n);
        }
        else if (format[i]=='r'){
            double n = *(double*) extract_val(val, i , i+SIZE_REAL);
            printf("%lf",n);
        }
        else if (format[i]=='s'){
            char* s =   extract_val(val, i , i+SIZE_STRING);
            printf("%s",s);
        }
    }
}
void initialize_istack (int start, int size) {
    for(int i=start; i<size; i++);
}
void* concat_void (void* val1, void* val2) {
    return val1;

}
void exec_addi()
{ int n, m;

    n = pop_int();
    m = pop_int();
    push_int(m+n);
}
void exec_addr(){
    double n, m;
    n = pop_real();
    m = pop_real();
    push_real(m+n);
}
void exec_apop() {
    if(ap == 0)
    error("Pop from empty acrivation stack");
    op=astack[ap].ret_addr;
        ap--; }// controllare valore di ritorno
void exec_card() {
    int dim=ostack[op].num;
    pop_obj();
    push_int(dim);
}
void exec_cidx() {
     int index=pop_int();
    if (index>=ostack[op].num)
        error("index out of bound");
    else push_int(index);
}
void exec_code() {
    /**
     *scrivi il code con chat che risize ostack, non credo che serva perchè ostack rimane della grandezza
     *massima al massimo in futuro puoi mettere gestione dinamica memo per risparmiare sapazio
    */
}
void exec_conc()  {
  /**
    void* new_val=concat_void(val1, val2);
    **/
    /**i due vettori vengono letti in ordine inverso dalla pila: viene letta prima la coda e poi la testa**/
    Object obj1=ostack[op-1];
    Object obj2=ostack[op-2];
    int dim1=obj1.size*obj1.num;
    int dim2=obj2.size*obj2.num;
    void* val1= pop_val(obj1.size, obj1.num);
    void* val2= pop_val(obj2.size,obj2.num);
    void* newVal=malloc(dim1+dim2);
    memcpy(&newVal[0],val2,dim2);
    memcpy(&newVal[dim2-1],val1,dim1);
    push_val(newVal,obj1.size,dim1+dim2);
}
void exec_divi() {
    int n, m;
    n=pop_int();
    if (!n) error("division by 0 not allowed");
    m=pop_int();
    push_int(m/n);
}
void exec_divr() { int n, m;
    n=pop_real();
    if (n==0.0) error("division by 0 not allowed");
    m=pop_real();
    push_real((m/n));}
void exec_empt() {
    int dim=ostack[op].num;
    pop_obj();
    push_int(dim ? 0 : 1);
}
void exec_equa() {
    /**
     * in teoria la symbol table ha gia controllato che siano compatibili
     *quindi basterebbe la dimensione di uno o dell'altro
     *
     */
    Object obj1=ostack[op-1];
    Object obj2=ostack[op-2];
    int dim1=obj1.size*obj1.num;
    int dim2=obj2.size*obj2.num;
    void* val1= pop_val(obj1.size, obj1.num);
    void* val2= pop_val(obj2.size,obj2.num);
    push_int((memcmp(val1, val2,(dim1<=dim2) ? dim1 : dim2)!=0)?0:1);

}//check del type già fatto dalla stable???
void exec_geqi() {  int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m>=n ? 1: 0);}
void exec_geqr() {  double n, m;
    n = pop_real();
    m = pop_real();
    push_int(m>=n ? 1:0);}
void exec_geqs() {  char *n, *m;
    n = pop_string();
    m = pop_string();
    push_int((strcmp(m,n )>=0) ? 1: 0);}
void exec_gthi() { int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m>n ? 1: 0);}
void exec_gthr() { double n, m;
    n = pop_real();
    m = pop_real();
    push_int(m>n ? 1: 0);}
void exec_gths() {char *n, *m;
    n = pop_string();
    m = pop_string();
    push_int((strcmp(m,n )>0) ? 1: 0);}
void exec_head() {
    Object obj=ostack[op-1];
    if(!obj.size)
        error("array is empty, cant access to the head");
    void* val=pop_val(obj.size, obj.num);
    void* head=extract_val(val, 0 , obj.size);
    push_val(head, obj.size,1);}
void exec_indl(int offset, int size) {

    Object obj =ostack[op-1];
    if(!size)
        error("array is empty, cant access to the head");
    void* val=pop_val(obj.size,obj.num);
    void* field=extract_val(val, offset , size);
    push_val(field, size,1);
}
void exec_ixad(int scale ) {
    int offset=pop_int();
    ostack[op-1].addr+=offset*scale;
}
void exec_jump(int addr) {pc=addr;}
void exec_leqi() {int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m<=n ? 1: 0); }
void exec_leqr() { double n, m;
    n = pop_real();
    m = pop_real();
    push_int(m<=n ? 1: 0); }
void exec_leqs() { char *n, *m;
    n = pop_string();
    m = pop_string();
    push_int((strcmp(m,n )<=0) ? 1: 0); }
void exec_load(int env, int oid) {
    if(!env) {
       push_val( vars[oid].addr, vars[oid].size,vars[oid].num);
    }
}
void exec_loci(int const_i) {
    push_int(const_i);
}
 void exec_locr(double const_r) {
    push_real(const_r);
}
void exec_locs(char* const_s) {
    push_string(const_s);
}
void exec_loda(int env ,int oid) {
    if(!env) {
    Object obj;
    obj.size=vars[oid].size;
    obj.num=vars[oid].num;
    obj.addr=vars[oid].addr;
    push_obj(obj);
} }
void exec_lthi() { int n, m;
    n = pop_int();
    m = pop_int();
    push_int(m<n ? 1: 0); }
void exec_lthr() { double n, m;
    n = pop_real();
    m = pop_real();
    push_int(m<n ? 1: 0); }
void exec_lths() { char *n, *m;
    n = pop_string();
    m = pop_string();
    push_int((strcmp(m,n )<0) ? 1: 0); }
void exec_memb()   {
    Object obj =ostack[op-1];

    int found=1;
    void* array_val=pop_val(obj.size,obj.num);
    void* val=pop_val(obj.size,1);
    for(int i=0; i<obj.size*obj.num; i+=obj.size)
       if( memcmp(val, &array_val[i],obj.size )==0) {
           found=1;
           break;
       }
    push_int(found);
 }
void exec_muli() {  int n, m;
    n=pop_int();
    m=pop_int();
    push_int(m*n);}
void exec_mulr() {   double n, m;
    n = pop_real();
    m = pop_real();
    push_real(m*n); }
void exec_negb() {  int  m;
    m = pop_int();
    push_int(m?0:1); }
void exec_negi() {  int m;
    m = pop_int();
    push_real(-m); }
void exec_negr() {   double m;
    m = pop_real();
    push_real(-m);}
void exec_nequ() {
    Object obj1=ostack[op-1];
    Object obj2=ostack[op-2];
    int dim1=obj1.size*obj1.num;
    int dim2=obj2.size*obj2.num;
    void* val1= pop_val(obj1.size, obj1.num);
    void* val2= pop_val(obj2.size,obj2.num);
    push_int((memcmp(val1, val2,(dim1<=dim2) ? dim1 : dim2)!=0)?0:1);
}
void exec_newo(int size ,  int num) {
    Object var;
    var.size=size;
    var.num=num;
  if (num){
      initialize_istack(ip, size);
      var.addr= &istack[ip];
      ip += size;
  }else {
      var.addr=NULL;
  }
      vars[vp++]=var;
}
void exec_pack(int num, int size , int card) {
    void* vars[num];
    void* concat;
    for (int i=0; i<num; i++)
      if(i)
          concat=concatena_stringa(concat,  pop_val(ostack[op-i].size) );
      else
          { concat=pop_val( ostack[op-i].size);}
    push_val(concat, strlen(concat));
    if(card>1)
        ostack[op].num=card;
}
void exec_push(int n) {
      Activation_record act;
      act.obj=ostack[op];
    act.param_num=n;
    act.ret_addr=pc+1;
}
void exec_read(int oid , char * format) {
    if (format[0]!='{' && format[0]!='[')
        read_simple( oid ,  format);
    else read_complex(oid ,format);
}
void exec_retn() {
    void* val=pop_val(ostack[op-1].size,ostack[op-1].num);
    for (int i =0; i<astack[ap].param_num;i++)
        pop_val(ostack[op-1].size,ostack[op-1].num);
    pc=astack[ap-1].ret_addr;
    ap--;
    push_val(val,strlen(val));
}
void exec_skip(int offset) {
    pc+=offset-1;
}
void exec_skpf(int offset) {
    if(pop_int()) pc+=offset-1;
}
void exec_stor() {
    char* val1=pop_val(ostack[op-1].size);
  Object obj =pop_obj();
  strcpy(obj.addr,val1);
}
void exec_subi() { int n, m;
    n=pop_int();
    m=pop_int();
    push_int(m-n); }
void exec_subr() { double n, m;
    n=pop_real();
    m=pop_real();
    push_real(m-n);}
void exec_tail() {  Object obj;
    int size=ostack[op].size;
    int num=ostack[op].num;
    if(!size)
        error("array is empty, cant access to the head");
    char* val=pop_val(size);
    char* tail=extract_val(val, size, size*num);
    push_val(tail, strlen(tail));}
void exec_toin() {
    double n=pop_real();
    push_int((int) n);
}
void exec_tore() {
    int n =pop_int();
    push_real(n);
}
void exec_vars(int num) {
    vars= (Object*) malloc(sizeof (Object)*num);
}
void exec_writ(char* format) {
    if (strlen(format)==1)
        write_simple(format);
    else
        write_complex(format);
}

// Funzione switch per eseguire la funzione corrispondente
void execute(Pstat istr) {
    switch(istr->op) {
        case ADDI: exec_addi(); break;
        case ADDR: exec_addr(); break;
        case APOP: exec_apop(); break;
        case CARD: exec_card(); break;
        case CIDX: exec_cidx(); break;
        case CODE: exec_code(); break;
        case CONC: exec_conc(); break;
        case DIVI: exec_divi(); break;
        case DIVR: exec_divr(); break;
        case EMPT: exec_empt(); break;
        case EQUA: exec_equa(); break;
        case FUNC:  break;
        case GEQI: exec_geqi(); break;
        case GEQR: exec_geqr(); break;
        case GEQS: exec_geqs(); break;
        case GTHI: exec_gthi(); break;
        case GTHR: exec_gthr(); break;
        case GTHS: exec_gths(); break;
        case HALT: break;
        case HEAD: exec_head(); break;
        case INDL: exec_indl(istr->arg1.ival,istr->arg2.ival); break;
        case IXAD: exec_ixad(); break;
        case JUMP: exec_jump(istr->arg1.ival); break;
        case LEQI: exec_leqi(); break;
        case LEQR: exec_leqr(); break;
        case LEQS: exec_leqs(); break;
        case LOAD: exec_load(istr->arg1.ival, istr->arg2.ival); break;
        case LOCI: exec_loci(istr->arg1.ival); break;
        case LOCR: exec_locr(istr->arg1.rval); break;
        case LOCS: exec_locs(istr->arg1.sval); break;
        case LODA: exec_loda(istr->arg1.ival, istr->arg2.ival); break;
        case LTHI: exec_lthi(); break;
        case LTHR: exec_lthr(); break;
        case LTHS: exec_lths(); break;
        case MEMB: exec_memb(); break;
        case MULI: exec_muli(); break;
        case MULR: exec_mulr(); break;
        case NEGB: exec_negb(); break;
        case NEGI: exec_negi(); break;
        case NEGR: exec_negr(); break;
        case NEQU: exec_nequ(); break;
        case NEWO: exec_newo(istr->arg1.ival, istr->arg2.ival); break;
        case PACK: exec_pack(istr->arg1.ival, istr->arg2.ival, istr->arg3.ival); break;
        case PUSH: exec_push(istr->arg1.ival); break;
        case READ: exec_read(istr->arg1.ival, istr->arg2.sval); break;
        case RETN: exec_retn(); break;
        case SKIP: exec_skip(istr->arg1.ival); break;
        case SKPF: exec_skpf(istr->arg1.ival); break;
        case STOR: exec_stor(); break;
        case SUBI: exec_subi(); break;
        case SUBR: exec_subr(); break;
        case TAIL: exec_tail(); break;
        case TOIN: exec_toin(); break;
        case TORE: exec_tore(); break;
        case VARS: exec_vars(istr->arg1.ival); break;
        case WRIT: exec_writ(istr->arg1.sval); break;
        default: printf("Operazione sconosciuta\n"); break;
    }
}

void start_execution(Pstat program, int length) {
    prog=program;
    pc =0;
    istack=  malloc(10);

    while(prog[pc].op!=HALT) {
        execute(&prog[pc]);
        pc++;
    }
}


