key_words key_words_list[] = {{-1, '+'}, {-2, '-'}, {-3, '='}, {-4, '*'},
    {-5, '/'}, {-6, '<'}, {-7, '>'}, {-8, '['},
    {-9, ']'}, {-10, ':'}, {-11, ';'}, {-12, ','},
    {-13, '|'}, {-14, '/'}, {-15, '('}, {-16, ')'}, {-17, '.'}, {-18, "\0"}  //{-15, ":-"}, (-10,-2 = if)
};



struct rule S__E = {//S->E
        S, {E, 0}, {0}, NULL
    };

    struct rule S__S_equal_E = {//S->S=E
        S, {is_key_word('='), E, 0}, {0}, NULL
    };

    struct rule S__S_bigger_E = {//S->S>E
        S, {is_key_word('>'), E, 0}, {0}, NULL
    };

    struct rule  S__S_smaller_E = { //S->S<E
        S, {is_key_word('<'), E}, {0}, NULL
    };

    struct rule E__T = { //E->T
        E, {T, 0}, {is_key_word(')'), is_key_word('+'), is_key_word('-'), 0}, NULL //TBD СПЕЦСИМВОЛ-РЕШЕТКА
    };

    struct rule E__E_plus_T = {//E->E+T
        E, 
        {E, is_key_word('+'), T, 0},
        {is_key_word(')'), is_key_word('+'), is_key_word('-'), 0}, 
        NULL
    };

    struct rule E__E_minus_T = {//E->E-T
        E, {E, is_key_word('-'), T, 0}, {is_key_word(')'), is_key_word('+'), is_key_word('-'), 0}, NULL
    };


    struct rule T__F = {//T->F
        T, {F, 0}, {0}, NULL
    };

    struct rule T__T_multiply_F = {//T->T*F
        T,
        {is_key_word('*'), F, 0},
        {is_key_word('+'), is_key_word('-'), is_key_word('*'), is_key_word('/'), is_key_word(')'), 0},
        NULL
    };

    struct rule T__T_divide_F = {//T->T/F
        T,
        {is_key_word('/'), F, 0},
        {is_key_word('+'), is_key_word('-'), is_key_word('*'), is_key_word('/'), is_key_word(')'), 0},
        NULL
    };
    struct rule F__a = {//F->a
        F, {a, 0}, {0}, NULL
    };

    struct rule F__E = {//F->(E)
        F, {is_key_word('('), E, is_key_word(')'), 0}, {0}, NULL
    };









     struct rule B__B_COMMA_a_BRACKET_ARG_BRACKET = {//B->B,a(ARG)
        B, {B, is_key_word(','), a, is_key_word('('), ARG, is_key_word(')'),}, {0}, NULL
    };

    struct rule B__a_BRACKET_ARG_BRACKET = {//B->B,a(ARG)
        B, {a, is_key_word('('), ARG, is_key_word(')'),}, {is_key_word('.'), is_key_word(','),}, NULL
    };

    struct rule H__a_BRACKET_ARG_BRACKET = {//B->B,a(ARG)
        H,
        {a, is_key_word('('), ARG, is_key_word(')'),}, 
        {is_key_word(':'), is_key_word('-'), B}, NULL
    };

    struct rule PROGR__MOP = {//PROGR->MOP 
        PROGR, {MOP, 0}, {0}, NULL
    };

    struct rule MOP__MOP_DOT_P = {//MOP->MOP. P
        MOP, {MOP, is_key_word('.'), P, 0}, {0}, NULL
    };

    struct rule MOP__P = {//MOP->MOP. P
        MOP, {P, 0}, {0}, NULL
    };

    struct rule P__H_COLON_DASH_B_DOT = {//P->H:-B.
        P, {H, is_key_word(':'), is_key_word('-'), B, is_key_word('.'), 0}, {0}, NULL
    };

    struct rule P__H_DOT = {//P->H:-B. 
        P, {H, is_key_word('.'), 0}, {0}, NULL
    };
    


     1 -15  2 -16 -10 -2  3 -15  4 -12  5 -12  6 -12  7 -16 -17  8 -15  9 -12 10 -12 11 -16 -17 -18 
     1 -15  2 -16 -10 -2  3 -15  4 -12  5 -12  6 -12  7 -16 -17  8 -15  9 -12 10 -12 11 -16 -17 