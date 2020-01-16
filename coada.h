int player = 1;
bool ales, alesDreapta; // variabile care ma ajuta sa vad ce cale initiala am ales (in jos sau in dreapta)

struct piesaLee {
    int lin, col; // pozitia piesei
    int tip; // tipul piesei
    char dir; //u, d, l, r : directia din care vine traseul
};

struct coada {
    piesaLee v[1000]; // coada propriu-zisa
    int in = 0, sf = 0; // inceputul si sfarsitul cozii
};

void push(coada & C, piesaLee p) {
    C.v[++C.sf] = p;
}

piesaLee read(coada C) {
    return C.v[C.in];
}

void pop(coada & C) {
    C.in++;
}

bool isEmpty(coada C) {
    return (C.in > C.sf);
}

void initLee(coada & C) {
    C.in = C.sf = 0;
    if (!ales) {
        push(C, {0, 0, 1, 'l'});
        push(C, {0, 0, 1, 'u'});
    } else {
        if (alesDreapta) push(C, {0, 0, 1, 'l'});
        else push(C, {0, 0, 1, 'u'});
    }
}
