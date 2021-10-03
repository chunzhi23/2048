#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

int score = 0;
int best_score = 0;

int board[4][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};

void title(void);
void getBestScore(void);
void play(void);
void printBoard(void);

void gotoxy(int x, int y){
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// reference: https://m.blog.naver.com/azure0777/220295388741
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
void setCursorType(CURSOR_TYPE e){
     CONSOLE_CURSOR_INFO CurInfo;
 
     switch (e) {
     case NOCURSOR:
          CurInfo.dwSize=1;
          CurInfo.bVisible=FALSE;
          break;
     case SOLIDCURSOR:
          CurInfo.dwSize=100;
          CurInfo.bVisible=TRUE;
          break;
     case NORMALCURSOR:
          CurInfo.dwSize=20;
          CurInfo.bVisible=TRUE;
          break;
     }
     SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo);
}

int main(){
    setCursorType(NOCURSOR);
    system("title 2048");
    title();
    play();
}

void title(void){
    int x=5, y=3;

    gotoxy(x,y);   printf("﹥﹥﹥  ﹥﹥﹥  ﹥  ﹥  ﹥﹥﹥");Sleep(50);
    gotoxy(x,y+1); printf("    ﹥  ﹥  ﹥  ﹥  ﹥  ﹥  ﹥");Sleep(50);
    gotoxy(x,y+2); printf("﹥﹥﹥  ﹥  ﹥  ﹥﹥﹥  ﹥﹥﹥");Sleep(50);
    gotoxy(x,y+3); printf("﹥      ﹥  ﹥      ﹥  ﹥  ﹥");Sleep(50);
    gotoxy(x,y+4); printf("﹥﹥﹥  ﹥﹥﹥      ﹥  ﹥﹥﹥");Sleep(50);
    
    gotoxy(x+2,y+6); printf("- Press anything to start -");

    gotoxy(x+0,y+9);  printf("Operation Keys :");
    gotoxy(x+2,y+11); printf("﹦   : Pull Up");
    gotoxy(x+0,y+12); printf("８  Ⅰ : Pull Left & Right");
    gotoxy(x+2,y+13); printf("∩   : Pull Down");

    getBestScore();
    gotoxy(x+32,y+9);  printf("Top Score :");
    gotoxy(x+32,y+11); printf("≠ Best : %d", best_score);

    while (1)
        if (kbhit()) break;
    
    system("cls");
}

void getBestScore(){
    FILE *file = fopen("score.bat", "rt");
    if (file == 0) best_score=0;
    else {
        fscanf(file, "%d", &best_score);
    }
    fclose(file);
}

void play(){
    do {
        int empty[16][2] = {}, tmp=0;
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                if (board[i][j] == 0){
                    empty[tmp][0] = j;
                    empty[tmp][1] = i;
                    tmp++;
                }
            }
        }

        srand((unsigned int)time(NULL));

        int point1 = rand() % tmp;
        int point2 = rand() % tmp;
        
        board[empty[point1][0]][empty[point1][1]] = 2;
        board[empty[point2][0]][empty[point2][1]] = 2;

        printBoard();
        break;
    } while (1);
    system("pause");
}

void printBoard(){
    int x=5, y=3;

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            gotoxy(x+j*5, y+i*3);   printf("忙式式式忖");
            gotoxy(x+j*5, y+i*3+1); if (board[i][j] == 0) printf("弛   弛"); 
                                    else printf("弛 %d 弛", board[i][j]);
            gotoxy(x+j*5, y+i*3+2); printf("戌式式式戎");
        }
    }
}