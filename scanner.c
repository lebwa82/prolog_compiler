//todo ввод из файла, исправить чтение float
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "header.h"

int main_massiv_id[100];
int main_i;

char pre_stroka[100] = "nested_condition(Book):-sum(X,Y). read(Nina, article, qwerty).";
char stroka[100] = "";
//char stroka[100] = "nested_condition(\"mark\",Book):-relation(Tom,123,-22222).";
int stroka_i = -1;
int pre_stroka_i = -1;
int max_main_id;


char get_symbol()
{
    stroka_i++;
    if (stroka[stroka_i] != '\0') {
        //printf("i = %c\n", stroka[stroka_i]);
        return stroka[stroka_i];
    }
    return 0;
}

char pre_get_symbol()
{
    pre_stroka_i++;
    if (pre_stroka[pre_stroka_i] != '\0') {
        //printf("i = %c\n", stroka[stroka_i]);
        return pre_stroka[pre_stroka_i];
    }
    return 0;
}

char step_back()
{
    stroka_i--;
    return 0;
}

struct literal *head;

int main()
{
    printf("len_key_words_list = %d\n", len_key_words_list);
    head = malloc(sizeof(struct literal));
    head->id = 0;
    head->next = NULL;

    int i_massiv;
    int count_bracket = 0;
    char symbol;
    int name_id = 1, key_id = -1;
    main_i = 0, max_main_id = 0;
    int i = 0, id = 0;
    int word_i = 0;
    char word[30];
    enum type type;//тип
    enum const_or_var const_or_var;//вид
    float number = 0;



    //удаление комментариев и пробелов
    stroka_i = 0;
    pre_stroka_i = -1;
PRE_AUTOMAT: {
        symbol = pre_get_symbol();
        //printf("pre_symbol = %c\n", symbol);
        switch (symbol) {
            case '%':
                goto ONELINE_COMMENT;
            case '/':
                goto PREDICT_MULTILINE_COMMENT;
            case ' ':
                goto  PRE_AUTOMAT;
            case '\0':
                stroka[stroka_i] = '\0';
                printf("PRE_AUTOMAT complete\n");
                goto PRE_START;

            default:
                stroka[stroka_i] = symbol;
                stroka_i++;
                goto  PRE_AUTOMAT;
        }
    }     

ONELINE_COMMENT:
    symbol = pre_get_symbol();
    switch (symbol) {
        case '\n':
            goto PRE_AUTOMAT;
        //case 'EOF':
        //    goto PRE_START;
        case '\0':
            printf("ONELINE_COMMENT ERROR\n");
            goto ERROR;

        default:
            goto ONELINE_COMMENT;
    }

PREDICT_MULTILINE_COMMENT: {
        symbol = pre_get_symbol();
        switch (symbol) {
            case '*':
                goto MULTILINE_COMMENT;

            default:
                stroka[stroka_i] = '/';
                stroka_i++;
                goto PRE_AUTOMAT;
        }
    }

MULTILINE_COMMENT: {
        symbol = pre_get_symbol();
        switch (symbol) {
            case '*':
                goto PREDICT_END_MULTILINE_COMMENT;
            
            case '\0':
                printf("MULTILINE_COMMENT ERROR\n");
                goto ERROR;

            default:
                goto MULTILINE_COMMENT;
        }
    }
PREDICT_END_MULTILINE_COMMENT:{
        symbol = pre_get_symbol();
        switch (symbol) {
            case '/':
                goto PRE_AUTOMAT;

            default:
                goto MULTILINE_COMMENT;
        }
    }


PRE_START:
    //считаем, что избавились от пробелов и комментариев
    stroka_i = -1;
    printf("stroka = %s\n", stroka);
    //return 0;
START:
    //stroka[stroka_i] = '\0';
    //return 0;
    symbol = get_symbol();
    //printf("symbol = %c\n", symbol);
    //getchar();
    if (isalpha(symbol)) {
        step_back();
        goto GET_WORD;
    }
    if (isdigit(symbol)) {
        step_back();
        goto GET_NUMBER;
    }
    id = is_key_word(symbol);
    printf("key_symbol = %c id = %d\n", symbol, id);
    if (id != 0) {
        main_massiv_id[main_i] = id;
        main_i++;
        goto START;
    }
    if (symbol == '"')
        goto GET_STRING;

    if (symbol == '\0')
        goto COMPLETE;

    goto ERROR;



GET_WORD: {
        symbol = get_symbol();
        if (isalpha(symbol) || isdigit(symbol) || symbol == '_') {
            word[word_i] = symbol; word_i++;
            goto GET_WORD;
        }
        word[word_i] = '\0';
        printf("word = %s ", word);
        word_i = 0;
        step_back();
        id = append_word_in_literals(word, VAR_TYPE, VAR);
        printf("id = %d\n", id);
        main_massiv_id[main_i] = id;
        main_i++;
        goto START;
    }


GET_NUMBER: {
        symbol = get_symbol();

        if (isdigit(symbol) || number == 0) {
            number = number * 10 + (symbol - '0');
            //printf("\nNUMBER = %f\n",number);
            goto GET_NUMBER;
        }
        step_back();
        char word[30];
        sprintf(word, "%f", number);
        id = append_word_in_literals(word, DIGIT, CONSTANTA);
        main_massiv_id[main_i] = id;
        main_i++;
        printf("DIGIT = %f id = %d\n", number, id);
        number = 0;
        goto START;
    }

GET_STRING: {
        char symbol = get_symbol();

        if (isalpha(symbol) || isdigit(symbol) || symbol == '_') {
            word[word_i] = symbol; word_i++;
            goto GET_STRING;
        }
        //printf("\nsymbol is: %c\n", symbol);
        if (symbol != '"') {

            goto ERROR;
        }

        word[word_i] = '\0';
        word_i = 0;
        id = append_word_in_literals(word, STRING, CONSTANTA);
        printf("STRING = \"%s\" id = %d\n", word, id);
        main_massiv_id[main_i] = id;
        main_i++;
        goto START;
    }


COMPLETE:
    puts("yes");
    print_main_massiv();
    print_literal();
    
    return 0;
ERROR:
    puts("ERROR");
    return 0;
}