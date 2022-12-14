/*---------------------------------------------------------------------------------------------
Header-файл, содержащий функции, переменные и массивы, небходимые в разных частях проекта
-----------------------------------------------------------------------------------------------*/


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
//главный массив, выходной для сканера, входной для синт. управляемого перевода
int main_massiv_id[100]; 
//итератор главного массива
int main_i; 
//максимальный id главного массива
int max_main_id = 1; 

//Список типов данных, кодируются положительным числом
enum type { 
    DIGIT,
    STRING,
    NAME,
    VAR_TYPE,
};

//возможный вид типов данных
enum const_or_var {
    CONSTANTA,
    VAR,
};

//Элемент таблицы имен
struct literal { 
    int id;
    enum type type;//тип
    enum const_or_var const_or_var;//вид

    char literal_name[30];//изначальное имя
    char value_char[30]; //значение переменной
    int value_int;
    struct literal *next;
};
struct literal *head_literals;

//элемент таблицы ключевых слов
typedef struct key_words {
    int index;
    char word;
} key_words;

//Таблица ключевых слов интерпретируемого языка
key_words key_words_list[] = {{-1, '+'}, {-2, '-'}, {-3, '='}, {-4, '*'},
    {-5, '/'}, {-6, '<'}, {-7, '>'}, {-8, '['},
    {-9, ']'}, {-10, ':'}, {-11, ';'}, {-12, ','},
    {-13, '|'}, {-14, '/'}, {-15, '('}, {-16, ')'}, {-17, '.'}, {-18, '#'},  //{-15, ":-"}, (-10,-2 = if)
};


int len_key_words_list = sizeof(key_words_list) / sizeof(key_words_list[0]);

//Проверка, находится ли слово в таблице ключевых слов
int is_key_word(char word)
{
    for (int i = 0; i < len_key_words_list; i++) {
        //printf("%d %c\n", key_words_list[i].index, key_words_list[i].word);
        if (key_words_list[i].word == word)
            return key_words_list[i].index;
    }
    return 0;
}

//Заполнение таблицы имен
int append_word_in_literals(char *word, enum type type, enum const_or_var const_or_var)
{
    struct literal *p = head_literals;
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
    p->id = max_main_id;
    p->type = type;
    p->const_or_var = const_or_var;

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


//функция вывода элемента таблицы имен
int print_literal()
{   FILE *name_table;
    if ((name_table = fopen("name_table.txt", "w")) == NULL) {
        printf("Невозможно открыть файл\n");
        return 0;
    };
    struct literal *p = head_literals->next;
    while (p != NULL) {
        if(p->type == DIGIT)
            fprintf(name_table, "%2d %2d %2d %s %s %2d\n", p->id, p->type, p->const_or_var, p->literal_name, "NULL", p->value_int);
        else 
        fprintf(name_table, "%2d %2d %2d %s %s %2d\n", p->id, p->type, p->const_or_var, p->literal_name, p->value_char, p->value_int);
        printf("p->literal_name = %s,  p->id = %2d,  p->type = %d\n", p->literal_name, p->id, p->type);
        p = p->next;
    }
    fclose(name_table);
    return 0;
}

//функция вывода главного массива
int print_main_massiv()
{   
    FILE *file_mass;
    if ((file_mass = fopen("mainmass.txt", "w")) == NULL) {
        printf("Невозможно открыть файл\n");
        return 0;
    };
    for (int i = 0; i < main_i; i++) {
        printf("%2d ", main_massiv_id[i]);
        fprintf(file_mass, "%2d ", main_massiv_id[i]);
    }
    fclose(file_mass);
    printf("\n");
    return 0;
}