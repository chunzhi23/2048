#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

int score = 0;
int best_score = 0;

int board[4][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};

void title(int x, int y);
void homepage(void);
void getBestScore(void);
void play(void);
void printInterface(void);
void printBoard(void);

void goLeft(void);
void goRight(void);
void goUp(void);
void goDown(void);

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
    homepage();
    play();
}

void homepage(void){
    int x=5, y=3;

    title(x,y);
    
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

void title(int x, int y){
    gotoxy(x,y);   printf("﹥﹥﹥  ﹥﹥﹥  ﹥  ﹥  ﹥﹥﹥");Sleep(50);
    gotoxy(x,y+1); printf("    ﹥  ﹥  ﹥  ﹥  ﹥  ﹥  ﹥");Sleep(50);
    gotoxy(x,y+2); printf("﹥﹥﹥  ﹥  ﹥  ﹥﹥﹥  ﹥﹥﹥");Sleep(50);
    gotoxy(x,y+3); printf("﹥      ﹥  ﹥      ﹥  ﹥  ﹥");Sleep(50);
    gotoxy(x,y+4); printf("﹥﹥﹥  ﹥﹥﹥      ﹥  ﹥﹥﹥");Sleep(50);
}

void getBestScore(){
    FILE *file = fopen("score.bat", "rt");
    if (file == 0) best_score=0;
    else {
        fscanf(file, "%d", &best_score);
    }
    fclose(file);
}

void printInterface(){
    int x=5, y=3;
    
    title(x,y);

    gotoxy(x+20,y+8); printf("≧ Score : %d", score);
    gotoxy(x+20,y+9); printf("≠ Best Score : %d", best_score);

    gotoxy(x+20,y+11);  printf("Operation Keys :");
    gotoxy(x+22,y+13); printf("﹦   : Pull Up");
    gotoxy(x+20,y+14); printf("８  Ⅰ : Pull Left & Right");
    gotoxy(x+22,y+15); printf("∩   : Pull Down");

}

void printBoard(){
    int x=6, y=10;

    gotoxy(x,y);   printf("忙式式式成式式式成式式式成式式式忖");
    gotoxy(x,y+1); printf("弛   弛   弛   弛   弛");
    gotoxy(x,y+2); printf("戍式式式托式式式托式式式托式式式扣");
    gotoxy(x,y+3); printf("弛   弛   弛   弛   弛");
    gotoxy(x,y+4); printf("戍式式式托式式式托式式式托式式式扣");
    gotoxy(x,y+5); printf("弛   弛   弛   弛   弛");
    gotoxy(x,y+6); printf("戍式式式托式式式托式式式托式式式扣");
    gotoxy(x,y+7); printf("弛   弛   弛   弛   弛");
    gotoxy(x,y+8); printf("曲式式式扛式式式扛式式式扛式式式戎");

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            gotoxy(x+j*4+2, y+i*2+1); printf("%d", board[i][j]);
        }
    }
}

void play(){
    char e;

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

        printInterface();
        printBoard();
        
        while (1){
            e = _getch();
            switch (e){
                case 75:
                    goLeft();
                    break;
                case 77:
                    goRight();
                    break;
                case 72:
                    goUp();
                    break;
                case 80:
                    goDown();
                    break;
                default: continue;
            }
            break;
        }
    } while (1);
    Sleep(10000);
}

void goLeft(){
    printf("Left");
}

void goRight(){
    printf("Right");
}

void goUp(){
    printf("Up");
}

void goDown(){
    printf("Down");
}