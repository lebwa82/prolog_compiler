#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum type {
    PROGR,
    MOP,
    P,
    H,
    B,
    S,
    E,
    T, 
    F, 
    a,
    ARG,
};



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

struct lex {
    enum symbol type;
    char data[10];
    int len;
};

enum symbol {
    OPEN_BRACKET,
    CLOSE_BRACKET,
    WORD,
    IF,
    CAV,
    COMMA,//запятая
    POINT,
    PLUS,
    MINUS,
    MORE,
    LESS,
    MULTIPLY,
    DIVIDE,
    PERCENT,
    MULTIPLE_COMMENT_START,
    MULTIPLE_COMMENT_END,
    ONELINE_COMMENT_START,
    BACKSLASH_N,
};
enum symbol massiv[100];
enum symbol *pointer = massiv;

int create_lex(enum symbol symbol_type, char *data)
{
    struct lex lex = {symbol_type, data, strlen(data)};
    memcpy(pointer, &lex, sizeof(struct lex));//massiv[i]=lex;
    pointer++;
}

int main()
{
    char symbol = 0;
    while(symbol!='\0')
    {
        symbol = get_symbol();
        switch (symbol){
            case '(':
                create_lex(OPEN_BRACKET, '(');
                break;
            case ')':
                create_lex(CLOSE_BRACKET, '(');
                break;
            case ':':
                symbol = get_symbol();
                if(symbol!='-'){printf("ERROR, next symbol is not "); goto ERROR;}
                create_lex(IF, ':-');
                break;
            case '"':
                create_lex(CAV, '"');
                break;
            case ',':
                create_lex(COMMA, ',');
                break;
            case '.':
                create_lex(POINT, '.');
                break;
            case '+':
                create_lex(PLUS, '+');
                break;
            case '-':
                create_lex(MINUS, '-');
                break;
            case '>':
                create_lex(MORE, '>');
                break;
            case '<':
                create_lex(LESS, '<');
                break;
            case '%':
                create_lex(PERCENT, '%');
                break;
            case '\n':
                create_lex(BACKSLASH_N, '\n');
                break;
            case '*':
                symbol = get_symbol();
                if (symbol == '/') {
                    create_lex(MULTIPLE_COMMENT_END, '');
                }
                else {
                    step_back();
                    create_lex(MULTIPLY, '*');
                    break;
                } 
            case '/':
                symbol = get_symbol();
                if (symbol == '*') {
                    create_lex(MULTIPLE_COMMENT_START, '');
                }
                else {
                    step_back();
                    create_lex(DIVIDE, '/');
                    break;
                }                
        }
        if(isdigit(symbol))
        {
            step_back();
            char word[30] = get_word();
            create_lex(WORD, word);
        }
        goto ERROR;
    }
   
ERROR:
    printf("ERROR");    




}