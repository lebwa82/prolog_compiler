#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


enum key_word {//отрицательные
    OPEN_BRACKET,
    CLOSE_BRACKET,
    WORD,
    IF,
    //IS,
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
enum key_word massiv[100];

int main_massiv[100];

enum names{//положительные
    DIGIT,
    STRING, 
    NAME,
    VAR,
    //KEY,
};

struct literal//может как ключевое слово, так и литералы
{
    int id;
    enum names type;//add вид
    char изначальное имя[30];
    //значение переменной
    struct literal *next;
};



char stroka[] = "nested_condition(mark,Book):-relation(Tom,Jerry,Kim).";
int i;
int id;
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

typedef struct key_words {
    int index;
    char word[10];
} key_words;

key_words key_words_list[] = {{-1, "+"}, {-2, "-"}, {-3, "="}, {-4, "*"},
    {-5, "/"}, {-6, "<"}, {-7, ">"}, {-8, "["},
    {-9, "]"}, {-10, ":"}, {-11, ";"}, {-12, ","},
    {-13, "|"}, {-14, "/"}, {-15, ":-"}, {-16, "("}, {-17, ")"}
};

int is_key_word(char word)
{
    int code;
    //пробежаться по массиву
    int name_id = 1;
    if(code<0)
    {
        return code;
    }
    else
    {
        return 0;
    }
}

int append_word_in_literals(char word) // СДЕЛАТЬ ПРОВЕРКУ НА КОНЕЦ ПРЕДОЖЕНИЯ
//потому что переменная живет в рамках одного предложения
{
    int code;
    //пробежаться по массиву
    int name_id = 1;
    if(code<0)
    {
        return code;
    }
    else
    {
        return 0;
    }
    //если есть - вернуть текущий id, если нет - добавить новый и вернуть 
}

int main()
{
    struct literal* head = {0, 0, NULL, NULL};
    int i_massiv;
    int count_bracket=0;
    char symbol;
    int name_id = 1, key_id = -1;
    int main_i;
    i=0, id=0;
    
    START:
        symbol = get_symbol();
        printf("symbol = %c\n", symbol);
        if(isalpha(symbol))
        {
            char *word = get_word();
            id = append_word_in_literals(word);
            int type = 0;//как-то вычислить тип
            struct literal literal = {id, type, word, NULL};
            //добавить в список 
            main_massiv[main_i] = id;
            main_i++;
        }
        if(isdigit(symbol))
        {
            float number = get_number();
            int type = DIGIT;
            char str_number[30];
            sprintf(str_number, &number, 30);//записать число в строку
            struct literal literal = {id, type, str_number, NULL};
            id++;//те числа просто добавляем с следующим по порядку id
            main_massiv[main_i] = id;
            main_i++;
        }
        else
        {
            id = is_key_word(symbol);
            if(id)
            {
                main_massiv[main_i] = id;
                main_i++;
            }
            else
            {
                goto ERROR;
            }
        }
        
        

        
        
    
COMPLETE:
    puts("yes");
     

ERROR:
    puts("ERROR");







}