#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum vid{
    VAR,
    CONST,
};

enum type{
    INT,
    FLOAT,
    STRING,
    //ATOM1,
    SPISOK,
    STRUCTURA,
};

typedef struct key_words{
    int index;
    char word[10];
}key_words;

key_words key_words_list[] = {{-1, '+'}, {-2, '-'}, {-3, '='}, {-4, '*'}, 
                            {-5, '/'}, {-6, '<'}, {-7, '>'}, {-8, '['}, 
                            {-9, ']'}, {-10, ':'}, {-11, ';'}, {-12, ','}, 
                            {-13, '|'}, {-14, '/'}, {-15, ':-'}};

struct names{
    int index;
    char name[10];
    int vid;
    int type;
};

char get_symbol()
{
    return 'c';
}

int main()
{
    char symbol;
START:
    symbol = get_symbol();
    switch(symbol)
    {
    case '%':
        goto ONELINE_COMMENT;
    case '/':
        goto PREDICT_MULTILINE_COMMENT;
    default:
        printf("START ERROR\n");
        goto ERROR;
    }

ONELINE_COMMENT:
    symbol = get_symbol();
    switch (symbol)
    {
    case '\n':
        goto START;
    default:
        goto ONELINE_COMMENT;
    }

PREDICT_MULTILINE_COMMENT:
    symbol = get_symbol();
    switch (symbol)
    {
    case '*':
        goto MULTILINE_COMMENT;

    default:
        printf("PREDICT_MULTILINE_COMMENT ERROR\n");
        goto ERROR;
    }

MULTILINE_COMMENT:
    symbol = get_symbol();
    switch (symbol)
    {
    case '/':
        goto START;

    default:
        printf("MULTILINE_COMMENT ERROR\n");
        goto ERROR;
    }

ERROR:
    printf("ERROR\n");


DIGIT:
    int int_number = 0;
    while(isdigit(symbol) || int_number == 0)
    {
        int_number = int_number*10 + (symbol-'0');
        symbol = get_symbol();
    }
    if(symbol == '.')
        goto FLOAT;
    else{}
        //что-то еще

FLOAT:
    float float_number = int_number;
    float tmp = 10;
    while(isdigit(symbol) || int_number == 0)
    {
        float_number += (symbol-'0')/tmp;
        tmp *= 10;
        symbol = get_symbol();
    }



return 0;    
}

//комментарии для души
//isdigit
//isalpha

        


