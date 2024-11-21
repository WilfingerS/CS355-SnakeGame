#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
// to compile:
//      gcc snake.c -o snake -l curses
/*
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
    

    Project Details (from Stan's requirements)
    https://cs.ccsu.edu/~stan/classes/CS355/Project-FA24.html
    The snake pit:
        The snake pit is the area where the snake can move.
        The snake pit must utilize all available space of the current terminal window.
        There must be a visible border delineating the snake pit.
    The initial length of the snake is three characters.
        Initial direction of the snake's movement is chosen randomly.
        The user can press one of the four arrow keys to change the direction of the snake's movement.
        The snake's speed is proportional to its length.
    The trophies:
        Trophies are represented by a digit randomly chosen from 1 to 9.
        There's always exactly one trophy in the snake pit at any given moment.
        When the snake eats the trophy, its length is increased by the corresponding number of characters.
        A trophy expires after a random interval from 1 to 9 seconds.
        A new trophy is shown at a random location on the screen after the previous one has either expired or is eaten by the snake.
    The gameplay:
        The snake dies and the game ends if:
        It runs into the border; or
        It runs into itself; or
        The user attempts to reverse the snake's direction.
        The user wins the game if the snake's length grows to the length equal to half the perimeter of the border.

*/
// functions initialization
void createBorders();
void createSnake();
void input();
int getRand(int max,int min);

struct vector2{
    int x;
    int y;
};

// Snake info; (i wonder if we allow diagonal movement?)
int snakeLen = 5; // initial length
struct vector2 moveDirection; // this will be a vector 2;
struct vector2 snake[128]; // contains info on the actual snake

// Game info

int main(){
    int isgameWon = 0; // exit if game won
    int isgameRun = 1; // exit if game ended
    srand(time(NULL)); // seeds rand number generator with current time atm
    LINES = 20; // Define borders
    COLS = 40; // Define border

    // Start Creating Game
    initscr(); // Create Screen
    createBorders();
    createSnake();
    
    sleep(3);
    endwin();
    return 0;
}

int getRand(int max,int min){ // returns random number in between the max and min
    return (rand() % (max - min + 1)) + min;
}

void createBorders(){ // LINES COLS globals
    // x borders: x == 0 || x == LINES -1
    // y borders: y == 0 || y == COLS  -1
    for (int x = 0; x<LINES; x++){ // No need to nested loop cause then you are looping over unecessary positions on the screen
        for (int y = 0; y<COLS; y++){
            if ((x == 0 || x == LINES-1) || (y == 0 || y == COLS -1)){ // Same comment as above but this is just columns
                move(x,y);
                addch('#');
            }
        }
    }
}

void createSnake(){
    // Get Starting moveDirection
    int r = getRand(4, 1);
    printf("rand:%d\n",r);
    switch (r) { // random direction
        case 1:
            moveDirection.x = 1;
            moveDirection.y = 0;
            break;
        case 2:
            moveDirection.x = -1;
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
    printf("MoveDirection x:%d,y%d\n",moveDirection.x,moveDirection.y);
    // Move to Center and Add Head
    int curX = LINES/2;
    int curY = COLS/2;
    move(curX,curY);
    addch('0');
    snake[0].x = curX;
    snake[0].y = curY; // Sets the Head to center
    for (int i = 1; i < snakeLen; i++){ // Build the rest of the body in the opposite of the move direction
        snake[i].x = snake[i-1].x - moveDirection.x;
        snake[i].y = snake[i-1].y - moveDirection.y;
        move(snake[i].x, snake[i].y);
        addch('o');
    }

    refresh();
}