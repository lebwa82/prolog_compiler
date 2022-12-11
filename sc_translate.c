#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "header.h"

enum leexema123 {
    PROGR = -150,
    MOP,
    P,
    Q,
    S,
    E,
    T,
    F,
    a,
    ARG,
};

struct rule {
    int id_rule;
    int L;
    int R[10];
    int condition_symbol[20]; //= {-1};
    struct rule *next;
};


int main_mass[100];
int main_mass_size = 0;
int name_table_size = 0;
struct literal *Names;

int get_len_rule_R(int *R)
{
    int i=0;
    while (R[i] != 0) {
        i++;
    }
    return i;
}

int is_symbol_not_a_condition_symbol(int main_i, int rule_len_R, int * condition_symbol)
{
    int i=0;
    //printf("main_i+rule_len_R = %d main_mass_size = %d\n", main_i+rule_len_R, main_mass_size);
    if(main_i+rule_len_R >= main_mass_size)
        return 0;
    int symbol = main_mass[main_i+rule_len_R];
    for(i=0; i<20; i++) {
        if(condition_symbol[i] == symbol)
            return 1;
    }
    return 0;
}




int compress_main_massiv(int left, int size)
{
    if (left + size > main_mass_size) {
        printf("compress_main_massiv ERROR\n");
        return -1;
    }

    while (main_mass[left + size] != 0) {
        main_mass[left] = main_mass[left + size];
        left++;
    }
    main_mass[left] = main_mass[left + size];
    left++;
    main_mass_size=main_mass_size-size;
    return 0;
}


int print_main_mass()
{   
    printf("main_mass_size = %d\n", main_mass_size);
    for (int i = 0; i < main_mass_size; i++) {
        printf("%2d ", main_mass[i]);
    }
    printf("\n");
    return 0;
}

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
    //int *main_mass = (int *)malloc(sizeof(int) * main_mass_size);
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
        fscanf(name_table, "%d %d %d %s %s %d\n", &id, &type, &const_or_var, literal_name, value_char,
               &value_int);
        Names[i].id = id;
        Names[i].type = type;
        //printf("\ntype is %i\n", type);
        Names[i].const_or_var = const_or_var;
        strcpy(Names[i].literal_name, literal_name);
        strcpy(Names[i].value_char, value_char);
        Names[i].value_int = value_int;
        if (Names[i].type == DIGIT) {
            Names[i].value_char[0] = '\0';
            printf("%d \t%d \t%d \t%s %20d\n", Names[i].id, Names[i].type, Names[i].const_or_var,
                   Names[i].literal_name, Names[i].value_int);
        } else
            printf("%d \t%d \t%d \t%s %20s\n", Names[i].id, Names[i].type, Names[i].const_or_var,
                   Names[i].literal_name, Names[i].value_char);
    }

    struct rule PROGR__MOP = {//PROGR->MOP
        7, PROGR, {MOP}, {0}, NULL
    };
    struct rule MOP__MOP_P = {//MOP->MOP P
        8, MOP, {MOP, P}, {0}, &PROGR__MOP
    };
    struct rule MOP__P = {//MOP->P
        9, MOP, {P }, {0}, &MOP__MOP_P
    };

    
    struct rule P__a_BRACKET_ARG_BRACKET_POINT = {//P->a(ARG).
        1, P, {a, is_key_word('('), ARG, is_key_word(')'), is_key_word('.')}, {0}, &MOP__P
    };

    struct rule P__Q_POINT = {//P->Q.
        2, P, {Q, is_key_word('.')}, {0}, &P__a_BRACKET_ARG_BRACKET_POINT
    };

    struct rule Q__Q_COMMA_a__BRACKET_ARG_BRACKET = {//Q->Q,a(ARG)
       3, Q, {Q, is_key_word(','), a, is_key_word('('), ARG, is_key_word(')'), 0}, {0}, &P__Q_POINT
    };

    struct rule Q__a_BRACKET_ARG_BRACKET_COLON_DASH_a_BRACKET_ARG_BRACKET = {//Q->a(ARG):-a(ARG)
       4, Q,
        {a, is_key_word('('), ARG, is_key_word(')'), is_key_word(':'), is_key_word('-'), a, is_key_word('('), ARG, is_key_word(')')},
        {0},
        &Q__Q_COMMA_a__BRACKET_ARG_BRACKET
    };
    struct rule ARG__a = {//ARG->a
       5, ARG, {a, 0}, {is_key_word('('), 0}, &Q__a_BRACKET_ARG_BRACKET_COLON_DASH_a_BRACKET_ARG_BRACKET
    };
    struct rule ARG__ARG_COMMA_a = {//ARG->ARG,a
       6, ARG, {ARG, is_key_word(','), a, 0}, {0}, &ARG__a
    };


    print_main_mass();
    for (int i = 0; i < main_mass_size; i++) {
        if(main_mass[i] > 0)
        {
            main_mass[i] = a;
        }
    }
    print_main_mass();
    //getchar();
    main_mass[main_mass_size] = 0;
    int main_i, rule_len_R, i=0, j;
    struct rule *head = &ARG__ARG_COMMA_a;
    struct rule *p = head;
    main_i = 0;
    while(main_i <= main_mass_size){
        p = head;
        while (p != NULL) {
            int flag = 0;
            int *R = p->R;
            //printf("id_rule = %d main_i = %d\n", p->id_rule, main_i);
            if(R == NULL) {
                printf("NULL\n");
                return 0;
            }
          //  getchar();
            rule_len_R = get_len_rule_R(p->R);
            if(p->id_rule == 1)
            {
                //printf("rule_len_R = %d\n", rule_len_R);
                //printf("\n j = %d!!!!!!!!!!!!!!!\n", j);
            }
                
            j = main_i;
            //printf("\n j = %d!!!!!!!!!!!!!!!\n", j);
            for (i = 0; i < rule_len_R; i++, j++) {
                //if(p->id_rule == 1)
                //    printf(" i = %3d, j = %3d main_mass[j] = %3d R[i] = %3d\n", i, j, main_mass[j], main_mass[j]);
                if (main_mass[j] != R[i]) {
                    p = p->next;
                    flag = 1;//правило не подошло
                    //printf("правило не подошло\n\n");
                    break;
                }
            }
            //printf("Flag = %i\n", flag);
            
            if (flag == 0) { //правило подошло, заменяем R->L
                int current_symbol = main_mass[main_i+rule_len_R];
                //printf("For rule number %d cond_symb = %d\n", p->id_rule ,is_symbol_not_a_condition_symbol(main_i, rule_len_R, p->condition_symbol));
                if(is_symbol_not_a_condition_symbol(main_i, rule_len_R, p->condition_symbol))
                {
                    p = p->next;
                    //main_i++;
                    continue;
                }
                //printf("before= main_i+1 = %d len = %d\n", main_i+1, rule_len_R);
                //print_main_mass();
                print_main_mass();
                compress_main_massiv(main_i+1, rule_len_R-1);
                main_mass[main_i] = p->L;
                main_i = -1;
                print_main_mass();
                printf("Done rule %d\n", p->id_rule);
                getchar();
                break;
            }
            //p = p->next;
        }
        main_i++;
    }
    printf("END\n");
    print_main_mass();

return 0;
}
