# CS355-SnakeGame
 Project Details (from Stan's requirements) <br>
    https://cs.ccsu.edu/~stan/classes/CS355/Project-FA24.html<br><br>
    <b>The snake pit:</b><br>
        The snake pit is the area where the snake can move.<br>
        The snake pit must utilize all available space of the current terminal window.<br>
        There must be a visible border delineating the snake pit.<br>
        The initial length of the snake is three characters.<br>
        Initial direction of the snake's movement is chosen randomly.<br>
        The user can press one of the four arrow keys to change the direction of the snake's movement.<br>
        The snake's speed is proportional to its length.<br><br>
    <b>The trophies:</b><br>
        Trophies are represented by a digit randomly chosen from 1 to 9.<br>
        There's always exactly one trophy in the snake pit at any given moment.<br>
        When the snake eats the trophy, its length is increased by the corresponding number of characters.<br>
        A trophy expires after a random interval from 1 to 9 seconds.<br>
        A new trophy is shown at a random location on the screen after the previous one has either expired or is eaten by the snake.<br><br>
    <b>The gameplay:</b><br>
        The snake dies and the game ends if:<br>
        It runs into the border; or<br>
        It runs into itself; or<br>
        The user attempts to reverse the snake's direction.<br>
        The user wins the game if the snake's length grows to the length equal to half the perimeter of the border.<br>
