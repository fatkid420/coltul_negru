#define MAX 50

struct buton
{
    char text[MAX];
    int x,y;
    int marime;
    int x1,y1,x2,y2;
};

void initButon(buton& b, char text0[MAX], int x0, int y0,int m0)
{
    strcpy(b.text,text0);
    b.x=x0; b.y=y0;b.marime=m0;
    settextstyle(SIMPLEX_FONT,HORIZ_DIR,b.marime);
    b.x1=x0-textwidth(b.text)/2-5;
    b.x2=x0+textwidth(b.text)/2+5;
    b.y1=y0-textheight(b.text)/2-5;
    b.y2=y0+textheight(b.text)/2+5;
}

void deseneazaButon(buton b,bool esteTitlu = false)
{
    settextstyle(SIMPLEX_FONT,HORIZ_DIR,b.marime);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    if(esteTitlu==0) {
       outtextxy((b.x1+b.x2)/2+2,(b.y1+b.y2)/2+4,b.text);
       rectangle(b.x1, b.y1, b.x2, b.y2);
    } else outtextxy((b.x1+b.x2)/2+3,(b.y1+b.y2)/2+5,b.text);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    settextstyle(DEFAULT_FONT,HORIZ_DIR,0);
}
