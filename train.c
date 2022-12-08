#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum leexema123 {
    PROGR,
    MOP,
    P,
    H,
    B,
    S,
    E,
    T,
    F,
    //a,
    //ARG,
};
//таблица ключевых слов(отриц)+создать массив АКА таблица имен([хэш-функция??])
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
    //DIGIT,//константа
    //STRING,


    //ARG,
    //a
};

enum names{
    DIGIT,
    STRING, 
    NAME,
    VAR,
};

enum symbol _massiv[100];
enum symbol *pointer = _massiv;

struct lex {
    enum lexema leexema;
    int len;
    char data[10];
};

int create_lex(enum symbol symbol_type, char *data)
{
    struct lex lex = {symbol_type, strlen(data), ""};
    memcpy(lex.data, data, strlen(data));
    memcpy(pointer, &lex, sizeof(struct lex));//massiv[i]=lex;
    pointer++;
}

char stroka[] =
    "nested_condition(mark,\"Book\"):-relation(\"Tom\",\"Jerry\",Kim).";//входная строка
char *i = stroka - 1;

char step_back()
{
    i--;
    return 0;
}

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

float get_number()
{
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

    } else
        printf("READ INT\n");

    step_back();
    printf("DIGIT = %f\n", number);
    return number;
}



int main()
{
    int count_bracket = 0;
    enum symbol *massiv = _massiv;
    char symbol = 1;
    while (1) {
        symbol = get_symbol();
        printf("symbol = %c\n", symbol);
        if(count_bracket < 0) 
        {
            printf("Bracket ERROR\n");
            goto ERROR;
        }
        switch (symbol) {
            case '\0':
                goto COMPLETE;;
            case '(':
                count_bracket++;
                create_lex(OPEN_BRACKET, "(");
                break;
            case ')':
                count_bracket--;
                create_lex(CLOSE_BRACKET, ")");
                break;
            case ':':
                symbol = get_symbol();
                if (symbol != '-') {
                    printf("ERROR, next symbol is not ");
                    goto ERROR;
                }
                create_lex(IF, ":-");
                break;
            case '"':
              //  char a[2];
               // a[0] = '"';
                //a[1] = '\0';
                create_lex(CAV, "\"");
                break;
            case ',':
                create_lex(COMMA, ",");
                break;
            case '.':
                create_lex(POINT, ".");
                break;
            case '+':
                create_lex(PLUS, "+");
                break;
            case '-':
                create_lex(MINUS, "-");
                break;
            case '>':
                create_lex(MORE, ">");
                break;
            case '<':
                create_lex(LESS, "<");
                break;
            case '%':
                create_lex(PERCENT, "%");
                break;
            case '\n':
                create_lex(BACKSLASH_N, "\n");
                break;
            case '*':
                symbol = get_symbol();
                if (symbol == '/') {
                    create_lex(MULTIPLE_COMMENT_END, "");
                } else {
                    step_back();
                    create_lex(MULTIPLY, "*");
                    break;
                }
            case '/':
                symbol = get_symbol();
                if (symbol == '*') {
                    create_lex(MULTIPLE_COMMENT_START, " ");
                } else {
                    step_back();
                    create_lex(DIVIDE, "/");
                    break;
                }
            default:
                if (isalpha(symbol)) {
                    //puts("digit");
                    step_back();
                    char *word = get_word();
                    create_lex(WORD, word);
                } else if (isdigit(symbol)) {
                    //puts("digit");
                    step_back();
                    int number = get_number();
                    char word[30];
                    sprintf(word, "%d", number);
                    create_lex(DIGIT, word);
                } else
                    goto ERROR;
        }

    }

ERROR:
    printf("ERROR");
    return 0;



COMPLETE:
    if (count_bracket != 0)
    {
        printf("Bracket ERROR\n");
        goto ERROR;
    }
    enum symbol *p = massiv;
    while (p != pointer) {
        printf("%2d ", *p);
        p++;
    }
    int len = pointer - massiv, i, i2, len2 = pointer - massiv;

    enum symbol massiv2[len];
    while (i2 < len2) {
        if (i2 + 2 < len && massiv[i] == CAV && massiv[i + 1] == WORD && massiv[i + 2] == CAV) {
            //massiv2[i2] = massiv[i + 1];
            massiv2[i2] = WORD;//STRING
            i2++;
            i+=3;
            len2-=2;
        } else {
            massiv2[i2] = massiv[i];
            i++;
            i2++;
        }
    }
    printf("\nlen = %d len2 = %d\n", len, len2);
    for(i = 0; i < len2; i++)
        printf("%2d ", massiv2[i]);
    
    massiv = massiv2;
    len = len2;
    for (i = 0; i < len; i++) {
        if (i + 2 < len && massiv[i] == OPEN_BRACKET && massiv[i + 1] == WORD &&
            massiv[i + 2] == CLOSE_BRACKET) {
            massiv[i + 1] = ARG;
            continue;
        }
        if (i + 1 < len & massiv[i] == WORD && massiv[i + 1] == OPEN_BRACKET) {
            massiv[i] = a;
            continue;
        }
        if (i + 2 < len && massiv[i] == COMMA && massiv[i + 1] == WORD && massiv[i + 2] == CLOSE_BRACKET) {
            massiv[i + 1] = a;
            continue;
        }
        if (i + 2 < len && massiv[i] == COMMA && massiv[i + 1] == WORD && massiv[i + 2] == COMMA) {
            massiv[i + 1] = a;
            continue;
        }
        if (i + 2 < len && massiv[i] == OPEN_BRACKET && massiv[i + 1] == WORD && massiv[i + 2] == COMMA) {
            massiv[i + 1] = ARG;
            continue;
        }
    }
    printf("\n\n");
    for (i = 0; i < len; i++) {
        printf("%2d ", massiv[i]);
    }

    struct relation{
        char relation_name[30];
        int count_args;
        char args[30];
    };

    struct sentence{
        struct relation *head;
        struct relation *body;
    };








    return 0;



}