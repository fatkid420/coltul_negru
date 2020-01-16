int l = 80; // dimensiunea unei piese
int n; // dimensiunea tablei
int tabla[10][10]; // 0 nepus, 1-3 tipurile de piesa
int valid[10][10]; // 0 nu se poate pune piesa, 1 se poate pune

void piesa0(int x, int y) {
    circle(x + l / 2, y + l / 2, l / 2);
}

void piesa1(int x, int y) {
    line(x, y + l / 2, x + l, y + l / 2);
    line(x + l / 2, y, x + l / 2, y + l);
}

void piesa2(int x, int y) {
    arc(x, y + l, 0, 90, l / 2);
    arc(x + l, y, 180, 270, l / 2);
}

void piesa3(int x, int y) {
    arc(x, y, 270, 0, l / 2);
    arc(x + l, y + l, 90, 180, l / 2);
}

void filledSquare(int x, int y) {
    bar(x + 1, y + 1, x + l, y + l);
}

void square(int x, int y) {
    rectangle(x, y, x + l, y + l);
}

void umple0(int a[10][10]) {
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            a[i][j] = 0;
}

bool esteAfara(int i, int j) {
    if (i < 0 || i >= n || j < 0 || j >= n) return true;
    return false;
}

void deseneazaTabla() {
    setcolor(BLACK);
    int x, y;
    for (int i = 0; i < n; i++) {
        x = i * l;
        for (int j = 0; j < n; j++) {
            y = j * l;
            filledSquare(x, y);
            square(x, y);
            if (tabla[i][j] == 0 && valid[i][j] == 1) // mutare valida
                piesa0(x, y);
            if (tabla[i][j] == 1) // piesa de tip 1
                piesa1(x, y);
            else if (tabla[i][j] == 2) // piesa de tip 2
                piesa2(x, y);
            else if (tabla[i][j] == 3) // piesa de tip 3
                piesa3(x, y);
        }
    }
    setfillstyle(SOLID_FILL, BLACK);
    filledSquare(n * l - l, n * l - l); // coltul negru
    setfillstyle(SOLID_FILL, WHITE);
    setcolor(WHITE);
    line(0, n * l, n * l, n * l);
    line(n * l, 0, n * l, n * l);
}
