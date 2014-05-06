SUDOKU SOLVER V1.0
==================

1. Info
Author: TNA
Description: solve a sudoku problem input from keyboard or file

2. Usage
For 32-bit Linux:
	sudoku_solver32 [input_file]
For 64-bit Linux:
	sudoku_solver64 [input_file]

3. Input file format
Input file contains the sudoku matrix. Every square of the matrix
is represented by its number. if a square is to be solved, it will
be input with 0. Each number on a row will be separated by a space
and each row will be on a separate line.
Example:
0 3 0 0 1 0 9 0 0
2 9 0 0 0 0 4 0 1
0 1 0 0 0 0 0 0 0
0 0 0 0 3 4 0 0 0
0 2 0 0 0 8 0 4 0
7 4 0 0 0 0 1 0 3
0 5 2 7 0 0 0 9 0
0 8 1 3 0 0 2 7 0
3 0 0 0 2 0 0 0 0
