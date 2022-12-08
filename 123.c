#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct key_words {
    int index;
    char word;
} key_words;

key_words key_words_list[16] = {{-1, '+'}, {-2, '-'}, {-3, '='}, {-4, '*'},
    {-5, '/'}, {-6, '<'}, {-7, '>'}, {-8, '['},
    {-9, ']'}, {-10, ':'}, {-11, ';'}, {-12, ','},
    {-13, '|'}, {-14, '/'}, {-15, '('}, {-16, ')'},  //{-15, ":-"}, (-10,-2 = if)
};

int is_key_word(char word)
{
    for (int i = 0; i < 16; i++) {
        if (key_words_list[i].word == word)
            return key_words_list[i].index;
    }
    return 0;
}

int main_massiv_id[100];
int main_i;

enum type { //положительные
    DIGIT,
    STRING,
    NAME,
};

enum const_or_var {
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



char stroka[] = "nested_condition(mark,Book):-relation(Tom,Jerry,Kim).";
int *stroka_i;
int max_main_id;
char get_symbol()
{
    stroka_i++;
    if (*stroka_i != '\0') {
        printf("i = %c\n", *stroka_i);
        return *stroka_i;
    }
    return 0;
}

char step_back()
{
    stroka_i--;
    return 0;
}

/*char *get_word()
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
}*/

/*float get_number()
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
}*/


struct literal *head;
int append_word_in_literals(char *word, enum type type, enum const_or_var const_or_var)
// СДЕЛАТЬ ПРОВЕРКУ НА КОНЕЦ ПРЕДОЖЕНИЯ
//потому что переменная живет в рамках одного предложения
{
    int code;
    struct literal *p = head;
    while (p->next != NULL) {
        if (strcmp(p->literal_name, word) == 0)
            return p->id;
        p = p->next;
    }
    if (strcmp(p->literal_name, word) == 0)
        return p->id;
    
    struct literal *last = p;

    //пробежаться по массиву
    max_main_id++;
    p = malloc(sizeof(struct literal));
    /**p = {
        .id = max_main_id,
        .type = type,
        .const_or_var = const_or_var,
    };*/
    memcpy(p->literal_name, word, strlen(word));
    if (type == DIGIT) {
        p->value_char[0] = '\0';
        p->value_int = atoi(word);
    } else {
        memcpy(p->value_char, word, strlen(word));
        p->value_int = 0;
    }

    last->next = p;
    p->next = NULL;
    return p->id;

    //если есть - вернуть текущий id, если нет - добавить новый и вернуть
}

int main()
{
    head ->next = NULL;
    int i_massiv;
    int count_bracket = 0;
    char symbol;
    int name_id = 1, key_id = -1;
    main_i = 0, max_main_id = 0;
    int i = 0, id = 0;
    int word_i = 0;
    char *word[30];
    enum type type;//тип
    enum const_or_var const_or_var;//вид


//считаем, что избавились от пробелов и комментариев
START:
    symbol = get_symbol();
    printf("symbol = %c\n", symbol);
    if (isalpha(symbol)) {
        
        goto GET_WORD;
        char *word = get_word();
        id = append_word_in_literals(word, NAME, VAR);
        int type = 0;//как-то вычислить тип
        struct literal literal = {id, type, VAR, word};
        //добавить в массив
        main_massiv[main_i] = id;
        main_i++;
    }
    if (isdigit(symbol)) {
        float number = get_number();
        int type = DIGIT;
        char str_number[30];
        sprintf(str_number, &number, 30);//записать число в строку
        struct literal literal = {id, type, VAR, str_number};
        id++;//те числа просто добавляем с следующим по порядку id
        main_massiv[main_i] = id;
        main_i++;
    } else {
        id = is_key_word(symbol);
        if (id) {
            main_massiv[main_i] = id;
            main_i++;
        } else {
            goto ERROR;
        }
    }


GET_WORD: {
    char symbol = get_symbol();

    if (isalpha(symbol) || isdigit(symbol) || symbol == '_') {
        word[word_i] = symbol; word_i++;
        goto GET_WORD;
    }
    word[word_i] = '\0';
    word_i = 0;
    step_back();
    id = append_word_in_literals(word, NAME, VAR);
    main_massiv_id[main_i] = id;
    if(id > max_main_id)//нашел новое или старое
    {
        max_main_id++; 
    }
    
}




    
    

    
GET_NUMBER:
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


COMPLETE:
    puts("yes");
ERROR:
    puts("ERROR");
}