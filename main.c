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

enum key {
    START,
    CONDITION,
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

char operators[] = {'>', '<'};
int is_operator(char symbol)
{
    int len = sizeof(operators);
    int i;
    for(i=0; i<len; i++)
    {
        if(symbol == operators[i])
            return 1;
    }
    return 0;
}


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

char stroka[] = "nested_condition(mark,Book):-relation(Tom,Jerry,Kim):-fact(Book).";//входная строка
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
    int i = 0;
    while (isalpha(symbol) || isdigit(symbol) || symbol == '_') {
        //strcat(word, &symbol);
        word[i] = symbol; i++;
        symbol = get_symbol();
    }
    word[i] = '\0';
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
 
float get_number() {
        char symbol = get_symbol();
        float number = 0;
        while (isdigit(symbol) || number == 0) {
            number = number * 10 + (symbol - '0');
            symbol = get_symbol();
        }
        if (symbol == '.') {
            float tmp = 10;
            char symbol = get_symbol();
            while (isdigit(symbol) || number == 0) {
                number += (symbol - '0') / tmp;
                tmp *= 10;
                symbol = get_symbol();
            }
            printf("READ FLOAT\n");
            
        }
        else
            printf("READ INT\n"); 
        
        step_back();
        printf("DIGIT = %f\n", number);
        return number;
    }


int main()
{
    //f = fopen("input.txt", "r");
    char symbol = 0;
    //char word[30];
    enum key key = START;
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
            goto RELATION_NAME;
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
RELATION_NAME: {
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
                printf("RELATION_NAME ERROR\n");
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
    char *word = get_word();
    symbol = get_symbol();
    switch (symbol) {
        case ',':
            goto PREDICT_RELATION_SECOND_NAME;
        case ')':
            goto DEFINITELY_END_OF_FACT;

        default:
            printf("PREDICT_RELATION_OR_FACT_FIRST_VARIABLE ERROR\n");
            goto ERROR;
    }
}

DEFINITELY_END_OF_FACT: {
    symbol = get_symbol();
    if (symbol == '.') {
        printf("FACT COMPLETE\n");
        goto  START;
    }
    if(key == CONDITION)
    {
        if(symbol == ',')
        {
            printf("FACT COMPLETE, READING NEXT RELATION\n");
            goto START_CONDITION_BODY;
        }
    }
    printf("DEFINITELY_END_OF_FACT ERROR\n");
    goto ERROR;
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
        //step_back();
        goto PREDICT_RELATION_COMPLETE_PREDICT_BRACKET_AND_POINT;
    }

PREDICT_RELATION_COMPLETE_PREDICT_BRACKET_AND_POINT: {
        symbol = get_symbol();
        if (symbol == ')') {
            goto PREDICT_POINT_OR_CONDITION;
        }
        if (symbol == ',') {
            goto PREDICT_RELATION_SECOND_NAME;
        }
        printf("PREDICT_RELATION_COMPLETE_PREDICT_BRACKET ERROR\n");
        goto ERROR;
    }

PREDICT_POINT_OR_CONDITION: {
    symbol = get_symbol();
    if (symbol == '.') {
        printf("Sentence complete\n");
        goto START;
    }

    if (symbol == ':') {
        printf("RELATION COMPLETE READING CONDITION\n");
        goto PREDICT_CONDITION_DASH; //ждем тире
    }

    if (key == CONDITION){
        if(symbol == ','){
            printf("RED CONDITION, READING NEXT CONDITION\n");
            goto START_CONDITION_BODY;
        } 
    }
    printf("PREDICT_POINT_OR_CONDITION ERROR\n");
    goto ERROR;
}


PREDICT_CONDITION_DASH: {//прочитали тире
    symbol = get_symbol();

    if (symbol == '-') {
        key = CONDITION;
        goto START_CONDITION_BODY; //ждем тире
    }
    printf("PREDICT_CONDITION_DASH ERROR\n");
    goto ERROR;
}

START_CONDITION_BODY:{
    symbol = get_symbol();
    if(!isalpha(symbol))
    {
        printf("START_CONDITION_BODY_WORD ERROR\n");
        goto ERROR;
    }
    step_back();
    char *word = get_word();
    //потенчиално определить это отношение или переменная

    symbol = get_symbol();
    if(is_operator(symbol)) {
        goto CONDITION_BODY_SECOND_WORD_VARIABLE_OR_DIGIT;
    }
    if(symbol == '(') {
        goto CONDITION_BODY_RELATION;//зацикливаемся
    }
    printf("START_CONDITION_BODY OPERATOR OR ( ERROR\n");
    goto ERROR;
    
}

CONDITION_BODY_SECOND_WORD_VARIABLE_OR_DIGIT:{
    symbol = get_symbol();
    if (isalpha(symbol)){
        step_back();
        char *word = get_word();
        printf("RED OPERATOR EXPRESSION WITH A VARIABLE\n");
        goto PREDICT_POINT_OR_CONDITION;
    }

    if (isdigit(symbol)){
        step_back();
        get_number();
        printf("RED OPERATOR EXPRESSION WITH A DIGIT\n");
        goto PREDICT_POINT_OR_CONDITION;
        
    }

    printf("CONDITION_BODY_SECOND_WORD_VARIABLE_OR_DIGIT ERROR");
    goto ERROR;
}

CONDITION_BODY_RELATION:{
    symbol = get_symbol();
    if (isalpha(symbol))
    {
        step_back();
        goto PREDICT_RELATION_OR_FACT_FIRST_VARIABLE_OR_STRING;
    }

}


ERROR:
    printf("ERROR\n");
    return 0;




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
.

