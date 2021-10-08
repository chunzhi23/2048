#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#define ESC 27

int score;
int best_score;

int board_SIZE = 4;
int board[10][10]; //Global variables are automatically set to 0
bool flag_MOVED; //variable checking which the numbers have moved


void title(int x, int y);
void homepage(void);
void getBestScore(void);
void play(void);
void printInterface(void);
void printBoard(void);
void gameover(void);
void reset(void);

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
    system("chcp 65001  > nul");
    setCursorType(NOCURSOR);
    system("title 2048");
    homepage();
}

void homepage(void){
    int x=5, y=3, e;

    title(x,y);

    gotoxy(x+2,y+6); printf("- Press anything to start -");

    gotoxy(x+0,y+9);  printf("Operation Keys :");
    gotoxy(x+2,y+11); printf("△   : Pull Up");
    gotoxy(x+0,y+12); printf("◁  ▷ : Pull Left & Right");
    gotoxy(x+2,y+13); printf("▽   : Pull Down");

    getBestScore();
    gotoxy(x+32,y+9);  printf("Top Score :");
    gotoxy(x+32,y+11); printf("★ Best : %d", best_score);

    gotoxy(x+32,y+14); printf("User Interface : ");

    while(true){
        gotoxy(x+32,y+16); if(board_SIZE==10)printf("△");else printf("▲");
        gotoxy(x+32,y+17); printf("%2d",board_SIZE);
        gotoxy(x+32,y+18); if(board_SIZE==3)printf("▽");else printf("▼");
        //while (kbhit()) getch(); no need
        int _e = getch();
        if(_e==224 || _e == 0)e = getch(); //exception for extended keys (including arrow keys)
        //printf("%d",e); // for debug use
        if (_e == ESC){
            system("cls");
            exit(0);
        }else if (e == 72 && _e==224){
            if(board_SIZE!=10)board_SIZE++;
        }else if (e == 80 && _e==224){
            if(board_SIZE!=3)board_SIZE--;
        }else break;
	}
    //system("pause"); //for debug use
    system("cls");
    play();
}

void title(int x, int y){
    gotoxy(x,y);   printf("■■■  ■■■  ■  ■  ■■■");Sleep(50);
    gotoxy(x,y+1); printf("    ■  ■  ■  ■  ■  ■  ■");Sleep(50);
    gotoxy(x,y+2); printf("■■■  ■  ■  ■■■  ■■■");Sleep(50);
    gotoxy(x,y+3); printf("■      ■  ■      ■  ■  ■");Sleep(50);
    gotoxy(x,y+4); printf("■■■  ■■■      ■  ■■■");Sleep(50);
}

void getBestScore(){
    FILE *file = fopen("score.dat", "rt");
    if (file == 0) best_score=0;
    else {
        fscanf(file, "%d", &best_score);
    }
    fclose(file);
}

void printInterface(){
    int x=5, y=3;

    title(x,y);

    gotoxy(x+20+(board_SIZE-4)*4,y+8); printf("☆ Score : %d", score);
    gotoxy(x+20+(board_SIZE-4)*4,y+9); printf("★ Best Score : %d", best_score);

    gotoxy(x+20+(board_SIZE-4)*4,y+11);  printf("Operation Keys :");
    gotoxy(x+22+(board_SIZE-4)*4,y+13); printf("△   : Pull Up");
    gotoxy(x+20+(board_SIZE-4)*4,y+14); printf("◁  ▷ : Pull Left & Right");
    gotoxy(x+22+(board_SIZE-4)*4,y+15); printf("▽   : Pull Down");

}

void printBoardTop(int SIZE){
    printf("┌");
    for(int i = 0; i < SIZE - 1 ; i++)printf("───┬");
    printf("───┐");
    return;
}

void printBoardMid(int SIZE){
    printf("├");
    for(int i = 0; i < SIZE - 1 ; i++)printf("───┼");
    printf("───┤");
    return;
}

void printBoardBottom(int SIZE){
    printf("└");
    for(int i = 0; i < SIZE - 1 ; i++)printf("───┴");
    printf("───┘");
    return;
}

void printBoard(){
    int x=6, y=11, i, j;

    gotoxy(x,y);   printBoardTop(board_SIZE);
    gotoxy(x,y+1); printf("│");for(i = 0; i < board_SIZE; i++)printf("   │");
    for(i = 2; i < board_SIZE * 2; i += 2){
        gotoxy(x,y+i); printBoardMid(board_SIZE);
        gotoxy(x,y+i+1); printf("│");for(j = 0; j < board_SIZE; j++)printf("   │");
    }
    gotoxy(x,y+(board_SIZE*2)-1); printf("│");for(i = 0; i < board_SIZE; i++)printf("   │");
    gotoxy(x,y+(board_SIZE*2)); printBoardBottom(board_SIZE);

    for (i = 0; i < board_SIZE; i++){
        for (j = 0; j < board_SIZE; j++){
            if (board[i][j] == 0) continue;
            gotoxy(x+j*4+2, y+i*2+1); printf("%d", board[i][j]);
        }
    }
}

typedef struct _coord{
    int x;
    int y;
} coord;

bool checkBoardCoord(coord c){
    int target = board[c.y][c.x];

    coord cx = c;
    cx.x++;
    coord cy = c;
    cy.y++;

    bool flag=false;

    if(c.x != board_SIZE-1) flag = (board[c.y][c.x+1] == target) ? true : checkBoardCoord(cx);
    if(c.y != board_SIZE-1 && flag == false) flag = (board[c.y+1][c.x] == target) ? true : checkBoardCoord(cy);
    return flag;
}

bool checkGameOver(){
    coord p;
    p.x=0;
    p.y=0;
    return checkBoardCoord(p);
}

void play(){
    char e;
    bool firstplay = true;
    do {
        int empty[16][2] = {0}, tmp=0, i, j;
        for (i = 0; i < board_SIZE; i++){
            for (j = 0; j < board_SIZE; j++){
                if (board[i][j] == 0){
                    empty[tmp][0] = i;
                    empty[tmp][1] = j;
                    tmp++;
                }
            }
        }

        if (tmp == 0) if(!checkGameOver()) break;

        srand(time(NULL));
        int pos1 = rand() % tmp;
        int pos2 = rand() % tmp;
        while(pos1 == pos2 && tmp != 1 && firstplay == true){
            pos2 = rand() % tmp;
        }

        if(firstplay == true){
            board[empty[pos1][0]][empty[pos1][1]] = 2;
            board[empty[pos2][0]][empty[pos2][1]] = 2;
            firstplay = false;
        }else if(tmp!=0){
            if(rand() % 5 != 0)board[empty[pos1][0]][empty[pos1][1]] = 2;
            else board[empty[pos1][0]][empty[pos1][1]] = 4;
            tmp--;
        }

        printInterface();
        printBoard();

        if(tmp == 0) if(!checkGameOver()) break;
        flag_MOVED = false;
        while (1){
            e = getch();
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
                case ESC:
                	system("cls");
                	exit(0);
                	break;
                default:
                    continue;
            }
            if(flag_MOVED) break;
        }
        if (score > best_score) best_score = score;
    } while (1);
    gameover();
}

void goLeft(){
	int i, j;
    for (i = 0; i < board_SIZE; i++){
        for (j = 1; j < board_SIZE; j++){
            int x=j, y=i;
            if(board[y][x] == 0) continue; //optimization
            while (board[y][x-1] == 0 && x > 0){
                flag_MOVED = true;
                board[y][x-1] = board[y][x];
                board[y][x--] = 0;
            }
        }
    }

    for (i = 0; i < board_SIZE; i++){
        for (j = 0; j < board_SIZE; j++){
            if (board[i][j] == 0) continue;
            if (board[i][j] == board[i][j+1] && j < 3){
                flag_MOVED = true;
                score += board[i][j] *= 2;
                board[i][j+1] = 0;
            }
        }
    }

    for (i = 0; i < board_SIZE; i++){
        for (j = 1; j < board_SIZE; j++){
            int x=j, y=i;
            if(board[y][x] == 0)continue;
            while (board[y][x-1] == 0 && x > 0){
                flag_MOVED = true;
                board[y][x-1] = board[y][x];
                board[y][x--] = 0;
            }
        }
    }
}

void goRight(){
	int i, j;
    for (i = 0; i < board_SIZE; i++){
        for (j = board_SIZE-2; j >= 0; j--){
            int x=j, y=i;
            if(board[y][x] == 0)continue;
            while (board[y][x+1] == 0 && x < board_SIZE-1){
                flag_MOVED = true;
                board[y][x+1] = board[y][x];
                board[y][x++] = 0;
            }
        }
    }

    for (i = 0; i < board_SIZE; i++){
        for (j = board_SIZE-1; j >= 0; j--){
            if (board[i][j] == 0) continue;
            if (board[i][j] == board[i][j-1] && j > 0){
                flag_MOVED = true;
                score += board[i][j] *= 2;
                board[i][j-1] = 0;
            }
        }
    }

    for (i = 0; i < board_SIZE; i++){
        for (j = board_SIZE-2; j >= 0; j--){
            int x=j, y=i;
            if(board[y][x] == 0)continue;
            while (board[y][x+1] == 0 && x < board_SIZE-1){
                flag_MOVED = true;
                board[y][x+1] = board[y][x];
                board[y][x++] = 0;
            }
        }
    }
}

void goUp(){
	int i, j;
    for (i = 1; i < board_SIZE; i++){
        for (j = 0; j < board_SIZE; j++){
            int x=j, y=i;
            if(board[y][x] == 0)continue;
            while (board[y-1][x] == 0 && y > 0){
                flag_MOVED = true;
                board[y-1][x] = board[y][x];
                board[y--][x] = 0;
            }
        }
    }

    for (i = 0; i < board_SIZE; i++){
        for (j = 0; j < board_SIZE; j++){
            if (board[i][j] == 0) continue;
            if (board[i][j] == board[i+1][j] && i < board_SIZE-1){
                flag_MOVED = true;
                score += board[i][j] *= 2;
                board[i+1][j] = 0;
            }
        }
    }

    for (i = 1; i < board_SIZE; i++){
        for (j = 0; j < board_SIZE; j++){
            int x=j, y=i;
            if(board[y][x] == 0)continue;
            while (board[y-1][x] == 0 && y > 0){
                flag_MOVED = true;
                board[y-1][x] = board[y][x];
                board[y--][x] = 0;
            }
        }
    }
}

void goDown(){
	int i, j;
    for (i = board_SIZE-2; i >= 0; i--){
        for (j = 0; j < board_SIZE; j++){
            int x=j, y=i;
            if(board[y][x] == 0)continue;
            while (board[y+1][x] == 0 && y < board_SIZE-1){
                flag_MOVED = true;
                board[y+1][x] = board[y][x];
                board[y++][x] = 0;
            }
        }
    }

    for (i = board_SIZE-1; i >= 0; i--){
        for (j = 0; j < board_SIZE; j++){
            if (board[i][j] == 0) continue;
            if (board[i][j] == board[i-1][j] && i > 0){
                flag_MOVED = true;
                score += board[i][j] *= 2;
                board[i-1][j] = 0;
            }
        }
    }

    for (i = board_SIZE-2; i >= 0; i--){
        for (j = 0; j < board_SIZE; j++){
            int x=j, y=i;
            if(board[y][x] == 0)continue;
            while (board[y+1][x] == 0 && y < board_SIZE-1){
                flag_MOVED = true;
                board[y+1][x] = board[y][x];
                board[y++][x] = 0;
            }
        }
    }
}

void gameover(){
    int x=5, y=3;
    char e;

    gotoxy(x,y);   printf("  ■■■    ■■    ■      ■  ■■■■      ■■■■  ■      ■  ■■■■  ■■■■"); Sleep(50);
    gotoxy(x,y+1); printf("■        ■    ■  ■■  ■■  ■            ■    ■  ■      ■  ■        ■    ■"); Sleep(50);
    gotoxy(x,y+2); printf("■  ■■  ■■■■  ■  ■  ■  ■■■■      ■    ■  ■      ■  ■■■■  ■■■"); Sleep(50);
    gotoxy(x,y+3); printf("■    ■  ■    ■  ■      ■  ■            ■    ■    ■  ■    ■        ■    ■"); Sleep(50);
    gotoxy(x,y+4); printf("■■■■  ■    ■  ■      ■  ■■■■      ■■■■      ■      ■■■■  ■    ■"); Sleep(100);

    if (score == best_score && score != 0){
        gotoxy(x+2,y+6); printf("★☆★ New Best Score!! ★☆★");

        FILE* file = fopen("score.dat", "wt");
        fprintf(file, "%d", score);
    }
    Sleep(3000);

    gotoxy(x+2,y+6); printf("Press 'y' to try again, 'n' to exit the game.");

    while (kbhit()) getch();
    e = getch();
    if (e == 'y') reset();
    else system("exit");
}

void reset(){
	int i, j;
    system("cls");
    score = 0;
    for (i = 0; i < board_SIZE; i++){
        for (j = 0; j < board_SIZE; j++){
            board[i][j] = 0;
        }
    }

    homepage();
}
