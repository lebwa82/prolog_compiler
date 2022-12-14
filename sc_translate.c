#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "header.h"

void print_sentence_spisok();
void print_relation_spisok();


int main_mass[100];
int main_mass_copy[100];
int main_mass_size = 0;
int name_table_size = 0;
struct literal *Names;

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

typedef struct lex_words {
    int index;
    char word[10];
} lex_words;

lex_words lexema_string[] = {{PROGR, "PROGR"}, {MOP, "MOP"}, {P, "P"}, {Q, "Q"},
    {S, "S"}, {E, "E"}, {T, "T"}, {F, "F"}, {a, "a"}, {ARG, "ARG"}
};

char *get_lex_string(int word)
{
    for (int i = 0; i < 10; i++) {
        //printf("%d %c\n", key_words_list[i].index, key_words_list[i].word);
        if (lexema_string[i].index == word)
            return lexema_string[i].word;
    }
    return NULL;
}

int get_key_word(int index)
{
    for (int i = 0; i < len_key_words_list; i++) {
        //printf("%d %c\n", key_words_list[i].index, key_words_list[i].word);
        if (key_words_list[i].index == index)
            return key_words_list[i].word;
    }
    return 0;
}

int stack_len;

char *get_literal_word(int index)
{
    int i;
    for (i = 0; i < name_table_size; i++) {
        //printf("\nNames[%i].id IS: %d\n", i,  Names[i].id);
        if (Names[i].id == index) {
            //printf("p->literal_name = %s", Names[i].literal_name);
            return Names[i].literal_name;
        }
    }
    return NULL;

}

struct rule {
    int id_rule;
    int L;
    int R[10];
    int condition_symbol[20]; //= {-1};
    struct rule *next;
};

struct relation {
    int relation_name;
    int count_args;
    int args[30];
    struct relation *next;
};

struct sentence {
    struct relation *head;
    //struct relation *body;
    struct sentence *next;
};

struct relation *head_relation_spisok;
struct sentence *head_sentence_spisok;


struct relation *create_relation(int arg)
{
    struct relation *relation = malloc(sizeof(struct relation));
    relation->count_args = 1;
    relation->args[0] = arg;
    relation->args[1] = 0;
    //printf("\narg is %i\n", arg);
    relation->next = NULL;
    return relation;
}

struct relation *create_relation_and_add_to_spisok(int arg)
{
    //printf("create_relation_and_add_to_spisok\n");

    //printf("before print_relation_spisok\n");
    //print_relation_spisok(); //

    struct relation *p = head_relation_spisok;
    //printf("after\n");
    while (p->next != NULL) {
        //printf("%d\n", p->count_args);
        p = p->next;
    }


    p->next = create_relation(arg);

    //printf("after print_relation_spisok\n");
    //print_relation_spisok();//
    //printf("\n");

    return p->next;
}

struct relation *add_arg_to_relation(int arg)
{
    //printf("add_arg_to_relation\n");
    struct relation *p = head_relation_spisok;
    while (p->next != NULL)
        p = p->next;

    //printf("before print_relation_spisok\n");
    //print_relation_spisok(); //

    p->count_args++;
    p->args[p->count_args - 1] = arg;
    p->args[p->count_args] = 0;
    //p->next = NULL;

    //printf("after print_relation_spisok\n");
    //print_relation_spisok();//
    //printf("\n");
    return p;
}

struct sentence *create_sentence_and_add_to_spisok(int name1, int name2)
{
    //printf("create_sentence_and_add_to_spisok\n");
    //printf("name1: %s name2 = %s\n", get_literal_word(name1), get_literal_word(name2));
    struct sentence *p = head_sentence_spisok;
    while (p->next != NULL)
        p = p->next;

    //printf("before add\n");
    //print_sentence_spisok();//
    //printf("doing print_relation_spisok\n");
    //print_relation_spisok();
    struct sentence *sentence = malloc(sizeof(struct sentence));

    if (name1 != 0)
        head_relation_spisok->next->relation_name = name1;
    if (name2 != 0)
        head_relation_spisok->next->next->relation_name = name2;

    //printf("after adding names\n");
    //print_relation_spisok();//

    sentence->head = head_relation_spisok->next;

    //sentence->body = head_relation_spisok->next->next;
    head_relation_spisok->next = NULL;
    p->next = sentence;
    sentence->next = NULL;
    //printf("after add\n");
    //print_sentence_spisok();//
    //printf("doing print_relation_spisok\n");
    //print_relation_spisok();//

    return sentence;
}

struct relation *add_relation_to_last_sentense(int relation_name)
{
    //printf("add_relation_to_last_sentense\n");
    struct sentence *h = head_sentence_spisok;
    while (h->next != NULL)
        h = h->next;

    struct relation *p = h->head;
    while (p->next != NULL)
        p = p->next;

    p->next = head_relation_spisok->next;
    head_relation_spisok->next = NULL;
    p->next->relation_name = relation_name;
    return p->next;
}


int get_len_rule_R(int *R)
{
    int i = 0;
    while (R[i] != 0) {
        i++;
    }
    return i;
}

int is_symbol_not_a_condition_symbol(int main_i, int rule_len_R, int *condition_symbol)
{
    int i = 0;
    //printf("main_i+rule_len_R = %d main_mass_size = %d\n", main_i+rule_len_R, main_mass_size);
    if (main_i + rule_len_R >= main_mass_size)
        return 0;
    int symbol = main_mass[main_i + rule_len_R];
    for (i = 0; i < 20; i++) {
        if (condition_symbol[i] == symbol)
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
        main_mass_copy[left] = main_mass_copy[left + size];
        left++;

    }
    main_mass[left] = main_mass[left + size];
    main_mass_copy[left] = main_mass_copy[left + size];
    left++;
    main_mass_size = main_mass_size - size;
    return 0;
}


int print_main_mass()
{
    stack_len+=2;
    printf("main_mass_size = %d\n", stack_len);
    for (int i = 0; i < stack_len; i++) {
        if (get_key_word(main_mass[i]) > 0) {
            printf("%c", get_key_word(main_mass[i]));

        } else {
            printf("%s ", get_lex_string(main_mass[i]));
        }
        //printf("(%2d) ", main_mass[i]); //вывод кода каждого спец-символа(лексемы)
    }
    printf("\t");
    return 0;
}

int print_main_mass_copy()
{
    printf("main_mass_size = %d\n", main_mass_size);
    for (int i = 0; i < main_mass_size; i++) {
        if (get_literal_word(main_mass_copy[i]) != NULL) {
            printf("%s ", get_literal_word(main_mass_copy[i]));
        } else if (get_key_word(main_mass[i]) > 0) {
            printf("%c", get_key_word(main_mass[i]));
        }  else {
            printf("%s ", get_lex_string(main_mass[i]));
            //вывод кода каждого спец-символа(лексемы)
        }
    }
    printf("  ");
    return 0;
}


void print_relation_spisok()
{
    struct relation *p = head_relation_spisok->next;
    printf("print_relation_spisok\n");
    while (p != NULL) {
        printf("relation_name = %d  count_args = %d args: ", p->relation_name, p->count_args);
        for (int i = 0; i < p->count_args; i++) {
            printf("%s, ", get_literal_word(p->args[i]));
        }
        printf("\n");
        p = p->next;
    }
    //printf("\n");
}

void print_sentence_spisok()
{
    struct sentence *p = head_sentence_spisok->next;
    printf("\nprint_sentence_spisok:\n");

    while (p != NULL) {
        printf("Head:\n");
        struct relation *q = p->head;
        printf("relation_name = %s, count_args = %d, args: ", get_literal_word(q->relation_name),
               q->count_args);
        //printf("%s(", get_literal_word(q->relation_name));
        for (int i = 0; i < q->count_args; i++) {
            if (i)
                printf(", %s", get_literal_word(q->args[i]));
            if (!i)
                printf("%s", get_literal_word(q->args[i]));
        }
        printf("\n");
        q = q->next;
        if (q != NULL)
            printf("Body:\n");

        while (q != NULL) {
            printf("relation_name = %s, count_args = %d, args: ", get_literal_word(q->relation_name),
                   q->count_args);
            //printf("%s(", get_literal_word(q->relation_name));
            for (int i = 0; i < q->count_args; i++) {
                if (i)
                    printf(", %s", get_literal_word(q->args[i]));
                if (!i)
                    printf("%s", get_literal_word(q->args[i]));
            }
            printf("\n");
            q = q->next;
        }
        printf("\n");
        //printf("head = %d", p->head->relation_name);
        p = p->next;
    }

    printf("\n");
}

int main()
{
    head_relation_spisok = malloc(sizeof(head_relation_spisok));
    print_relation_spisok();
    head_sentence_spisok = malloc(sizeof(head_sentence_spisok));
    head_relation_spisok->next = NULL;
    head_sentence_spisok->head = NULL;
    head_sentence_spisok->next = NULL;
    print_relation_spisok();

    //Names = malloc(sizeof(struct literal)):

    FILE *file_mass;
    if ((file_mass = fopen("mainmass.txt", "r")) == NULL) {
        printf("Can't open file mainmass.txt\n");
        return 0;
    }
    int elem;
    char str[30];
    print_relation_spisok();

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

    head_relation_spisok->next = NULL;

    while ((fscanf(name_table, "%d %d %d %s %s %d\n", &elem, &elem, &elem, str, str, &elem) != EOF))
        name_table_size++;
    fseek(name_table, 0, SEEK_SET);
    Names = (struct literal *)malloc(sizeof(struct literal) * name_table_size);
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
    //puts("388");
    //print_relation_spisok();
    //return 0;

    struct rule PROGR__MOP = {//PROGR->MOP
        9, PROGR, {MOP, is_key_word('#')}, {0}, NULL
    };
    struct rule MOP__MOP_P = {//MOP->MOP P
        8, MOP, {MOP, P}, {0}, &PROGR__MOP
    };
    struct rule MOP__P = {//MOP->P
        7, MOP, {P }, {0}, &MOP__MOP_P
    };
    struct rule P__a_BRACKET_ARG_BRACKET_POINT = {//P->a(ARG).
        6, P, {a, is_key_word('('), ARG, is_key_word(')'), is_key_word('.')}, {0}, &MOP__P
    };

    struct rule P__Q_POINT = {//P->Q.
        5, P, {Q, is_key_word('.')}, {0}, &P__a_BRACKET_ARG_BRACKET_POINT
    };

    struct rule Q__Q_COMMA_a__BRACKET_ARG_BRACKET = {//Q->Q,a(ARG)
        4, Q, {Q, is_key_word(','), a, is_key_word('('), ARG, is_key_word(')'), 0}, {0}, &P__Q_POINT
    };

    struct rule Q__a_BRACKET_ARG_BRACKET_COLON_DASH_a_BRACKET_ARG_BRACKET = {//Q->a(ARG):-a(ARG)
        3, Q,
        {a, is_key_word('('), ARG, is_key_word(')'), is_key_word(':'), is_key_word('-'), a, is_key_word('('), ARG, is_key_word(')')},
        {0},
        &Q__Q_COMMA_a__BRACKET_ARG_BRACKET
    };
    struct rule ARG__a = {//ARG->a
        2, ARG, {a, 0}, {is_key_word('('), 0}, &Q__a_BRACKET_ARG_BRACKET_COLON_DASH_a_BRACKET_ARG_BRACKET
    };
    struct rule ARG__ARG_COMMA_a = {//ARG->ARG,a
        1, ARG, {ARG, is_key_word(','), a, 0}, {0}, &ARG__a
    };


    //print_main_mass();
    stack_len = main_mass_size;
    memcpy(main_mass_copy, main_mass, sizeof(main_mass));
    for (int i = 0; i < main_mass_size; i++) {
        if (main_mass[i] > 0) {
            main_mass[i] = a;
        }
    }
    printf("print_main_mass_copy\n");
    //print_main_mass_copy();
    printf("print_main_mass\n");
    print_main_mass();
    //getchar();
    main_mass[main_mass_size] = 0;
    int main_i, rule_len_R, i = 0, j;
    struct rule *head = &ARG__ARG_COMMA_a;
    struct rule *p = head;
    main_i = 0;

    printf("start relation spisok\n");
    print_relation_spisok();
    //return 0;

    while (main_i <= main_mass_size) {
        p = head;
        while (p != NULL) {
            int flag = 0;
            int *R = p->R;

            if (R == NULL) {
                printf("NULL\n");
                return 0;
            }

            rule_len_R = get_len_rule_R(p->R);
            
            j = main_i;

            for (i = 0; i < rule_len_R; i++, j++) {
                if (main_mass[j] != R[i]) {
                    p = p->next;
                    flag = 1;//правило не подошло

                    break;
                }
            }


            if (flag == 0) { //правило подошло, заменяем R->L
                int current_symbol = main_mass[main_i + rule_len_R];
                if (is_symbol_not_a_condition_symbol(main_i, rule_len_R, p->condition_symbol)) {
                    p = p->next;
                    continue;
                }



                switch (p->id_rule) {
                    case 1:
                        //puts("case 1\n");

                        add_arg_to_relation(main_mass_copy[main_i + 2]);
                        break;

                    case 2:
                        //puts("case 2\n");
                        create_relation_and_add_to_spisok(main_mass_copy[main_i]);
                        break;

                    case 3:
                        //puts("case 3\n");
                        create_sentence_and_add_to_spisok(main_mass_copy[main_i], main_mass_copy[main_i + 6]);
                        break;

                    case 4:
                        //puts("case 4\n");
                        //printf("relation_name = ")
                        //add_relation_to_last_sentense(main_mass_copy[main_i + 2]);
                        add_relation_to_last_sentense(main_mass_copy[main_i + 2]);
                        break;

                    //case 5:
                    //create_relation_and_add_to_spisok(main_mass_copy[main_i]);
                    //create_sentence_and_add_to_spisok();

                    case 6:
                        //puts("case 6\n");
                        create_relation_and_add_to_spisok(main_mass_copy[main_i]);
                        create_sentence_and_add_to_spisok(main_mass_copy[main_i], 0);
                        break;

                }
                stack_len = main_i + rule_len_R-1;
                print_main_mass();
                
                //print_main_mass_copy();
                compress_main_massiv(main_i + 1, rule_len_R - 1);
                main_mass[main_i] = p->L;
                main_mass_copy[main_i] = p->L;
                stack_len = main_i;
                main_i = -1;
                //print_main_mass_copy();
                
                print_main_mass();
                
                printf("Done rule %d\n\n", p->id_rule);
            }
        }
        main_i++;
    }
    printf("END\n");
    print_main_mass();
    print_sentence_spisok();

    //printf("\nNames[0]->id is %s\n", Names[0].literal_name);

    /* вывод таблицы имен
    for(int i =0; i<name_table_size; i++){
        if (Names[i].type == DIGIT) {
            Names[i].value_char[0] = '\0';
            printf("%d \t%d \t%d \t%s %20d\n", Names[i].id, Names[i].type, Names[i].const_or_var,
                   Names[i].literal_name, Names[i].value_int);
        } else
            printf("%d \t%d \t%d \t%s %20s\n", Names[i].id, Names[i].type, Names[i].const_or_var,
                   Names[i].literal_name, Names[i].value_char);
    }*/

    if (main_mass_size != 1 || main_mass[0] != PROGR) {
        printf("ERROR Programm\n");
    } else {
        printf("Correct Programm\n");
    }




    return 0;
}
