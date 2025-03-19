# Sudoku Solver

A C++ implementation of a Sudoku puzzle solver with an interactive ncurses-based interface. This program can solve both 4x4 and 9x9 Sudoku puzzles using a backtracking algorithm.

## Features

- Supports both 4x4 and 9x9 Sudoku puzzles
- Load puzzles from text files
- Generate random puzzles with configurable difficulty
- Interactive ncurses-based user interface
- Color-coded display (if terminal supports colors)
- Validate solutions
- Solve puzzles using backtracking algorithm

## Requirements

- C++ compiler (g++ or clang++)
- ncurses library
- UNIX-like operating system (Linux, macOS)

## Installation

1. Clone this repository or download the source code
2. Compile the program using g++ or clang++:

```bash
g++ -o sudoku_solver sudoku_solver.cpp -lncurses
```

Or using clang++:

```bash
clang++ -o sudoku_solver sudoku_solver.cpp -lncurses
```

## Usage

### Loading a puzzle from a file

```bash
./sudoku_solver <puzzle_file> [grid_size]
```

Example:
```bash
./sudoku_solver puzzle_9x9.txt
```

Or for a 4x4 puzzle:
```bash
./sudoku_solver puzzle_4x4.txt 4
```

### Generating a random puzzle

```bash
./sudoku_solver random <percentage> [grid_size]
```

Where `percentage` is the percentage of cells to fill (between 10 and 50).

Example:
```bash
./sudoku_solver random 20 9
```

This will generate a 9x9 puzzle with approximately 20% of cells filled.

### Interactive Controls

Once the program is running, you can use the following keyboard controls:

- `S` - Solve the puzzle using the backtracking algorithm
- `V` - Validate the current solution
- `Q` - Quit the program

## Puzzle File Format

Puzzle files should be formatted as plain text with one line per row of the Sudoku grid. Empty cells are represented by dots (`.`) or zeros (`0`), and filled cells contain the respective numbers.

Example 4x4 puzzle (puzzle_4x4.txt):
```
..3.
4...
...2
.1..
```

Example 9x9 puzzle (puzzle_9x9.txt):
```
5.3.7....
6..195...
.98....6.
8...6...3
4..8.3..1
7...2...6
.6....28.
...419..5
....8..79
```

## How It Works

The solver uses a backtracking algorithm to find a solution to the Sudoku puzzle:

1. Find an empty cell in the grid
2. Try placing numbers 1-9 (or 1-4 for 4x4 puzzles) in the cell
3. Check if the placed number is valid according to Sudoku rules
4. If valid, recursively attempt to fill the rest of the grid
5. If the recursive call is successful, the puzzle is solved
6. If the recursive call fails, backtrack and try a different number
7. If all numbers have been tried and none work, return false to trigger backtracking

The program uses the ncurses library to create an interactive terminal interface with color-coding (if supported by the terminal).

## Project Structure

- `sudoku_solver.cpp` - Main source code file containing all classes and functions
- `puzzle_4x4.txt` - Example 4x4 puzzle file
- `puzzle_9x9.txt` - Example 9x9 puzzle file
- `README.md` - This documentation file

## License

This project is open source and available for personal and educational use.