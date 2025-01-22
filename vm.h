#ifndef VM_H
#define VM_H
typedef enum {
    ADDI,
    ADDR,
    APOP,
    CARD,
    CIDX,
    CODE,
    CONC,
    DIVI,
    DIVR,
    EMPT,
    EQUA,
    FUNC,
    GEQI,
    GEQR,
    GEQS,
    GTHI,
    GTHR,
    GTHS,
    HALT,
    HEAD,
    INDL,
    IXAD,
    JUMP,
    LEQI,
    LEQR,
    LEQS,
    LOAD,
    LOCI,
    LOCR,
    LOCS,
    LODA,
    LTHI,
    LTHR,
    LTHS,
    MEMB,
    MULI,
    MULR,
    NEGB,
    NEGI,
    NEGR,
    NEQU,
    NEWO,
    PACK,
    PUSH,
    READ,
    RETN,
    SKIP,
    SKPF,
    STOR,
    SUBI,
    SUBR,
    TAIL,
    TOIN,
    TORE,
    VARS,
    WRIT
    }Operator;

typedef union  {
    int ival;
    double rval;
    char* sval;
}Value;

typedef struct sobject{
    int size;
    int num;
    void *addr;
}Object;

typedef struct sactivation{
    int param_num;
    int obj;
    int ret_addr;
}Activation_record;

typedef struct sstat {
    int addr;
    Operator op;
    Value arg1;
    Value arg2;
    Value arg3;

}Stat;
typedef Stat* Pstat;
void start_execution (Pstat program);

#endif //VM_H
