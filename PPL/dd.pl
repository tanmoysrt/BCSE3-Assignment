ancestor(X,Y) :- ancestor(Z, Y), parent(X, Z).
ancestor(X, X).
parent(amy, bob).