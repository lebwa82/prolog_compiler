# prolog_compiler

На данный момент программа умеет читать такие примеры:
%однострочный комментарий

/*
многострочный 
комментарий
*/

fact(Book).
relation(mark,Book).
relation(mark,Book,dzen).
condition(mark,Book):-relation(Tom,Jerry,Kim).
nested_condition(mark,Book):-relation(Tom,Jerry,Kim):-fact(Book).

