/*----------------------------------------------------------------------------------------------
Сканер-автомат, создающий массив лексем из входного потока данных
Дмитрий Парфёнов, Михаил Авшалумов, Александр Волков. ИНБИКСТ, Б07-903. 2022 год
------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "header.h"

//Входная строка с программой
char stroka[200] =
    "nested_condition(Book):-sum(33,Y,44,555,ABC),asoka(S,fdesf).second_sentense(f1):-sss(f2,f3),sochi(f4).#";

//Входной алфавит сканера
char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
char digit[] = "0123456789";
char CAV[] = "\"";
char hash[] = "#";
char key_word_massiv[] = "+-=*/<>[]:;,|().";

//Проверка наличия символа во входном алфавите
int is_symbol_in_massiv(char symbol, char *p)
{
    int i = 0;
    for (i = 0; i < strlen(p); i++) {
        if (symbol == p[i]) {
            return 1;
        }
    }
    return 0;
}

//Список состояний
enum states {
    START,
    GET_WORD,
    GET_NUMBER,
    GET_STRING,
    COMPLETE,
    ERROR
};
//Список процедур
enum proc {
    step_back = 1,
    get_word,
    get_digit,
    get_key_word,
    end_of_word_with_keyword,
    end_of_word_with_hash,
    end_of_digit_with_keyword,
    end_string,
    error,
    complete,
};

struct categories {
    int q1, q2;
    char *condition_symbol;
    int procedure;
};

//Сутевая часть автомата
struct categories rules[] = {
    {START, GET_WORD, alphabet, get_word},
    {START, GET_NUMBER, digit, get_digit},
    {START, START, key_word_massiv, get_key_word},
    {START, GET_STRING, CAV, 0},
    {START, COMPLETE, hash, complete},

    {GET_WORD, GET_WORD, alphabet, get_word},
    {GET_WORD, GET_WORD, digit, get_word},
    {GET_WORD, START, key_word_massiv, end_of_word_with_keyword},
    {GET_WORD, ERROR, hash, error},

    {GET_NUMBER, GET_NUMBER, digit, get_digit},
    {GET_NUMBER, ERROR, alphabet, error},
    {GET_NUMBER, START, key_word_massiv, end_of_word_with_keyword},
    {GET_NUMBER, ERROR, hash, error},

    {GET_STRING, GET_STRING, alphabet, get_word},
    {GET_STRING, GET_STRING, digit, get_word},
    {GET_STRING, START, CAV, end_string},
    {GET_STRING, ERROR, hash, error},
};

int stroka_i; //итератор для входного потока данных
int pre_stroka_i;
int max_main_id; //итоговый размер выходного массива

void get_text_from_file()//прочитать текст из файла 
{
    int i = 0;
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        printf("No file input.txt");
        exit(1);
    }
    char c;
    while ((c = fgetc(file)) != EOF) {
        //char c = fgetc(file);
        //printf("c = %c\n", c);
        //getchar();
        if (c!= '\n' && c != ' ')
        {
            stroka[i] = c;
            i++;
        }
    }
    stroka[i] = '#';
}


char get_symbol()//взять следующий символ
{
    stroka_i++;
    if (stroka[stroka_i] != '\0') {
        return stroka[stroka_i];
    }
    return 0;
}


int main()
{
    get_text_from_file();
    enum proc procedure = error;
    char word[30];
    int word_i = 0, i = 0;
    char symbol;
    int id;
    main_i = 0; //итератор выходного массива
    stroka_i = -1;
    pre_stroka_i = 0;
    max_main_id = 0;
    int flag = 0;
    int len_rules = sizeof(rules) / sizeof(rules[0]);
    int q = START;
    head_literals = malloc(sizeof(struct literal));
    head_literals->next = NULL;

    //Запуск автомата, поиск нужного состояния
    while (flag != 1) {
        symbol = get_symbol();
        for (i = 0; i < len_rules; i++) {
            if (q == rules[i].q1 && is_symbol_in_massiv(symbol, rules[i].condition_symbol) == 1) {
                q = rules[i].q2;
                procedure = rules[i].procedure;
                break;
            }
        }

        //выбор необходимой процедуры
        switch (procedure) {
            case 0:
                break;

            case get_key_word:
                main_massiv_id[main_i] = is_key_word(symbol);
                main_i++;
                break;

            case get_word:
                word[word_i] = symbol;
                word_i++;
                word[word_i] = '\0';
                break;

            case end_of_word_with_keyword:
                word_i = 0;
                id = append_word_in_literals(word, NAME, VAR);
                printf("id = %d\n", id);
                main_massiv_id[main_i] = id;
                main_i++;

                main_massiv_id[main_i] = is_key_word(symbol);
                main_i++;
                break;

            case get_digit:
                word[word_i] = symbol;
                word_i++;
                word[word_i] = '\0';
                break;

            case end_of_digit_with_keyword:
                word_i = 0;
                id = append_word_in_literals(word, DIGIT, CONSTANTA);
                printf("id = %d\n", id);
                main_massiv_id[main_i] = id;
                main_i++;

                main_massiv_id[main_i] = is_key_word(symbol);
                main_i++;
                break;

            case end_string:
                word_i = 0;
                id = append_word_in_literals(word, STRING, CONSTANTA);
                printf("id = %d\n", id);
                main_massiv_id[main_i] = id;
                main_i++;
                break;

            case error:
                puts("ERROR");
                flag = 1;
                break;

            case complete:
                puts("COMPLETE");
                main_massiv_id[main_i] = is_key_word(symbol);
                main_i++;
                print_main_massiv();
                print_literal();
                flag = 1;
                break;
        }
    }
}