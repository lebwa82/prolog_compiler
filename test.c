#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct rule
{
    int L;
    char R[5];
    int condition_symbol[10]; //= {-1};
    struct rule *next;
};



int main()
{

struct rule S__E = {1, {123,2}, {43,3}, NULL};
printf("%d %d %d\n", S__E.L, S__E.R[0], S__E.condition_symbol[0]);


}