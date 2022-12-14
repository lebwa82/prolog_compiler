/*----------------------------------------------------------------------------------------------
Процедура синтаксически управляемого перевода, создающая и заполняющая список внутренних структур
из входного массива.
------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "header.h"

void print_sentence_spisok();
void print_relation_spisok();

FILE *output_file, *log_file;
int main_mass[100];//стек хранящий коды лексем
int main_mass_copy[100];
int main_mass_size = 0;
int name_table_size = 0;
struct literal *Names;//голова списка имен
int stack_len;//длина стека

enum lexema {//лексемы
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

typedef struct
    lex_words {//стктура сопоставляющая код лексемы ее код
    int index;
    char word[10];
} lex_words;

lex_words lexema_string[] = {{PROGR, "PROGR"}, {MOP, "MOP"}, {P, "P"}, {Q, "Q"},
    {S, "S"}, {E, "E"}, {T, "T"}, {F, "F"}, {a, "a"}, {ARG, "ARG"}
};

char *get_lex_string(int word)
//по коду лексемы получить строковое представление
{
    for (int i = 0; i < 10; i++) {
        if (lexema_string[i].index == word)
            return lexema_string[i].word;
    }
    return NULL;
}

int get_key_word(int index)
//по индексу ключевого слова получить строковое представление
{
    for (int i = 0; i < len_key_words_list; i++) {
        if (key_words_list[i].index == index)
            return key_words_list[i].word;
    }
    return 0;
}

char *get_literal_word(int index)
//по индексу литерала получить строковое представление
{
    int i;
    for (i = 0; i < name_table_size; i++) {
        if (Names[i].id == index) {
            return Names[i].literal_name;
        }
    }
    return NULL;

}

struct rule {//структура правила грамматики
    int id_rule;//идендитификатор
    int L;//левая часть правила
    int R[10];//правая часть правила
    int condition_symbol[20];//массив симоволов, при который выполняется правило
    struct rule *next;
};

struct relation {//структура отношения в PROLOG
    int relation_name;
    int count_args;
    int args[30];
    struct relation *next;
};

struct sentence {
    struct relation *head;
    //структура предложения в прологе.
    //Если в списке одно отношение - значит в предложении нет тела.
    //Если больше одного отношения - значит первое отношение - голова, отсальные - тело
    struct sentence *next;
};

struct relation *head_relation_spisok;
struct sentence *head_sentence_spisok;


struct relation *create_relation(int arg)
//создать отношение с переданным аргументом
{
    struct relation *relation = malloc(sizeof(struct relation));
    relation->count_args = 1;
    relation->args[0] = arg;
    relation->args[1] = 0;
    relation->next = NULL;
    return relation;
}

struct relation *create_relation_and_add_to_spisok(int arg)
{//создать отношение и добавить в конец списка
    struct relation *p = head_relation_spisok;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = create_relation(arg);
    return p->next;
}

struct relation *add_arg_to_relation(int arg)
{//добавить аргумент в последнее отношение списка отношений
    struct relation *p = head_relation_spisok;
    while (p->next != NULL)
        p = p->next;
        
    p->count_args++;
    p->args[p->count_args - 1] = arg;
    p->args[p->count_args] = 0;
    return p;
}

struct sentence *create_sentence_and_add_to_spisok(int name1, int name2)
{
    //создать предложение, добавить в него два отношения из списка отношений,
    //присвоим им имена name1 и name2 и добавить в конец списка предложений
    struct sentence *p = head_sentence_spisok;
    while (p->next != NULL)
        p = p->next;

    struct sentence *sentence = malloc(sizeof(struct sentence));
    if (name1 != 0)
        head_relation_spisok->next->relation_name = name1;
    if (name2 != 0)
        head_relation_spisok->next->next->relation_name = name2;
    sentence->head = head_relation_spisok->next;
    head_relation_spisok->next = NULL;
    p->next = sentence;
    sentence->next = NULL;
    return sentence;
}

struct relation *add_relation_to_last_sentense(int relation_name)
{
    //переместить последнее отношение из списка отношений в последнее предложение
    //из списка предложений, добавив ему relation_name
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
    //вычислить длину массива симоволов, при котором выполняется правило
    int i = 0;
    while (R[i] != 0) {
        i++;
    }
    return i;
}

int is_symbol_not_a_condition_symbol(int main_i, int rule_len_R, int *condition_symbol)
{//правоерка находится ли симовол в массиве симоволов, при которых выполняется правило
    int i = 0;
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
{//убрать из стека правую часть правила, и добавить левую часть правила

    if (left + size > main_mass_size) {
        printf("compress_main_massiv ERROR\n");
        fprintf(log_file, "compress_main_massiv ERROR\n");
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


int print_main_mass()//вывод стека
{
    stack_len += 2;
    //stack_len = main_mass_size;
    //printf("main_mass_size = %d\n", stack_len);
    //fprintf(log_file, "main_mass_size = %d\n", stack_len);
    
    for (int i = 0; i < stack_len; i++) {
        if (get_key_word(main_mass[i]) > 0) {
            printf("%c", get_key_word(main_mass[i]));
            fprintf(log_file, "%c", get_key_word(main_mass[i]));

        } else {
            printf("%s ", get_lex_string(main_mass[i]));
            fprintf(log_file, "%s ", get_lex_string(main_mass[i]));
        }
    }
    printf("\n");
    fprintf(log_file,"\n");
    return 0;
}

int print_main_mass_copy()
{
    //printf("main_mass_size = %d\n", main_mass_size);
    //fprintf(log_file, "main_mass_size = %d\n", main_mass_size);
    for (int i = 0; i < main_mass_size; i++) {
        if (get_literal_word(main_mass_copy[i]) != NULL) {
            printf("%s ", get_literal_word(main_mass_copy[i]));
            fprintf(log_file, "%s ", get_literal_word(main_mass_copy[i]));
        } else if (get_key_word(main_mass[i]) > 0) {
            printf("%c", get_key_word(main_mass[i]));
            fprintf(log_file, "%c", get_key_word(main_mass[i]));
        }  else {
            printf("%s ", get_lex_string(main_mass[i]));
            fprintf(log_file, "%s ", get_lex_string(main_mass[i]));
        }
    }
    
    printf("\n");
    fprintf(log_file, "\n");
    return 0;
}


void print_relation_spisok()//вывести список отношений
{
    struct relation *p = head_relation_spisok->next;
    while (p != NULL) {
        printf("relation_name = %d  count_args = %d args: ", p->relation_name, p->count_args);
        fprintf(log_file, "relation_name = %d  count_args = %d args: ", p->relation_name, p->count_args);
        for (int i = 0; i < p->count_args; i++) {
            printf("%s, ", get_literal_word(p->args[i]));
            fprintf(log_file,"%s, ", get_literal_word(p->args[i]));
        }
        printf("\n");
        fprintf(log_file,"\n");
        p = p->next;
    }
}

void print_sentence_spisok()//вывести список предложений
{
    struct sentence *p = head_sentence_spisok->next;
    printf("\nprint_sentence_spisok:\n");

    while (p != NULL) {
        printf("Head:\n");
        fprintf(output_file, "Head:\n");
        struct relation *q = p->head;
        printf("relation_name = %s, count_args = %d, args: ", get_literal_word(q->relation_name),
               q->count_args);
        fprintf(output_file, "relation_name = %s, count_args = %d, args: ",
                get_literal_word(q->relation_name),
                q->count_args);
                
        for (int i = 0; i < q->count_args; i++) {
            if (i) {
                printf(", %s", get_literal_word(q->args[i]));
                fprintf(output_file, ", %s", get_literal_word(q->args[i]));
            }
            if (!i) {
                printf("%s", get_literal_word(q->args[i]));
                fprintf(output_file, "%s", get_literal_word(q->args[i]));
            }
        }

        printf("\n");
        fprintf(output_file, "\n");
        q = q->next;
        if (q != NULL) {
            printf("Body:\n");
            fprintf(output_file, "Body:\n");
        }

        while (q != NULL) {
            printf("relation_name = %s, count_args = %d, args: ", get_literal_word(q->relation_name),
                   q->count_args);
            fprintf(output_file, "relation_name = %s, count_args = %d, args: ",
                    get_literal_word(q->relation_name),
                    q->count_args);
            for (int i = 0; i < q->count_args; i++) {
                if (i) {
                    printf(", %s", get_literal_word(q->args[i]));
                    fprintf(output_file, ", %s", get_literal_word(q->args[i]));
                }
                if (!i) {
                    printf("%s", get_literal_word(q->args[i]));
                    fprintf(output_file, "%s", get_literal_word(q->args[i]));
                }
            }

            printf("\n");
            fprintf(output_file, "\n");
            q = q->next;
        }

        printf("\n");
        fprintf(output_file, "\n");
        p = p->next;
    }
    printf("\n");
    fprintf(output_file, "\n");
}

int main()
{
    head_relation_spisok = malloc(sizeof(head_relation_spisok));
    head_sentence_spisok = malloc(sizeof(head_sentence_spisok));
    head_relation_spisok->next = NULL;
    head_sentence_spisok->head = NULL;
    head_sentence_spisok->next = NULL;
    FILE *file_mass;
    if ((file_mass = fopen("mainmass.txt", "r")) == NULL) {
        printf("Can't open file mainmass.txt\n");
        return 0;
    }
    if ((output_file = fopen("out_structs.txt", "w")) == NULL) {
        printf("Can't open file output.txt\n");
        return 0;
    }
    if ((log_file = fopen("translate.txt", "w")) == NULL) {
        printf("Can't open file translate.txt\n");
        return 0;
    }

    int elem;
    char str[30];
    while ((fscanf(file_mass, "%d", &elem) != EOF))
        main_mass_size++;
    fseek(file_mass, 0, SEEK_SET);
    for (int i = 0; i < main_mass_size; i++) {
        fscanf(file_mass, "%d", &main_mass[i]);
        printf("%2d ", main_mass[i]);
    }
    FILE *name_table;
    if ((name_table = fopen("name_table.txt", "r")) == NULL) {
        printf("Can't open file name_table.txt\n");
        return 0;
    }

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
        Names[i].const_or_var = const_or_var;
        strcpy(Names[i].literal_name, literal_name);
        strcpy(Names[i].value_char, value_char);
        Names[i].value_int = value_int;
        if (Names[i].type == DIGIT) {
            Names[i].value_char[0] = '\0';
            printf("%d \t%d \t%d \t%s %20d\n", Names[i].id, Names[i].type, Names[i].const_or_var,
                   Names[i].literal_name, Names[i].value_int);
        } else{
            printf("%d \t%d \t%d \t%s %20s\n", Names[i].id, Names[i].type, Names[i].const_or_var,
                   Names[i].literal_name, Names[i].value_char);
        }
    }

    //Определение используемых в программе правил 
    //(из official_grammatics.txt) в виде элементов списка 
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


    stack_len = main_mass_size;
    memcpy(main_mass_copy, main_mass, sizeof(main_mass));
    for (int i = 0; i < main_mass_size; i++) {
        if (main_mass[i] > 0) {
            main_mass[i] = a;
        }
    }

    main_mass[main_mass_size] = 0;
    int main_i, rule_len_R, i = 0, j;
    struct rule *head = &ARG__ARG_COMMA_a;
    struct rule *p = head;
    main_i = 0;
    while (main_i <= main_mass_size) {
        p = head;
        while (p != NULL) {
            int flag = 0;
            int *R = p->R;

            if (R == NULL) {
                printf("NULL\n");
                fprintf(log_file,"NULL\n");
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

                        add_arg_to_relation(main_mass_copy[main_i + 2]);
                        break;
                    case 2:
                        create_relation_and_add_to_spisok(main_mass_copy[main_i]);
                        break;
                    case 3:
                        create_sentence_and_add_to_spisok(main_mass_copy[main_i], main_mass_copy[main_i + 6]);
                        break;
                    case 4:
                        add_relation_to_last_sentense(main_mass_copy[main_i + 2]);
                        break;
                    case 5:
                        break;
                    case 6:
                        create_sentence_and_add_to_spisok(main_mass_copy[main_i], 0);
                        break;
                }

                stack_len = main_i + rule_len_R - 1;
                print_main_mass();
                compress_main_massiv(main_i + 1, rule_len_R - 1);
                main_mass[main_i] = p->L;
                main_mass_copy[main_i] = p->L;
                stack_len = main_i;
                main_i = -1;
                print_main_mass();
                printf("  Done rule %d\n", p->id_rule);
                fprintf(log_file,"Done rule %d\n\n", p->id_rule);

            }
        }
        main_i++;
    }

    printf("can't collapse the rules anymore. END\n");
    fprintf(log_file,"can't collapse the rules anymore. END\n");
    print_main_mass();
    print_sentence_spisok();
    if (main_mass_size != 1 || main_mass[0] != PROGR) {
        printf("ERROR Programm\n");
        fprintf(log_file,"ERROR Programm\n");
    } else {
        printf("Correct Programm\n");
        fprintf(log_file,"Correct Programm\n");
    }
    return 0;
}
