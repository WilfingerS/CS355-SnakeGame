#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
/*
to compile: gcc snake.c -o snake -l curses
*/
// functions initialization
void createBorders();
void createSnake();
void input();
void movesnake();
void collision();
void genTrophy();
void calcSpeed();
void updateData();
void getData();

int getRand(int max,int min);

struct vector2{
    int x;
    int y;
};

// Snake info; (i wonder if we should allow diagonal movement?) (probably not)
int snakeLen = 3; // initial length
struct vector2 moveDirection; // this will be a vector 2;
struct vector2 snake[128]; // contains info on the actual snake
struct vector2 trophyPos;

// Game info
int isgameWon = 0; // (bool) exit if game won
int isgameRun = 1; // (bool) exit if game ended
int isHighscore = 0;
int snakeSpeed = 1000000/5; // 1/4 sec for uspeed
int highscore = 0;
char trophyChar;
char head = 'O'; // Snake's Head
char body = 'o'; // Snake's Body

int main(){
    // Get Highscore
    getData();
    // seeds rand number generator with current time atm
    srand(time(NULL)); 
    
    // Start Creating Game
    initscr();
    cbreak();             // Disable line buffering
    noecho();             // Don't echo input characters
    curs_set(0);          // Hide the cursor
    nodelay(stdscr,TRUE);
    keypad(stdscr,TRUE);
    refresh();
    clear();
    attrset(A_NORMAL);

    mvprintw(0,0,"Score: %d\tHighscore: %d\tTargetScore:%d",snakeLen-3,highscore,(COLS+LINES));

    createBorders();
    createSnake();
    genTrophy();

    do {
        // Here we should probably
        input();        // get input -> changes move Direction based off input: wasd/arrowkeys
        collision();    // check collisions
        movesnake();      // move snake -> move the snake head and the body follows the position it was last in
        move(LINES-1,COLS-1);
        refresh();
        if (moveDirection.y != 0){
            snakeSpeed *= 1.1;
        } 
        usleep(snakeSpeed); // sleep based on size
        if (moveDirection.y != 0)
            calcSpeed(); // recalc
    }while(isgameRun != 0);

    // Show Win/Lose
    char str[100];
    if (isgameWon == 1){ // PLAYER WON!
        strcpy(str, "YOU WON!");
    }else{// Show Player Lost
        strcpy(str,"YOU LOST!");
    }

    mvaddstr(LINES/2,(COLS - strlen(str))/2,str);
    if (isHighscore == 1){
        updateData();
        mvaddstr(LINES/2+1,COLS/2-7,"NEW HIGHSCORE!");
    }
    refresh();

    sleep(5);
    curs_set(1);
    endwin();
    return 0;
}
void getData(){
    FILE *data = fopen("data.dat", "r+");
    char buffer[128];
    if(data){
        fscanf(data,"%s",buffer);
        highscore = atoi(buffer);
    }else{
        data = fopen("data.dat", "w+");
        strcpy(buffer,"0");
        fprintf(data,buffer,sizeof(buffer));
    }

    fclose(data);
}
void updateData(){
    FILE *data = fopen("data.dat","w");
    fprintf(data,"%d",highscore);
    fclose(data);
}

void genTrophy(){
    int rand = getRand(9, 1);
    trophyChar = rand + '0';
    int isValidPos;
    do{
        isValidPos = 0;
        trophyPos.y = getRand(LINES-2, 2);
        trophyPos.x = getRand(COLS -2, 1);
        for (int i = 0; i < snakeLen; i++){
            if (snake[i].x == trophyPos.x && snake[i].y == trophyPos.y){
                isValidPos = 1;
                break;
            }
        }
    }while(isValidPos != 0);
    mvaddch(trophyPos.y,trophyPos.x,trophyChar);
    refresh();
}

void calcSpeed(){
    snakeSpeed = (1000000/5) - ((2500) * (snakeLen-3));
    if (snakeSpeed < 75000)
        snakeSpeed = 75000; // cant go lower than this
}

void input(){
    int ch = getch();
    if (ch == 'w' || ch == KEY_UP){
        if (moveDirection.y == 1)
            return;
        moveDirection.x = 0;
        moveDirection.y = -1;
        head = '^';
    }else if(ch == 's' || ch == KEY_DOWN){
        if (moveDirection.y == -1)
            return;
        moveDirection.x = 0;
        moveDirection.y = 1;
        head = 'v';
    }else if(ch == 'd' || ch == KEY_RIGHT){
        if (moveDirection.x == -1)
            return;
        moveDirection.x = 1;
        moveDirection.y = 0;
        head = '>';
    }else if(ch == 'a' || ch == KEY_LEFT){
        if (moveDirection.x == 1)
            return;
        moveDirection.x = -1;
        moveDirection.y = 0;
        head = '<';
    }
    
}

void collision(){
    move(snake[0].y + moveDirection.y,snake[0].x + moveDirection.x);
    chtype chT = winch(stdscr);
    char ch = chT & A_CHARTEXT; // get the character of where the head in going

    if (ch == body || ch == '#'){
        isgameRun = 0;
    }else if(ch != ' ' ){
        // Assume the snake ran into a trophy
        int trophyVal = ch - '0';
        int old = snakeLen;
        snakeLen += trophyVal;
        for (int i = old; i<snakeLen; i++){
            snake[i].x = snake[i-1].x;
            snake[i].y = snake[i-1].y;
        }
        genTrophy();
        calcSpeed();
        // check newHighscore? on interaction with char
        if ((snakeLen-3) > highscore){
            highscore = snakeLen-3;
            isHighscore = 1;
        }

        if (snakeLen >= (LINES + COLS) ) // half the perimeter
            isgameWon = 1;


        // Update Scoreboard
        mvprintw(0,0,"Score: %d\tHighscore: %d\tTargetScore:%d",snakeLen-3,highscore,(COLS+LINES));
    }
    
}

void movesnake(){
    struct vector2 prev;
    for (int i = 0; i < snakeLen; i++){
        int tempx = snake[i].x;
        int tempy = snake[i].y;
        char bodypart = body;
        if (i == 0){ //Snake Head
            bodypart = head;
            prev.x = tempx + moveDirection.x;
            prev.y = tempy  + moveDirection.y;
        }

        snake[i].x = prev.x;
        snake[i].y = prev.y;
        prev.x = tempx;
        prev.y = tempy;
        mvaddch(snake[i].y, snake[i].x,bodypart);
        mvaddch(prev.y,prev.x,' ');
    }
}

int getRand(int max,int min){ // Seth
    // returns random number in between the max and min
    return (rand() % (max - min + 1)) + min;
}

void createBorders(){ // Seth
    // LINES COLS globals
    // x borders: x == 0 || x == LINES-1
    // y borders: y == 0 || y == COLS-1
    for (int x = 0; x<COLS; x++){ // No need to nested loop cause then you are looping over unecessary positions on the screen
        for (int y = 1; y<LINES; y++){
            if ((x == 0 || x == COLS-1) || (y == 1 || y == LINES -1)){ // Same comment as above but this is just columns
                mvaddch(y,x,'#');
            }
        }
    }
}

void createSnake(){ // Seth
    // Get Random Starting moveDirection
    int r = getRand(4, 1);
    switch (r) { // random direction
        case 1:
            moveDirection.x = 0;
            moveDirection.y = -1;
            head = '^';
            break;
        case 2:
            moveDirection.x = 0;
            moveDirection.y = 1;
            head = 'v';
            break;
        case 3:
            moveDirection.x = 1;
            moveDirection.y = 0;
            head = '>';
            break;
        case 4:
            moveDirection.x = -1;
            moveDirection.y = 0;
            head = '<';
            break;
    }

    // Move to Center and Add Head
    int curX = COLS/2;
    int curY = LINES/2;
    snake[0].x = curX;
    snake[0].y = curY; // Sets the Head to center
    mvaddch(snake[0].y,snake[0].x,head);

    for (int i = 1; i < snakeLen; i++){ // Build the rest of the body in the opposite of the move direction
        snake[i].x = snake[i-1].x - moveDirection.x;
        snake[i].y = snake[i-1].y - moveDirection.y;
        mvaddch(snake[i].y, snake[i].x,body);
    }

    refresh();
}