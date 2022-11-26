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

char stroka[] = "read(mark, Book)";
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

char step_back()
{
    i--;
    return 0;
}    

char *get_word()
{
    //char symbol = *s;
    char *word = malloc(30 * sizeof(char));
    //strcat(word, &symbol);
    char symbol = get_symbol();
    while (isalpha(symbol) || isdigit(symbol) || symbol == '_') {
        strcat(word, &symbol);
        symbol = get_symbol();
    }
    step_back();
    return word;
}

FILE *f;
char get_word_f()
{
    char c = fgetc(f);
    if (c != '\0') {
        printf("c = %c\n", c);
        return c;
    }
    fclose(f);
    return 0;
}
 


int main()
{
    //f = fopen("input.txt", "r");
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
            //case 'EOF':
            //    goto START;
            case '\0':
                printf("ONELINE_COMMENT ERROR\n");
                goto ERROR;

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
        step_back();
        char *word = get_word();
        symbol = get_symbol();
        if (is_key_word(word)) {
            //что-то с ключевым словом
        } else {
            //занести в таблицу имен
            if (symbol == '(') {
                free(word);
                goto PREDICT_RELATION_OR_FACT_FIRST_VARIABLE_OR_STRING;
            } else {
                free(word);
                printf("WORD ERROR\n");
                goto ERROR;
            }
        }
    }

PREDICT_RELATION_OR_FACT_FIRST_VARIABLE_OR_STRING: {
        
        symbol = get_symbol();
        if(symbol == '"')
        {
            goto DEFINITELY_FACT_SOLE_STRING;
        }
        if(isalpha(symbol))
        {
            step_back();
            goto PREDICT_RELATION_OR_FACT_FIRST_VARIABLE;
        }
        printf("PREDICT_RELATION_OR_FACT_FIRST_VARIABLE_OR_STRING ERROR\n");
        goto ERROR;
    }

DEFINITELY_FACT_SOLE_STRING: {
    step_back();
    char *word = get_word();
        switch (symbol) {
            case '"':
                goto PREDICT_RELATION_COMPLETE_PREDICT_BRACKET_AND_POINT;

            default:
                printf("DEFINITELY_FACT_SOLE_STRING ERROR\n");
                goto ERROR;
        }        
}

PREDICT_RELATION_OR_FACT_FIRST_VARIABLE: {
    step_back();
    char *word = get_word();
    switch (symbol) {
        case ',':
            goto PREDICT_RELATION_SECOND_NAME;
        case ')':
            goto PREDICT_POINT;

        default:
            printf("PREDICT_RELATION_OR_FACT_FIRST_VARIABLE ERROR\n");
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
        step_back();
        char *word = get_word();
        if (symbol != '"') {
            printf("PREDICT_RELATION_SECOND_NAME_AS_STRING ERROR\n");
            goto ERROR;
        }
        step_back();
        goto PREDICT_RELATION_COMPLETE_PREDICT_BRACKET_AND_POINT;
    }


PREDICT_RELATION_SECOND_NAME_AS_VARIABLE: {
        step_back();
        char *word = get_word();
        step_back();
        goto PREDICT_RELATION_COMPLETE_PREDICT_BRACKET_AND_POINT;
    }

PREDICT_RELATION_COMPLETE_PREDICT_BRACKET_AND_POINT: {
        symbol = get_symbol();
        if (symbol == ')') {
            goto PREDICT_POINT;
        }
        printf("PREDICT_RELATION_COMPLETE_PREDICT_BRACKET ERROR\n");
        goto ERROR;
    }

PREDICT_POINT: {
    symbol = get_symbol();
    if (symbol == '.') {
        printf("Sentence complete\n");
        goto START;
    }
    printf("PREDICT_POINT ERROR\n");
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

VARIABLE:
    symbol = get_symbol();
    if(isalpha(symbol) && symbol < 'Z')
    {
        step_back();
        get_word();
    }
}

//комментарии для души
//isdigit
//isalpha
//код хуйня, кодеры пидоры



