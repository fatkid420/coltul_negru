#include <graphics.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <winbgim.h>
#include <MMsystem.h>
#include "coada.h"
#include "graficaJoc.h"
#include "stiva.h"
#include "butoaneInGame.h"

using namespace std;

void undo();
void redo();
bool lee(coada&);

//variabile meniu
buton bm[10]; // butoane meniu
buton dim[10]; // dimensiune tabla
bool meniuActiv = true, jocActiv = false; // variabile care imi spun ce meniu este activ la un moment dat
int gameMode; // 1 = pc, 2 = pvp
int xmouse, ymouse; // pozitia mouse ului pe ecran
int contor; // variabila de contorizare
int limba = 1; // 1 == romana; 2 == engleza

//variabile joc
coada C; // coada pentru determinat o pozitie valida
stiva S1, S2; //S1 e pentru undo, S2 e pentru redo
elementStiva ultimaPusa; // ultima piesa pusa
buton b[10]; // butoane in-game
bool player1W, player1L, player2W, player2L; // variabile care imi spun cine a castigat/pierdut
int i, j; // linia si coloana corespunzatoare pozitiei mouse ului
bool gata; // gata e adevarat daca jocul e gata
bool amPusPiesa; // vs pc
int valRandom; // valoarea unei piese aleatorii (vs pc)
int xoff = 25, yoff;
int xrel, yrel = 50, dimInGame = 1;

int main()
{
    srand(time(0));
    PlaySound("fsong.wav", NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);
    initwindow(800, 600);

    int w = getwindowwidth(), h = getwindowheight();
    int dimMeniu = 4; // dimensiunea butoanelor din meniu
    initButon(bm[0],"COLTUL NEGRU",w / 2,100,dimMeniu + 1);
    initButon(bm[1],"VS PLAYER",w / 2,200,dimMeniu);
    initButon(bm[2], "VS COMPUTER",w / 2,275,dimMeniu);
    initButon(bm[3], "INSTRUCTIUNI",w / 2,350,dimMeniu);
    initButon(bm[4], "LIMBA", w / 2,425,dimMeniu);
    initButon(bm[5], "IESIRE",w / 2,500,dimMeniu);
    initButon(bm[6], "INAPOI",w / 2,500,dimMeniu);
    initButon(bm[7], "ROMANA",w / 4, h / 2 + 50,dimMeniu);
    initButon(bm[8], "ENGLISH",3 * w / 4, h / 2 + 50,dimMeniu);

    int dimSize = 5; // dimensiunea butoanelor de selectare a tablei
    initButon(dim[0], "4x4", w / 4, 200, dimSize);
    initButon(dim[1], "5x5", w / 4, 275, dimSize);
    initButon(dim[2], "6x6", w / 4, 350, dimSize);
    initButon(dim[3], "7x7", 3 * w / 4, 200, dimSize);
    initButon(dim[4], "8x8", 3 * w / 4, 275, dimSize);

    bool rulez = true;
    while (rulez) {
        if (meniuActiv) {
            for (contor = 1; contor < 6; contor++) deseneazaButon(bm[contor]);
            deseneazaButon(bm[0], 1);
            while (1) {
                getmouseclick(WM_LBUTTONDOWN, xmouse, ymouse);
                if (xmouse > 0 && ymouse > 0) {
                    if (xmouse >= bm[1].x1 && xmouse <= bm[1].x2 && ymouse >= bm[1].y1 && ymouse <= bm[1].y2) { // vs player
                        jocActiv = true; gameMode = 2;
                        cleardevice();
                        for (contor = 0; contor < 5; contor++) deseneazaButon(dim[contor]);
                        settextstyle(SIMPLEX_FONT, HORIZ_DIR, 3);
                        if (limba == 1) outtextxy(50, 75, "Selectati dimensiunea tablei de joc.");
                        else if (limba == 2) outtextxy(50, 75, "Choose a configuration.");
                        settextstyle(DEFAULT_FONT, HORIZ_DIR, 0);
                        while (1) {
                            getmouseclick(WM_LBUTTONDOWN, xmouse, ymouse);
                            if (xmouse > 0 && ymouse > 0)
                                for (contor = 0; contor < 5; contor++)
                                    if (xmouse >= dim[contor].x1 && xmouse <= dim[contor].x2 && ymouse >= dim[contor].y1 && ymouse <= dim[contor].y2)
                                        {n = contor + 4; break;}
                            if (contor < 5) break;
                        }
                        closegraph();
                        break;
                    }
                    else if (xmouse >= bm[2].x1 && xmouse <= bm[2].x2 && ymouse >= bm[2].y1 && ymouse <= bm[2].y2) { // vs pc
                        jocActiv = true; gameMode = 1;
                        cleardevice();
                        for (contor = 0; contor < 5; contor++) deseneazaButon(dim[contor]);
                        settextstyle(SIMPLEX_FONT, HORIZ_DIR, 3);
                        if (limba == 1) outtextxy(50, 75, "Selectati dimensiunea tablei de joc.");
                        else if (limba == 2) outtextxy(50, 75, "Choose a configuration.");
                        settextstyle(DEFAULT_FONT, HORIZ_DIR, 0);
                        while (1) {
                            getmouseclick(WM_LBUTTONDOWN, xmouse, ymouse);
                            if (xmouse > 0 && ymouse > 0)
                                for (contor = 0; contor < 5; contor++)
                                    if (xmouse >= dim[contor].x1 && xmouse <= dim[contor].x2 && ymouse >= dim[contor].y1 && ymouse <= dim[contor].y2)
                                        {n = contor + 4; break;}
                            if (contor < 5) break;
                        }
                        closegraph();
                        break;
                    }
                    else if (xmouse >= bm[3].x1 && xmouse <= bm[3].x2 && ymouse >= bm[3].y1 && ymouse <= bm[3].y2) { // instructiuni
                        cleardevice();
                        settextstyle(8, 0, 2);
                        if (limba == 1) {
                            outtextxy(20, 20, "Jocul se incepe obligatoriu cu un jeton cu liniile incrucisate.");
                            outtextxy(5, 40, "Problema este ca plasand unul langa altul asemenea jetoane sa ");
                            outtextxy(5, 60,"se conduca o linie continua din coltul de inceput al tablei in  ");
                            outtextxy(5, 80,"coltul negru. Castiga jocul cel care pune jetonul cu care linia ");
                            outtextxy(5, 100,"lunga a jocului ajunge in (se suprapune peste) coltul negru.");
                            outtextxy(20, 120,"Din cele doua linii ale fiecarui cartonas numai una duce linia");
                            outtextxy(5,140,"jocului mai departe, dar la mutari urmatoare pot fi cuprinse in");
                            outtextxy(5, 160,"continuarea liniei si capete (ramificatii din linia principala)");
                            outtextxy(5,180,"ramase pe parcurs. ln felul acesta pot aparea si bucle.");
                            outtextxy(20,200,"Daca langa primul jeton, al doilea jucator va plasa un jeton cu");
                            outtextxy(5,220,"linii incrucisate, linia jocului se continua drept, iar daca");
                            outtextxy(5,240,"va plasa un jeton cu linii curbe, linia jocului este intoarsa cu");
                            outtextxy(5,260,"90° la stanga sau la dreapta.");
                            outtextxy(20,280,"Este interzis ca linia principala sa fie condusa la marginea");
                            outtextxy(5,300,"tablei! Jucatorul cu a carui mutare se conduce linia jocului la");
                            outtextxy(5,320,"conturul tablei pierde jocul. Se intelege, deci, ca al doilea");
                            outtextxy(5,340,"jeton nu va putea fi plasat.");
                            outtextxy(20,360,"Liniile libere se pot uni cu linia principala, dar daca si in");
                            outtextxy(5,380,"acest mod linia jocului ajunge la marginea tablei, jucatorul");
                            outtextxy(5,400,"pierde jocul.");
                        } else if (limba == 2) {
                            outtextxy(20, 20, "The game starts with the first tile, the one with crossed lines.");
                            outtextxy(5, 40, "From there, the players have to choose between three tiles in");
                            outtextxy(5, 60,"order to form a long line from the top left square to the black");
                            outtextxy(5, 80,"square. The winner is the player who manages to connect those");
                            outtextxy(5, 100,"two corners.");
                            outtextxy(20, 120,"Only one line (out of two) of each tile may continue the game,");
                            outtextxy(5,140,"but for the next moves any remaining lines of any piece may be");
                            outtextxy(5, 160,"used. Loops may appear in the process.");
                            outtextxy(20,180,"If Player 2 chose to place the first tile next to the top left");
                            outtextxy(5,200,"tile, the game’s line would continue straight, and if Player 2");
                            outtextxy(5,220,"chose to place one of the other two pieces, the game’s line");
                            outtextxy(5,240,"would make a 90 degree turn to the left or right.");
                            outtextxy(20,260,"It is forbidden for the main line to intersect one of the table's");
                            outtextxy(5,280,"margins. The player who makes such a move loses the game. Any");
                            outtextxy(5,300,"remaining lines can intersect with the main one, but if in the");
                            outtextxy(5,320,"process the main line goes out of bounds, the responsible player");
                            outtextxy(5,340,"loses the game.");
                        }

                        deseneazaButon(bm[6]);
                        while (1) {
                            getmouseclick(WM_LBUTTONDOWN, xmouse, ymouse);
                            if (xmouse > 0 && ymouse > 0)
                                if (xmouse >= bm[6].x1 && xmouse <= bm[6].x2 && ymouse >= bm[6].y1 && ymouse <= bm[6].y2)
                                    {cleardevice(); break;}
                        }
                        break;
                    }
                    else if (xmouse >= bm[4].x1 && xmouse <= bm[4].x2 && ymouse >= bm[4].y1 && ymouse <= bm[4].y2) { // limba
                        cleardevice();
                        for (contor = 7; contor < 9; contor++) deseneazaButon(bm[contor]);
                        readimagefile("ro.bmp", 98, 194, 304, 334);
                        readimagefile("en.bmp", 504, 194, 704, 334);
                        while (1) {
                            getmouseclick(WM_LBUTTONDOWN, xmouse, ymouse);
                            if (xmouse > 0 && ymouse > 0) {
                                if (xmouse >= bm[7].x1 && xmouse <= bm[7].x2 && ymouse >= bm[7].y1 && ymouse <= bm[7].y2) {
                                    if (limba != 1) {
                                        limba = 1;
                                        initButon(bm[0],"COLTUL NEGRU",w / 2,100,dimMeniu + 1);
                                        initButon(bm[1],"VS PLAYER",w / 2,200,dimMeniu);
                                        initButon(bm[2], "VS COMPUTER",w / 2,275,dimMeniu);
                                        initButon(bm[3], "INSTRUCTIUNI",w / 2,350,dimMeniu);
                                        initButon(bm[4], "LIMBA", w / 2,425,dimMeniu);
                                        initButon(bm[5], "IESIRE",w / 2,500,dimMeniu);
                                        initButon(bm[6], "INAPOI",w / 2,500,dimMeniu);
                                        initButon(bm[7], "ROMANA",w / 4, h / 2 + 50,dimMeniu);
                                        initButon(bm[8], "ENGLISH",3 * w / 4, h / 2 + 50,dimMeniu);
                                    }
                                    cleardevice();
                                    break;
                                }
                                else if (xmouse >= bm[8].x1 && xmouse <= bm[8].x2 && ymouse >= bm[8].y1 && ymouse <= bm[8].y2) {
                                    if (limba != 2) {
                                        limba = 2;
                                        initButon(bm[0],"BLACK SQUARE",w / 2,100,dimMeniu + 1);
                                        initButon(bm[1],"VS PLAYER",w / 2,200,dimMeniu);
                                        initButon(bm[2], "VS COMPUTER",w / 2,275,dimMeniu);
                                        initButon(bm[3], "INSTRUCTIONS",w / 2,350,dimMeniu);
                                        initButon(bm[4], "LANGUAGE", w / 2,425,dimMeniu);
                                        initButon(bm[5], "EXIT",w / 2,500,dimMeniu);
                                        initButon(bm[6], "BACK",w / 2,500,dimMeniu);
                                        initButon(bm[7], "ROMANA",w / 4, h / 2 + 50,dimMeniu);
                                        initButon(bm[8], "ENGLISH",3 * w / 4, h / 2 + 50,dimMeniu);
                                    }
                                    cleardevice();
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    else if (xmouse >= bm[5].x1 && xmouse <= bm[5].x2 && ymouse >= bm[5].y1 && ymouse <= bm[5].y2) { // ies
                        rulez = false; break;
                    }
                }
            }
        }

        if (jocActiv) {
            initwindow(n * l + 400, n * l + 250);
            setlinestyle(SOLID_LINE, 0, 2);

            yoff = n * l + 50;
            xrel = n * l + 100;

            if (limba == 1) { //romana
                initButon(b[0], "Piesa 1", n * l + 100, yrel*2+25, dimInGame);
                initButon(b[1], "Piesa 2", n * l + 200, yrel * 2+25, dimInGame);
                initButon(b[2], "Piesa 3", n * l + 300, yrel * 2+25, dimInGame);
                initButon(b[3], "Undo", n * l + 100, yrel * 4, dimInGame);
                initButon(b[4], "Redo", n * l + 200, yrel * 4, dimInGame);
                initButon(b[5], "Restart", n * l + 100, yrel * 5, dimInGame);
                initButon(b[6], "Meniu", n * l + 200, yrel * 5, dimInGame);
            } else if (limba == 2) { //engleza
                initButon(b[0], "Tile 1", n * l + 100, yrel*2+25, dimInGame);
                initButon(b[1], "Tile 2", n * l + 200, yrel * 2+25, dimInGame);
                initButon(b[2], "Tile 3", n * l + 300, yrel * 2+25, dimInGame);
                initButon(b[3], "Undo", n * l + 100, yrel * 4, dimInGame);
                initButon(b[4], "Redo", n * l + 200, yrel * 4, dimInGame);
                initButon(b[5], "Restart", n * l + 100, yrel * 5, dimInGame);
                initButon(b[6], "Menu", n * l + 200, yrel * 5, dimInGame);
            }

            for (contor = 0; contor < 7; contor++) deseneazaButon(b[contor]);
            piesa1(n*l+65,20);
            piesa2(n*l+165,20);
            piesa3(n*l+265,20);
            square(n*l+65,20);
            square(n*l+165,20);
            square(n*l+265,20);

            tabla[0][0] = 1;
            lee(C);
            deseneazaTabla();

            gata = false;
            while (!gata) {
                amPusPiesa = false;
                getmouseclick(WM_LBUTTONDOWN, xmouse, ymouse);
                i = xmouse / l;
                j = ymouse / l;
                if (limba == 1) {
                    outtextxy(xoff, yoff + 50, "Faceti click pe o zona valida (marcata cu cerc).");
                    if (gameMode == 1) outtextxy(xoff, yoff, "Este randul jucatorului.");
                    else if (gameMode == 2) {
                        if (player == 1) outtextxy(xoff, yoff, "Este randul jucatorului 2.");
                        else outtextxy(xoff, yoff, "Este randul jucatorului 1.");
                    }
                } else if (limba == 2) {
                    outtextxy(xoff, yoff + 50, "Select a valid area (square inscribed with a circle).");
                    if (gameMode == 1) outtextxy(xoff, yoff, "It is your turn.");
                    else if (gameMode == 2) {
                        if (player == 1) outtextxy(xoff, yoff, "It is the second player's turn.");
                        else outtextxy(xoff, yoff, "It is the first player's turn.");
                    }
                }
                if (xmouse > 0 && ymouse > 0) { // dau click
                    if (!esteAfara(i, j)) { // este inauntrul tablei
                        if (valid[i][j]) { // am selectat piesa valida
                            amPusPiesa = true;
                            setcolor(COLOR(255,100,0));
                            piesa0(i * l, j * l);
                            setcolor(WHITE);
                            if (limba == 1) outtextxy(xoff, yoff + 100, "Apasati pe butonul corespunzator piesei dorite.");
                            else if (limba == 2) outtextxy(xoff, yoff + 100, "Place a tile by pressing the corresponding button.");
                            player = 3 - player; // schimb playerul: 1 devine 2, 2 devine 1
                            while (1) {
                                getmouseclick(WM_LBUTTONDOWN, xmouse, ymouse);
                                if (xmouse > 0 && ymouse > 0) {
                                    for (contor = 0; contor < 3; contor++)
                                        if (xmouse >= b[contor].x1 && xmouse <= b[contor].x2 && ymouse >= b[contor].y1 && ymouse <= b[contor].y2)
                                            {tabla[i][j] = contor + 1; valid[i][j] = 0; push(S1, {i, j, contor + 1}); goleste(S2); gata = lee(C); break;}
                                    if (contor != 3) break;
                                }
                            }

                            if (!ales) {
                                ales = true;
                                if (tabla[0][1]) { // am ales calea prin dreapta
                                    alesDreapta = true;
                                    valid[1][0] = 0;
                                } else { // am ales calea prin jos
                                    alesDreapta = false;
                                    valid[0][1] = 0;
                                }
                            }
                        }
                    } else { // inafara tablei
                        if (xmouse >= b[3].x1 && xmouse <= b[3].x2 && ymouse >= b[3].y1 && ymouse <= b[3].y2) {
                            if (gameMode == 1) undo();
                            undo();
                        }
                        else if (xmouse >= b[4].x1 && xmouse <= b[4].x2 && ymouse >= b[4].y1 && ymouse <= b[4].y2) {
                            if (gameMode == 1) redo();
                            redo();
                        }
                        else if (xmouse >= b[5].x1 && xmouse <= b[5].x2 && ymouse >= b[5].y1 && ymouse <= b[5].y2) { // restart
                            goleste(S1); goleste(S2);
                            umple0(tabla); umple0(valid);
                            tabla[0][0] = 1;
                            ales = 0; alesDreapta = 0;
                            player = 1;
                            player1W = false; player1L = false; player2W = false; player2L = false;
                            lee(C);
                        }
                        else if (xmouse >= b[6].x1 && xmouse <= b[6].x2 && ymouse >= b[6].y1 && ymouse <= b[6].y2) { // restart
                            meniuActiv = true;
                            jocActiv = false;
                            goleste(S1); goleste(S2);
                            umple0(tabla); umple0(valid);
                            ales = 0; alesDreapta = 0;
                            player = 1;
                            player1W = false; player1L = false; player2W = false; player2L = false;
                            closegraph();
                            initwindow(800, 600);
                        }
                    }

                    if (!jocActiv) break;

                    cleardevice();
                    deseneazaTabla();
                    for (contor = 0; contor < 7; contor++) deseneazaButon(b[contor]);
                    piesa1(n*l+65,20);
                    piesa2(n*l+165,20);
                    piesa3(n*l+265,20);
                    square(n*l+65,20);
                    square(n*l+165,20);
                    square(n*l+265,20);

                    if (gata) break;

                    if (gameMode == 1 && amPusPiesa) {
                        if (limba == 1) outtextxy(xoff, yoff, "Este randul calculatorului.");
                        else if (limba == 2) outtextxy(xoff, yoff, "It is the computer's turn.");
                        player = 3 - player;
                        valRandom = rand() % 3 + 1;
                        tabla[ultimaPusa.lin][ultimaPusa.col] = valRandom;
                        valid[ultimaPusa.lin][ultimaPusa.col] = 0;
                        push(S1, {ultimaPusa.lin, ultimaPusa.col, valRandom});
                        goleste(S2);
                        gata = lee(C);
                        delay(1000);

                        cleardevice();
                        deseneazaTabla();
                        for (contor = 0; contor < 7; contor++) deseneazaButon(b[contor]);
                        piesa1(n*l+65,20);
                        piesa2(n*l+165,20);
                        piesa3(n*l+265,20);
                        square(n*l+65,20);
                        square(n*l+165,20);
                        square(n*l+265,20);
                    }
                }
            }

            if (gata) {
                if (limba == 1) {
                    if (gameMode == 1) {
                        if (player1W) outtextxy(xoff, yoff, "Calculatorul a castigat!");
                        if (player1L) outtextxy(xoff, yoff, "Calculatorul a pierdut!");
                        if (player2W) outtextxy(xoff, yoff, "Ati castigat!");
                        if (player2L) outtextxy(xoff, yoff, "Ati pierdut!");
                    } else if (gameMode == 2) {
                        if (player1W) outtextxy(xoff, yoff, "Jucatorul 1 a castigat!");
                        if (player1L) outtextxy(xoff, yoff, "Jucatorul 1 a pierdut!");
                        if (player2W) outtextxy(xoff, yoff, "Jucatorul 2 a castigat!");
                        if (player2L) outtextxy(xoff, yoff, "Jucatorul 2 a pierdut!");
                    }
                    outtextxy(xoff, yoff + 50, "Apasati orice tasta pentru a reveni la meniu.");
                } else if (limba == 2) {
                    if (gameMode == 1) {
                        if (player1W) outtextxy(xoff, yoff, "The computer won!");
                        if (player1L) outtextxy(xoff, yoff, "The computer lost!");
                        if (player2W) outtextxy(xoff, yoff, "You won!");
                        if (player2L) outtextxy(xoff, yoff, "You lost!");
                    } else if (gameMode == 2) {
                        if (player1W) outtextxy(xoff, yoff, "Player 1 won!");
                        if (player1L) outtextxy(xoff, yoff, "Player 1 lost!");
                        if (player2W) outtextxy(xoff, yoff, "Player 2 won!");
                        if (player2L) outtextxy(xoff, yoff, "Player 2 lost!");
                    }
                    outtextxy(xoff, yoff + 50, "Press any key to go back to the menu.");
                }
                getch();
                meniuActiv = true;
                jocActiv = false;
                goleste(S1); goleste(S2);
                umple0(tabla); umple0(valid);
                ales = 0; alesDreapta = 0;
                player = 1;
                player1W = false; player1L = false; player2W = false; player2L = false;
                closegraph();
                initwindow(800, 600);
            }
        }
    }
    closegraph();
    return 0;
}

void redo() {
    if (isEmpty(S2)) return;
    player = 3 - player;
    elementStiva e = top(S2);
    pop(S2);

    tabla[e.lin][e.col] = e.tip;
    valid[e.lin][e.col] = 0;

    push(S1, e);

    if (!ales) {
        ales = true;
        if (tabla[0][1]) { // am ales calea prin dreapta
            alesDreapta = true;
            valid[1][0] = 0;
        } else { // am ales calea prin jos
            alesDreapta = false;
            valid[0][1] = 0;
        }
    }

    lee(C);
}

void undo() {
    if (isEmpty(S1)) return;
    player = 3 - player;
    elementStiva e = top(S1);
    pop(S1);

    valid[ultimaPusa.lin][ultimaPusa.col] = 0;
    ultimaPusa = e;

    tabla[e.lin][e.col] = 0;
    valid[e.lin][e.col] = 1;

    push(S2, e);

    if (isEmpty(S1)) {
        ales = alesDreapta = false;
        valid[0][1] = 1;
        valid[1][0] = 1;
    }
}

bool lee(coada & C) {
    initLee(C);
    while (!isEmpty(C)) {
        piesaLee p = read(C);
        pop(C);

        if (p.dir == 'u') { // vin de sus, deci ma duc jos
            if (p.lin + 1 == n - 1 && p.col == n - 1) {player == 1 ? player1W = true : player2W = true; return true;}
            if (esteAfara(p.lin + 1, p.col)) {player == 1 ? player1L = true : player2L = true; return true;}

            if (tabla[p.lin + 1][p.col] == 0) {valid[p.lin + 1][p.col] = 1; ultimaPusa = {p.lin + 1, p.col, 0};}
            else {
                if (tabla[p.lin + 1][p.col] == 1) push(C, {p.lin + 1, p.col, 1, 'u'});
                else if (tabla[p.lin + 1][p.col] == 2) push(C, {p.lin + 1, p.col, 2, 'l'});
                else if (tabla[p.lin + 1][p.col] == 3) push(C, {p.lin + 1, p.col, 3, 'r'});
            }
        }
        else if (p.dir == 'd') { // vin din jos, ma duc in sus
            if (esteAfara(p.lin - 1, p.col)) {player == 1 ? player1L = true : player2L = true; return true;}

            if (tabla[p.lin - 1][p.col] == 0) {valid[p.lin - 1][p.col] = 1; ultimaPusa = {p.lin - 1, p.col, 0};}
            else {
                if (tabla[p.lin - 1][p.col] == 1) push(C, {p.lin - 1, p.col, 1, 'd'});
                else if (tabla[p.lin - 1][p.col] == 2) push(C, {p.lin - 1, p.col, 2, 'r'});
                else if (tabla[p.lin - 1][p.col] == 3) push(C, {p.lin - 1, p.col, 3, 'l'});
            }
        }
        else if (p.dir == 'l') { // vin din stanga, deci ma duc in dreapta
            if (p.lin == n - 1 && p.col + 1 == n - 1) {player == 1 ? player1W = true : player2W = true; return true;}
            if (esteAfara(p.lin, p.col + 1)) {player == 1 ? player1L = true : player2L = true; return true;}

            if (tabla[p.lin][p.col + 1] == 0) {valid[p.lin][p.col + 1] = 1; ultimaPusa = {p.lin, p.col + 1, 0};}
            else {
                if (tabla[p.lin][p.col + 1] == 1) push(C, {p.lin, p.col + 1, 1, 'l'});
                else if (tabla[p.lin][p.col + 1] == 2) push(C, {p.lin, p.col + 1, 2, 'u'});
                else if (tabla[p.lin][p.col + 1] == 3) push(C, {p.lin, p.col + 1, 3, 'd'});
            }
        }
        else if (p.dir == 'r') { // vin din dreapta, deci ma duc in stanga
            if (esteAfara(p.lin, p.col - 1)) {player == 1 ? player1L = true : player2L = true; return true;}

            if (tabla[p.lin][p.col - 1] == 0) {valid[p.lin][p.col - 1] = 1; ultimaPusa = {p.lin, p.col - 1, 0};}
            else {
                if (tabla[p.lin][p.col - 1] == 1) push(C, {p.lin, p.col - 1, 1, 'r'});
                else if (tabla[p.lin][p.col - 1] == 2) push(C, {p.lin, p.col - 1, 2, 'd'});
                else if (tabla[p.lin][p.col - 1] == 3) push(C, {p.lin, p.col - 1, 3, 'u'});
            }
        }
    }

    return false;
}
