#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
/*
to compile: gcc snake.c -o snake -l curses

    references:
        https://cs.ccsu.edu/~stan/classes/CS355/notes/09-EventDrivenProgramming.html
        https://cs.ccsu.edu/~stan/classes/CS355/notes/08-TerminalControl.html
        https://cs.ccsu.edu/~stan/classes/CS355/notes/10-VideoGameProgramming.html
    
    Terminal modes:
        canonical (cooked): buffered until user presses enter
        non-canonical (crmode): no buffering but driver handles ctrl-c and translating between new line and carriage return
        non-anything (raw): driver passes input directly into prgrm
        tcsetattr() -> setting specific terminal mode

    curses.h:
        allows programmer to set pos of the cursor and
        control appearance of text on terminal screen
        screen: 2d array with 0,0 being upper left
        uses virtual screen where all changes are made via refresh()

        initscr() initializes curseslib and tty
        endwin() turns off curses lib and tty
        refresh() updates screen
        move(r,c) moves cursor to position (r,c)
        addstr(s) draws string s on the screen in the current position
        addch(c) draws char c on the screen in current position
        clear() clears the screen


    time prgraming:  
        alarm();
        sleep(n: unsigned int) :> suspend thread execution for interval

    signals:
        sync: inside process (division by 0)
        async: outside proccess (Ctrl-C)

*/
// functions initialization
void createBorders();
void createSnake();
void input();
void movesnake();
void collision();
int getRand(int max,int min);

struct vector2{
    int x;
    int y;
};

// Snake info; (i wonder if we should allow diagonal movement?) (probably not)
int snakeLen = 5; // initial length
struct vector2 moveDirection; // this will be a vector 2;
struct vector2 snake[128]; // contains info on the actual snake
struct vector2 trophyPos;

// Game info
int isgameWon = 0; // (bool) exit if game won
int isgameRun = 1; // (bool) exit if game ended
int winCondition;
char trophy;
char head = 'X'; // Snake's Head
char body = '*'; // Snake's Body

int main(){
    srand(time(NULL)); // seeds rand number generator with current time atm
    // Start Creating Game
    initscr();
    createBorders();
    createSnake();
    winCondition = 2*(LINES + COLS);

    cbreak();             // Disable line buffering
    noecho();             // Don't echo input characters
    curs_set(0);          // Hide the cursor
    nodelay(stdscr,TRUE);

    do {
        // Here we should probably
        input();        // get input -> changes move Direction based off input: wasd/arrowkeys
        collision();    // check collisions
        movesnake();      // move snake -> move the snake head and the body follows the position it was last in
        refresh();
        usleep(1000000/10); // microseconds = 1sec => 1,000,000 micro
        // (1/2) of a second ^
    }while(isgameWon == 0 && isgameRun != 0);
    char str[100];
    if (isgameWon == 1){
        strcpy(str, "YOU WON!");
        // Show Player Won?
    }else{
        strcpy(str,"YOU LOST!");
        // Show Player Lost
    }
    move(LINES/2,(COLS - strlen(str))/2);
    addstr(str);
    refresh();

    sleep(5);
    endwin();
    return 0;
}

void input(){
    int ch = getch();
    if (ch == 'w' || ch == KEY_UP){
        moveDirection.x = -1;
        moveDirection.y = 0;
    }else if(ch == 's' || ch == KEY_DOWN){
        moveDirection.x = 1;
        moveDirection.y = 0;
    }else if(ch == 'd' || ch == KEY_RIGHT){
        moveDirection.x = 0;
        moveDirection.y = 1;
    }else if(ch == 'a' || ch == KEY_LEFT){
        moveDirection.x = 0;
        moveDirection.y = -1;
    }
    
}
void collision(){
    move(snake[0].x + moveDirection.x,snake[0].y + moveDirection.y);
    chtype chT = winch(stdscr);
    char ch = chT & A_CHARTEXT; // get the character of where the head in going
    
    if (ch == head || ch == body || ch == '#'){
        isgameRun = 0;
    }else if(ch != ' ' ){
        // Assume the snake ran into a trophy
        //int trophyVal = ch - '0'; // convert char num to value by sub ascii value of 0
        //snakeLen += trophyVal;
        // generate new trophy on claim
        printf("%c\n",ch);
        // check winCondition on interaction with char
        if (snakeLen > winCondition){
            isgameWon = 1;
        }
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
        move(snake[i].x, snake[i].y);
        addch(bodypart);
        move(prev.x,prev.y);
        addch(' ');
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
    for (int x = 0; x<LINES; x++){ // No need to nested loop cause then you are looping over unecessary positions on the screen
        for (int y = 0; y<COLS; y++){
            if ((x == 0 || x == LINES-1) || (y == 0 || y == COLS -1)){ // Same comment as above but this is just columns
                move(x,y);
                addch('#');
            }
        }
    }
}

void createSnake(){ // Seth
    // Get Random Starting moveDirection
    int r = getRand(4, 1);
    switch (r) { // random direction
        case 1:
            moveDirection.x = -1;
            moveDirection.y = 0;
            break;
        case 2:
            moveDirection.x = 1;
            moveDirection.y = 0;
            break;
        case 3:
            moveDirection.x = 0;
            moveDirection.y = 1;
            break;
        case 4:
            moveDirection.x = 0;
            moveDirection.y = 1;
            break;
    }

    // Move to Center and Add Head
    int curX = LINES/2;
    int curY = COLS/2;
    move(curX,curY);
    addch(head);
    snake[0].x = curX;
    snake[0].y = curY; // Sets the Head to center

    for (int i = 1; i < snakeLen; i++){ // Build the rest of the body in the opposite of the move direction
        snake[i].x = snake[i-1].x - moveDirection.x;
        snake[i].y = snake[i-1].y - moveDirection.y;
        move(snake[i].x, snake[i].y);
        addch(body);
    }

    refresh();
}