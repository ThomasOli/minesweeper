# Minesweeper

Minesweeper... yep, that old classic game you remember playing on good ol' Windows '95 back in the day (in fact it was implemented in Microsoft's software updates until Windows 8). Minesweeper has its origins in the earliest mainframe games of the 1960s and 1970s. The earliest ancestor of Minesweeper was Jerimac Ratliff's Cube. The basic gameplay style became a popular segment of the puzzle game genre during the 1980s.

Basic Gameplay:

Choose your desired difficulty level:
Easy: 9x9 grid with 10 mines.
Medium: 16x16 grid with 40 mines.
Hard: 30x30 grid with 160 mines.
Start the game by clicking anywhere on the game board. The timer will begin.
Each number on the board indicates the count of adjacent mines to that cell.
Use "Shift + Click" to place flags on cells you believe contain mines.
Winning/Losing:

If you uncover a mine, the game ends.
Win by revealing all cells without mines.

Technologies Used:

The game is built using the C++ programming language and the SFML (Simple and Fast Multimedia Library).
The graphical representation and user interaction are achieved through SFML's capabilities.

Game Flow:

Choose a difficulty level.
Generate the game board based on the chosen difficulty.
Reveal cells by clicking on them. Numbers indicate nearby mines.
Use "Shift + Click" to flag potential mine cells.
Hitting a mine ends the game.
Win by uncovering all non-mine cells.

Design Choices:

The game's visuals pay homage to the classic Minesweeper design.
SFML is used for creating the graphical interface and handling user input.
A blend of nostalgia and modern programming techniques enhances the experience.

Next Steps:

The Minesweeper project is a valuable learning opportunity in C++ and game development.
Future enhancements might include:
Enhancing the user interface with interactive elements.
Resolving aesthetic issues affecting cell dimensions.
Expanding the classic design with a clock and desktop-style icons.
Adding a feature for advanced players to input custom difficulty settings.
