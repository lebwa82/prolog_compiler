sum(1,1).
sum(N,S):-
    N>1,
   N2 is N-1,
sum(N2,S2),
S is S2+N.