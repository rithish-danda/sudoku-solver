#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ncurses.h>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

// Class to represent a Sudoku grid
class SudokuGrid {
private:
    std::vector<std::vector<int> > grid;
    int size;       // Grid size (9 for 9x9, 4 for 4x4)
    int boxSize;    // Box size (3 for 9x9, 2 for 4x4)

public:
    // Constructor
    SudokuGrid(int gridSize = 9) {
        size = gridSize;
        boxSize = (gridSize == 9) ? 3 : 2;
        grid.resize(size, std::vector<int>(size, 0));
    }

    // Load puzzle from file
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            return false;
        }

        std::string line;
        int row = 0;

        // Read first line to determine grid size
        if (std::getline(file, line)) {
            if (line.length() >= 4 && line.length() <= 9) {
                size = line.length();
                boxSize = (size == 9) ? 3 : 2;
                grid.resize(size, std::vector<int>(size, 0));
            } else {
                return false; // Invalid grid size
            }

            // Process first line
            for (int col = 0; col < size; col++) {
                if (line[col] == '.' || line[col] == '0') {
                    grid[row][col] = 0;
                } else if (line[col] >= '1' && line[col] <= '9') {
                    grid[row][col] = line[col] - '0';
                } else {
                    return false; // Invalid character
                }
            }
            row++;
        }

        // Process remaining lines
        while (std::getline(file, line) && row < size) {
            if (line.length() != size) {
                return false; // Inconsistent line length
            }

            for (int col = 0; col < size; col++) {
                if (line[col] == '.' || line[col] == '0') {
                    grid[row][col] = 0;
                } else if (line[col] >= '1' && line[col] <= '9') {
                    if (size == 4 && line[col] > '4') {
                        return false; // Invalid number for 4x4 grid
                    }
                    grid[row][col] = line[col] - '0';
                } else {
                    return false; // Invalid character
                }
            }
            row++;
        }

        file.close();
        return (row == size); // Check if we read the correct number of rows
    }

    // Generate a random puzzle with given percentage of filled cells
    void generateRandom(int filledPercentage) {
        // Clear the grid
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                grid[i][j] = 0;
            }
        }

        // Calculate number of cells to fill
        int totalCells = size * size;
        int cellsToFill = (totalCells * filledPercentage) / 100;

        // Seed random number generator
        std::srand(std::time(nullptr));

        // Fill random cells
        int filledCells = 0;
        while (filledCells < cellsToFill) {
            int row = std::rand() % size;
            int col = std::rand() % size;
            int num = (std::rand() % size) + 1;

            // If cell is empty and number is valid
            if (grid[row][col] == 0 && isValid(row, col, num)) {
                grid[row][col] = num;
                filledCells++;
            }
        }
    }

    // Check if placing 'num' at position (row, col) is valid
    bool isValid(int row, int col, int num) const {
        // Check row
        for (int c = 0; c < size; c++) {
            if (grid[row][c] == num) {
                return false;
            }
        }

        // Check column
        for (int r = 0; r < size; r++) {
            if (grid[r][col] == num) {
                return false;
            }
        }

        // Check box
        int boxRow = row - (row % boxSize);
        int boxCol = col - (col % boxSize);
        for (int r = 0; r < boxSize; r++) {
            for (int c = 0; c < boxSize; c++) {
                if (grid[boxRow + r][boxCol + c] == num) {
                    return false;
                }
            }
        }

        return true;
    }

    // Find an empty cell
    bool findEmptyCell(int& row, int& col) const {
        for (row = 0; row < size; row++) {
            for (col = 0; col < size; col++) {
                if (grid[row][col] == 0) {
                    return true;
                }
            }
        }
        return false;
    }

    // Solve the puzzle using backtracking
    bool solve() {
        int row, col;

        // If no empty cell is found, puzzle is solved
        if (!findEmptyCell(row, col)) {
            return true;
        }

        // Try placing numbers 1 to size
        for (int num = 1; num <= size; num++) {
            if (isValid(row, col, num)) {
                // Place the number
                grid[row][col] = num;

                // Recursively try to solve the rest of the puzzle
                if (solve()) {
                    return true;
                }

                // If placing num doesn't lead to a solution, backtrack
                grid[row][col] = 0;
            }
        }

        // Trigger backtracking
        return false;
    }

    // Validate if the current grid is a valid Sudoku solution
    bool validateSolution() const {
        // Check if grid is completely filled
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if (grid[row][col] == 0) {
                    return false; // Grid is not completely filled
                }
            }
        }

        // Check rows
        for (int row = 0; row < size; row++) {
            std::vector<bool> seen(size + 1, false);
            for (int col = 0; col < size; col++) {
                int num = grid[row][col];
                if (seen[num]) {
                    return false; // Duplicate in row
                }
                seen[num] = true;
            }
        }

        // Check columns
        for (int col = 0; col < size; col++) {
            std::vector<bool> seen(size + 1, false);
            for (int row = 0; row < size; row++) {
                int num = grid[row][col];
                if (seen[num]) {
                    return false; // Duplicate in column
                }
                seen[num] = true;
            }
        }

        // Check boxes
        for (int boxRow = 0; boxRow < size; boxRow += boxSize) {
            for (int boxCol = 0; boxCol < size; boxCol += boxSize) {
                std::vector<bool> seen(size + 1, false);
                for (int r = 0; r < boxSize; r++) {
                    for (int c = 0; c < boxSize; c++) {
                        int num = grid[boxRow + r][boxCol + c];
                        if (seen[num]) {
                            return false; // Duplicate in box
                        }
                        seen[num] = true;
                    }
                }
            }
        }

        return true; // Solution is valid
    }

    // Get value at position (row, col)
    int getValue(int row, int col) const {
        if (row >= 0 && row < size && col >= 0 && col < size) {
            return grid[row][col];
        }
        return 0;
    }

    // Set value at position (row, col)
    void setValue(int row, int col, int value) {
        if (row >= 0 && row < size && col >= 0 && col < size) {
            grid[row][col] = value;
        }
    }

    // Get grid size
    int getSize() const {
        return size;
    }

    // Get box size
    int getBoxSize() const {
        return boxSize;
    }
};

// Class to handle the display and user interface
class SudokuDisplay {
private:
    SudokuGrid& grid;
    int startY, startX;
    bool colorEnabled;
    std::vector<std::vector<bool> > originalCells; // Track original cells from puzzle

public:
    SudokuDisplay(SudokuGrid& g) : grid(g) {
        startY = 2;
        startX = 4;
        colorEnabled = false;
    }

    // Initialize ncurses
    void initDisplay() {
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(0); // Hide cursor

        // Initialize colors if terminal supports them
        if (has_colors()) {
            start_color();
            init_pair(1, COLOR_WHITE, COLOR_BLACK);  // Normal cells
            init_pair(2, COLOR_CYAN, COLOR_BLACK);   // Fixed cells
            init_pair(3, COLOR_GREEN, COLOR_BLACK);  // Highlighted cells
            init_pair(4, COLOR_RED, COLOR_BLACK);    // Error cells
            colorEnabled = true;
        }

        clear();
        refresh();
    }

    // Display the Sudoku grid
    void displayGrid() {
        clear();

        int size = grid.getSize();
        int boxSize = grid.getBoxSize();

        // Display title
        mvprintw(0, startX, "SUDOKU SOLVER");
        mvprintw(1, startX, "%dx%d Grid", size, size);

        // Display horizontal lines
        for (int i = 0; i <= size; i++) {
            for (int j = 0; j <= size * 2; j++) {
                if (i % boxSize == 0) {
                    mvaddch(startY + i, startX + j, '-');
                } else if (j % 2 == 0) {
                    mvaddch(startY + i, startX + j, '-');
                }
            }
        }

        // Display vertical lines and numbers
        for (int i = 0; i <= size; i++) {
            for (int j = 0; j <= size; j++) {
                if (j % boxSize == 0) {
                    mvaddch(startY + i, startX + j * 2, '|');
                } else {
                    mvaddch(startY + i, startX + j * 2, '|');
                }

                // Display numbers
                if (i < size && j < size) {
                    int value = grid.getValue(i, j);
                    if (value != 0) {
                        if (colorEnabled) {
                            attron(COLOR_PAIR(2));
                        }
                        mvprintw(startY + i, startX + j * 2 + 1, "%d", value);
                        if (colorEnabled) {
                            attroff(COLOR_PAIR(2));
                        }
                    } else {
                        mvprintw(startY + i, startX + j * 2 + 1, " ");
                    }
                }
            }
        }

        // Display instructions
        int instructY = startY + size + 2;
        mvprintw(instructY, startX, "Press 'S' to solve the puzzle");
        mvprintw(instructY + 1, startX, "Press 'V' to validate the solution");
        mvprintw(instructY + 2, startX, "Press 'Q' to quit");

        refresh();
    }

    // Display solving animation
    void displaySolving() {
        int size = grid.getSize();
        mvprintw(startY + size + 5, startX, "Solving... Please wait");
        refresh();
    }

    // Display solution result
    void displaySolutionResult(bool solved) {
        int size = grid.getSize();
        if (solved) {
            if (colorEnabled) {
                attron(COLOR_PAIR(3));
            }
            mvprintw(startY + size + 5, startX, "Puzzle solved successfully!      ");
            if (colorEnabled) {
                attroff(COLOR_PAIR(3));
            }
        } else {
            if (colorEnabled) {
                attron(COLOR_PAIR(4));
            }
            mvprintw(startY + size + 5, startX, "No solution exists for this puzzle!");
            if (colorEnabled) {
                attroff(COLOR_PAIR(4));
            }
        }
        refresh();
    }

    // Display validation result
    void displayValidationResult(bool valid) {
        int size = grid.getSize();
        if (valid) {
            if (colorEnabled) {
                attron(COLOR_PAIR(3));
            }
            mvprintw(startY + size + 5, startX, "Solution is valid!              ");
            if (colorEnabled) {
                attroff(COLOR_PAIR(3));
            }
        } else {
            if (colorEnabled) {
                attron(COLOR_PAIR(4));
            }
            mvprintw(startY + size + 5, startX, "Solution is invalid!            ");
            if (colorEnabled) {
                attroff(COLOR_PAIR(4));
            }
        }
        refresh();
    }

    // Clean up ncurses
    void cleanUp() {
        endwin();
    }
};

// Main function
int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <puzzle_file> [grid_size]" << std::endl;
        std::cout << "  or   " << argv[0] << " random <percentage> [grid_size]" << std::endl;
        return 1;
    }

    // Default grid size is 9x9
    int gridSize = 9;
    if (argc >= 3 && std::string(argv[1]) != "random") {
        gridSize = std::stoi(argv[2]);
        if (gridSize != 4 && gridSize != 9) {
            std::cout << "Grid size must be 4 or 9" << std::endl;
            return 1;
        }
    } else if (argc >= 4 && std::string(argv[1]) == "random") {
        gridSize = std::stoi(argv[3]);
        if (gridSize != 4 && gridSize != 9) {
            std::cout << "Grid size must be 4 or 9" << std::endl;
            return 1;
        }
    }

    // Create Sudoku grid
    SudokuGrid grid(gridSize);

    // Load puzzle or generate random
    bool loaded = false;
    if (std::string(argv[1]) == "random") {
        if (argc < 3) {
            std::cout << "Missing percentage for random generation" << std::endl;
            return 1;
        }
        int percentage = std::stoi(argv[2]);
        if (percentage < 10 || percentage > 50) {
            std::cout << "Percentage must be between 10 and 50" << std::endl;
            return 1;
        }
        grid.generateRandom(percentage);
        loaded = true;
    } else {
        loaded = grid.loadFromFile(argv[1]);
    }

    if (!loaded) {
        std::cout << "Failed to load puzzle" << std::endl;
        return 1;
    }

    // Initialize display
    SudokuDisplay display(grid);
    display.initDisplay();

    // Main loop
    bool quit = false;
    while (!quit) {
        display.displayGrid();

        int ch = getch();
        if (ch == 's' || ch == 'S') {
            display.displaySolving();
            bool solved = grid.solve();
            display.displayGrid();
            display.displaySolutionResult(solved);
        } else if (ch == 'v' || ch == 'V') {
            bool valid = grid.validateSolution();
            display.displayValidationResult(valid);
        } else if (ch == 'q' || ch == 'Q') {
            quit = true;
        }
    }

    // Clean up
    display.cleanUp();
    return 0;
}