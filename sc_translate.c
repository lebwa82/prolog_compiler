#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "header.h"

enum leexema123 {
    PROGR = -150,
    MOP,
    P,
    H,
    B,
    S,
    E,
    T,
    F,
    a,
    ARG,
};

struct rule {
    int L;
    int R[10];
    int condition_symbol[10]; //= {-1};
    struct rule *next;
};


int *main_mass;
int main_mass_size = 0;
int name_table_size = 0;
struct literal *Names;

int main()
{
    FILE *file_mass;
    if ((file_mass = fopen("mainmass.txt", "r")) == NULL) {
        printf("Can't open file mainmass.txt\n");
        return 0;
    }
    int elem;
    char str[30];
    while ((fscanf(file_mass, "%d", &elem) != EOF))
        main_mass_size++;
    fseek(file_mass, 0, SEEK_SET);
    int *main_mass = (int *)malloc(sizeof(int) * main_mass_size);
    for (int i = 0; i < main_mass_size; i++) {
        fscanf(file_mass, "%d", &main_mass[i]);
        printf("%2d ", main_mass[i]);
    }

    FILE *name_table;
    if ((name_table = fopen("name_table.txt", "r")) == NULL) {
        printf("Can't open file name_table.txt\n");
        return 0;
    }
    puts("");
    while ((fscanf(name_table, "%d %d %d %s %s %d\n", &elem, &elem, &elem, str, str, &elem) != EOF))
        name_table_size++;
    fseek(name_table, 0, SEEK_SET);
    struct literal *Names = (struct literal *)malloc(sizeof(struct literal) * name_table_size);
    int id, type, const_or_var, value_int;
    char literal_name[30], value_char[30]; 
    printf("id \ttype \tconst_or_var \tliteral_name \t\tvalue\n");
    for (int i = 0; i < name_table_size; i++) {
        fscanf(name_table, "%d %d %d %s %s %d\n", &id, &type, &const_or_var, literal_name, value_char, &value_int);
        Names[i].id = id;
        Names[i].type = type;
        //printf("\ntype is %i\n", type);
        Names[i].const_or_var = const_or_var;
        strcpy(Names[i].literal_name, literal_name);
        strcpy(Names[i].value_char, value_char);
        Names[i].value_int = value_int;
        if (Names[i].type == DIGIT){
            Names[i].value_char[0] = '\0'; 
            printf("%d \t%d \t%d \t%s %20d\n", Names[i].id, Names[i].type, Names[i].const_or_var, Names[i].literal_name, Names[i].value_int);
        }
        else
            printf("%d \t%d \t%d \t%s %20s\n", Names[i].id, Names[i].type, Names[i].const_or_var, Names[i].literal_name, Names[i].value_char);
    }



    struct rule S__E = {//S->E
        S, {E, 0}, {0}, NULL
    };

    struct rule S__S_equal_E = {//S->S=E
        S, {is_key_word('='), E, 0}, {0}, NULL
    };

    struct rule S__S_bigger_E = {//S->S>E
        S, {is_key_word('>'), E, 0}, {0}, NULL
    };

    struct rule  S__S_smaller_E = { //S->S<E
        S, {is_key_word('<'), E}, {0}, NULL
    };

    struct rule E__T = { //E->T
        E, {T, 0}, {is_key_word(')'), is_key_word('+'), is_key_word('-'), 0}, NULL //TBD СПЕЦСИМВОЛ-РЕШЕТКА
    };

    struct rule E__E_plus_T = {//E->E+T
        E, 
        {E, is_key_word('+'), T, 0},
        {is_key_word(')'), is_key_word('+'), is_key_word('-'), 0}, 
        NULL
    };

    struct rule E__E_minus_T = {//E->E-T
        E, {E, is_key_word('-'), T, 0}, {is_key_word(')'), is_key_word('+'), is_key_word('-'), 0}, NULL
    };


    struct rule T__F = {//T->F
        T, {F, 0}, {0}, NULL
    };

    struct rule T__T_multiply_F = {//T->T*F
        T,
        {is_key_word('*'), F, 0},
        {is_key_word('+'), is_key_word('-'), is_key_word('*'), is_key_word('/'), is_key_word(')'), 0},
        NULL
    };

    struct rule T__T_divide_F = {//T->T/F
        T,
        {is_key_word('/'), F, 0},
        {is_key_word('+'), is_key_word('-'), is_key_word('*'), is_key_word('/'), is_key_word(')'), 0},
        NULL
    };
    struct rule F__a = {//F->a
        F, {a, 0}, {0}, NULL
    };

    struct rule F__E = {//F->(E)
        F, {is_key_word('('), E, is_key_word(')'), 0}, {0}, NULL
    };

    struct rule ARG__a = {//ARG->a
        ARG, {a, 0}, {0}, NULL
    };

    struct rule ARG__ARG_COMMON_a = {//ARG->ARG,a
        ARG, {ARG, is_key_word(','), a, 0}, {0}, NULL
    };

    struct rule PROGR__MOP = {//PROGR->MOP 
        PROGR, {MOP, 0}, {0}, NULL
    };

    struct rule MOP__MOP_DOT_P = {//MOP->MOP. P
        MOP, {MOP, is_key_word('.'), P, 0}, {0}, NULL
    };

    struct rule MOP__P = {//MOP->MOP. P
        MOP, {P, 0}, {0}, NULL
    };

    struct rule P__H_COLON_DASH_B_DOT = {//P->H:-B.
        P, {H, is_key_word(':'), is_key_word('-'), B, is_key_word('.'), 0}, {0}, NULL
    };

    struct rule P__H_DOT = {//P->H:-B. 
        P, {H, is_key_word('.'), 0}, {0}, NULL
    };
    
    /*
    17) PROGR->MOP              R = '\0'
18) MOP->MOP. P
19) MOP->P
20) P->H:-B.
21) P->H. 


22) B->B,S
23) B->B,a(ARG)
24) B->S            
25) B->a(ARG).       R= "." "," и -5 символ if
26) H->a(ARG)       R= ":-" "."
    */



}