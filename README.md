# GeeseSpotter

This repository contains the implementation of a simplified version of Minesweeper, called GeeseSpotter. The game includes functionalities for creating and managing a game board, revealing and marking tiles, and computing neighboring geese counts. The code ensures memory management for dynamically allocated arrays and includes functions to check for game completion. Key features include:

 1.  create_board: Initializes a game board of specified dimensions.
 2.  clean_board: Safely deallocates memory used by the game board.
 3.  print_board: Prints the current state of the game board with proper formatting.
 4.  hide_board: Sets the hidden bit for all tiles on the board.
 5.  mark: Toggles the marked state of a specified tile.
 6.  compute_neighbours: Calculates the number of neighboring geese for each tile.
 7.  is_game_won: Checks if the game has been won by the player.
 8.  reveal: Reveals a tile and its neighbors if it is empty.
  
The implementation focuses on bitwise operations to manage tile states efficiently, ensuring a smooth and engaging gameplay experience.
