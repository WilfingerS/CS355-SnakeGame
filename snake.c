#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
// to compile: gcc snake.c -o snake -l curses

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

void createBorders();
void createSnake();
void input();

int main(){
    fprintf(stderr,"Creating the virtual screen\n");
    LINES = 20; // Define borders
    COLS = 40; // Define border
    initscr();
    if (stdscr == NULL){
        fprintf(stderr,"Error initializing screen");
        return 1;
    }
    // x borders: x == 0 || x == LINES -1
    // y borders: y == 0 || y == COLS  -1
    createBorders();
    refresh();
    createSnake();

    sleep(3);
    endwin();
    return 0;
}

void createBorders(){ // LINES COLS globals
    for (int x = 0; x<LINES; x++){
        for (int y = 0; y<COLS; y++){
            if (x == 0 || y == 0 || x == LINES-1 || y == COLS -1){ // Creates Borders when 
                move(x,y);
                addch('#');
            }
        }
    }
}

void createSnake(){
    // get random position
}