#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For usleep()

#define WIDTH 80   // Width of the grid
#define HEIGHT 40  // Height of the grid

// Function to initialize Gosper's Glider Gun pattern at any position
void placeGosperGliderGun(int *grid, int startX, int startY) {
    if (startX + 36 >= WIDTH || startY + 9 >= HEIGHT) return;  // Ensure pattern fits in the grid

    grid[startX + 1 + WIDTH * (startY + 5)] = 1;
    grid[startX + 2 + WIDTH * (startY + 5)] = 1;
    grid[startX + 1 + WIDTH * (startY + 6)] = 1;
    grid[startX + 2 + WIDTH * (startY + 6)] = 1;

    grid[startX + 13 + WIDTH * (startY + 3)] = 1;
    grid[startX + 14 + WIDTH * (startY + 3)] = 1;
    grid[startX + 12 + WIDTH * (startY + 4)] = 1;
    grid[startX + 16 + WIDTH * (startY + 4)] = 1;
    grid[startX + 11 + WIDTH * (startY + 5)] = 1;
    grid[startX + 17 + WIDTH * (startY + 5)] = 1;
    grid[startX + 11 + WIDTH * (startY + 6)] = 1;
    grid[startX + 15 + WIDTH * (startY + 6)] = 1;
    grid[startX + 17 + WIDTH * (startY + 6)] = 1;
    grid[startX + 18 + WIDTH * (startY + 6)] = 1;
    grid[startX + 11 + WIDTH * (startY + 7)] = 1;
    grid[startX + 17 + WIDTH * (startY + 7)] = 1;
    grid[startX + 12 + WIDTH * (startY + 8)] = 1;
    grid[startX + 16 + WIDTH * (startY + 8)] = 1;
    grid[startX + 13 + WIDTH * (startY + 9)] = 1;
    grid[startX + 14 + WIDTH * (startY + 9)] = 1;
}

// Function to initialize multiple Gosper Glider Guns
void initializeMultipleGosperGliderGuns(int *grid) {
    int xSpacing = 40, ySpacing = 20;
    for (int y = 0; y < HEIGHT; y += ySpacing) {
        for (int x = 0; x < WIDTH; x += xSpacing) {
            placeGosperGliderGun(grid, x, y);
        }
    }
}

// Display the grid with yellow live cells
void printGrid(int *grid) {
    printf("\033[H");  // Move cursor to top-left

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[y * WIDTH + x]) {
                printf("\033[1;43m  \033[0m");  // Yellow background for live cells
            } else {
                printf("\033[1;40m  \033[0m");  // Black background for dead cells
            }
        }
        printf("\n");
    }
    fflush(stdout);
}

// Apply the Game of Life rules
void updateGrid(int *grid, int *newGrid) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int aliveNeighbors = 0;

            // Check all 8 neighbors
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx == 0 && dy == 0) continue;

                    int nx = (x + dx + WIDTH) % WIDTH;
                    int ny = (y + dy + HEIGHT) % HEIGHT;

                    aliveNeighbors += grid[ny * WIDTH + nx];
                }
            }

            int currentState = grid[y * WIDTH + x];

            // Apply Conway's Game of Life rules
            if (currentState == 1 && (aliveNeighbors < 2 || aliveNeighbors > 3)) {
                newGrid[y * WIDTH + x] = 0;  // Cell dies
            } else if (currentState == 0 && aliveNeighbors == 3) {
                newGrid[y * WIDTH + x] = 1;  // Cell becomes alive
            } else {
                newGrid[y * WIDTH + x] = currentState;  // Cell stays the same
            }
        }
    }
}

// Main function
int main() {
    // Allocate memory for the grids
    int *grid = (int *)calloc(WIDTH * HEIGHT, sizeof(int));
    int *newGrid = (int *)calloc(WIDTH * HEIGHT, sizeof(int));

    if (grid == NULL || newGrid == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 1;
    }

    // Initialize the grid with multiple Gosper Glider Guns
    initializeMultipleGosperGliderGuns(grid);

    // Hide cursor
    printf("\033[?25l");

    // Run the simulation
    for (int t = 0; t < 100; t++) {
        printGrid(grid);
        updateGrid(grid, newGrid);

        // Swap grids
        int *temp = grid;
        grid = newGrid;
        newGrid = temp;

        usleep(200000);  // Delay (200ms)
    }

    // Show cursor again
    printf("\033[?25h");

    // Free allocated memory
    free(grid);
    free(newGrid);

    return 0;
}
