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


Contact:
Парфенов Дмитрий, 
Александ Волков,
Михаил Авшалумов


Feel free to email your questions or comments to issues or to di_par@bk.ru.



