# Sudoku Consistency Check

This project implements a Sudoku consistency checker using a multi-process approach in C.

## Overview
Sudoku Consistency Check is a C program designed for validating the consistency of a Sudoku puzzle. The project utilizes a multi-process architecture, with a parent process and four child processes each assigned specific roles.process.

1. **Parent Process**: Reads the input file, decodes the ciphered alphabet characters, and communicates with other processes using name pipes.

2. **Decoder Process**: Decodes the ciphered Sudoku board received from the parent process and sends it back.

3. **Row Checker Process**: Checks for row duplications in the Sudoku board.

4. **Column Checker Process**: Checks for column duplications in the Sudoku board.

5. **Submatrix Checker Process**: Checks for submatrix duplications in the Sudoku board.


## Implementation

   - Parent process reads and decodes the Sudoku puzzle from the input file and send it to decoder process.
   - Decoder process decodes ciphered characters and returns the board.
   - parent process distributes the board to child processes for consistency checks.
   - Each child process checks its aspect of the Sudoku puzzle.
   - If constraints are satisfied, outputs: `Sudoku puzzle constraints satisfied`. Otherwise, if incorrect or inconsistent: `Sudoku puzzle is wrong`.



