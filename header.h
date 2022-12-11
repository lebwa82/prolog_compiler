#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


enum type { //тип, положительный
    DIGIT,
    STRING,
    NAME,
    VAR_TYPE,
};

enum const_or_var {//вид
    CONSTANTA,
    VAR,
};

struct literal { //может как ключевое слово, так и литералы
    int id;
    enum type type;//тип
    enum const_or_var const_or_var;//вид

    char literal_name[30];//изначальное имя
    char value_char[30]; //значение переменной
    int value_int;
    struct literal *next;
};

typedef struct key_words {
    int index;
    char word;
} key_words;

key_words key_words_list[] = {{-1, '+'}, {-2, '-'}, {-3, '='}, {-4, '*'},
    {-5, '/'}, {-6, '<'}, {-7, '>'}, {-8, '['},
    {-9, ']'}, {-10, ':'}, {-11, ';'}, {-12, ','},
    {-13, '|'}, {-14, '/'}, {-15, '('}, {-16, ')'}, {-17, '.'}, {-18, '#'},  //{-15, ":-"}, (-10,-2 = if)
};


int len_key_words_list = sizeof(key_words_list) / sizeof(key_words_list[0]);
int is_key_word(char word)
{
    for (int i = 0; i < len_key_words_list; i++) {
        //printf("%d %c\n", key_words_list[i].index, key_words_list[i].word);
        if (key_words_list[i].word == word)
            return key_words_list[i].index;
    }
    return 0;
}