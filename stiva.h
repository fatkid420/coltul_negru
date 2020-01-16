struct elementStiva{
    int lin, col; // pozitia piesei
    int tip; // tipul piesei
};

struct stiva {
    elementStiva v[1000];
    int vf = -1;
};

void push(stiva& S, elementStiva e) {
    S.v[++S.vf] = e;
}

void pop(stiva& S) {
    S.vf--;
}

elementStiva top(stiva& S) {
    return S.v[S.vf];
}

bool isEmpty(stiva S) {
    return (S.vf < 0);
}

void goleste(stiva& S) {
    while (S.vf >= 0) pop(S);
}
