#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum vid {
    VAR,
    CONST,
};

enum type {
    INT,
    FLOAT,
    STRING,
    //ATOM1,
    SPISOK,
    STRUCTURA,
};

typedef struct key_words {
    int index;
    char word[10];
} key_words;

key_words key_words_list[] = {{-1, "+"}, {-2, "-"}, {-3, "="}, {-4, "*"},
    {-5, "/"}, {-6, "<"}, {-7, ">"}, {-8, "["},
    {-9, "]"}, {-10, ":"}, {-11, ";"}, {-12, ","},
    {-13, "|"}, {-14, "/"}, {-15, ":-"}
};

struct names {
    int index;
    char name[10];
    int vid;
    int type;
};

int is_key_word(char *word)
{
    return 0;
}

char stroka[] = "rty(abc,def";
char *i = stroka - 1;
char get_symbol()
{
    i++;
    if (*i != '\0') {
        printf("i = %c\n", *i);
        return *i;
    }
    return 0;
}

char *get_word(char *s)
{
    char symbol = *s;
    char *word = malloc(30 * sizeof(char));
    strcat(word, &symbol);
    symbol = get_symbol();
    while (isalpha(symbol) || isdigit(symbol) || symbol == '_') {
        strcat(word, &symbol);
        symbol = get_symbol();
    }
    *s = symbol;
    return word;
}

int main()
{
    char symbol = 0;
    //char word[30];

START: {
        symbol = get_symbol();
        switch (symbol) {
            case '%':
                goto ONELINE_COMMENT;
            case '/':
                goto PREDICT_MULTILINE_COMMENT;
            case '\0':
                printf("Programm complete\n");
                return 0;
        }
        printf("symbol = %c\n", symbol);
        if (isalpha(symbol)) {
            goto WORD;
        }

        printf("START ERROR\n");
        goto ERROR;
    }


ONELINE_COMMENT: {
        symbol = get_symbol();
        switch (symbol) {
            case '\n':
                goto START;
            default:

                goto ONELINE_COMMENT;
        }
    }

PREDICT_MULTILINE_COMMENT: {
        symbol = get_symbol();
        switch (symbol) {
            case '*':
                goto MULTILINE_COMMENT;

            default:
                printf("PREDICT_MULTILINE_COMMENT ERROR\n");
                goto ERROR;
        }
    }
MULTILINE_COMMENT: {
        symbol = get_symbol();
        switch (symbol) {
            case '/':
                goto START;

            default:
                printf("MULTILINE_COMMENT ERROR\n");
                goto ERROR;
        }
    }
WORD: {
        char *word = get_word(&symbol);
        if (is_key_word(word)) {
            //что-то с ключевым словом
        } else {
            //занести в таблицу имен
            if (symbol == '(') {
                free(word);
                goto PREDICT_RELATION_OR_FACT_FIRST_NAME;
            } else {
                free(word);
                printf("WORD ERROR\n");
                goto ERROR;
            }
        }
    }


PREDICT_RELATION_OR_FACT_FIRST_NAME: {
        symbol = get_symbol();
        if (!isalpha(symbol)) {
            printf("PREDICT_RELATION_OR_FACT_FIRST_NAME ERROR\n");
            goto ERROR;
        }
        char *word = get_word(&symbol);
        switch (symbol) {
            case ',':
                goto PREDICT_RELATION_SECOND_NAME;
            case ')':
                printf("Fact complete\n");
                goto START;

            default:
                printf("PREDICT_RELATION_OR_FACT_FIRST_NAME ERROR\n");
                goto ERROR;
        }
    }
PREDICT_RELATION_SECOND_NAME: {
        symbol = get_symbol();
        if (symbol == '"') {
            goto PREDICT_RELATION_SECOND_NAME_AS_STRING;
        }
        if (isalpha(symbol)) {
            goto PREDICT_RELATION_SECOND_NAME_AS_VARIABLE;
        }
        printf("PREDICT_RELATION_SECOND_NAME ERROR\n");
        goto ERROR;

    }

PREDICT_RELATION_SECOND_NAME_AS_STRING: {
        symbol = get_symbol();
        if (!isalpha(symbol)) {
            printf("PREDICT_RELATION_SECOND_NAME_AS_STRING ERROR\n");
            goto ERROR;
        }
        char *word = get_word(&symbol);
        if (symbol != '"') {
            printf("PREDICT_RELATION_SECOND_NAME_AS_STRING ERROR\n");
            goto ERROR;
        }
        goto PREDICT_RELATION_COMPLETE_PREDICT_BRACKET;
    }


PREDICT_RELATION_SECOND_NAME_AS_VARIABLE: {
        char *word = get_word(&symbol);
        goto PREDICT_RELATION_COMPLETE_PREDICT_BRACKET;
    }

PREDICT_RELATION_COMPLETE_PREDICT_BRACKET: {
        if (symbol == ')') {
            printf("RELATION complete\n");
            goto START;
        }
        printf("PREDICT_RELATION_COMPLETE_PREDICT_BRACKET ERROR\n");
        goto ERROR;

    }


ERROR:
    printf("ERROR\n");
    return 0;


DIGIT: {
        int int_number = 0;
        while (isdigit(symbol) || int_number == 0) {
            int_number = int_number * 10 + (symbol - '0');
            symbol = get_symbol();
        }
        if (symbol == '.')
            goto FLOAT;
        else {}
        //что-то еще

FLOAT:
        float float_number = int_number;
        float tmp = 10;
        while (isdigit(symbol) || int_number == 0) {
            float_number += (symbol - '0') / tmp;
            tmp *= 10;
            symbol = get_symbol();
        }
    }


    return 0;
}

//комментарии для души
//isdigit
//isalpha




