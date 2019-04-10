#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// define boolean
#define FALSE 0
#define TRUE 1
// define constants
#define AVAILABLE 0
#define UNAVAILABLE 1
#define ADD 0
#define REMOVE 1

// define globals
// this is to minimize the parameters passed
int boardsize, num_possible_moves;
int* unavailable_col;
int* unavailable_row;
int** board;

// prints the board
void print_board() {
    int i, j;
    for (i = 0; i < boardsize; i++) {
        for (j = 0; j < boardsize; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

// generalizes a knight's move whether adding or removing a chancellor
void knight_move(int type, int x, int y, int delta_x, int delta_y) {
    if (type == ADD) {
        if (board[x+delta_x][y+delta_y] == 0) num_possible_moves--;
        board[x+delta_x][y+delta_y]++;
    } else if (type == REMOVE) {
        board[x+delta_x][y+delta_y]--;
        if (board[x+delta_x][y+delta_y] == 0) num_possible_moves++;
    }
}

// adds a chancellor at the board
// will add unavailable spots on the board
void add_chancellor(int x, int y) {
    unavailable_row[x] = UNAVAILABLE;
    unavailable_col[y] = UNAVAILABLE;

    int i;
    // block off rook moves
    for (i = 0; i < boardsize; i++) {
        if (board[x][i] == 0) num_possible_moves--;
        board[x][i]++;
        if (board[i][y] == 0) num_possible_moves--;
        board[i][y]++;
    }
    board[x][y]--; // remove extra

    // block off knight moves
    if (y - 2 > 0 && x - 1 > 0) knight_move(ADD, x, y, -1, -2);
    if (y - 2 > 0 && x + 1 < boardsize) knight_move(ADD, x, y, 1, -2);
    if (y + 2 < boardsize && x - 1 > 0) knight_move(ADD, x, y, -1, 2);
    if (y + 2 < boardsize && x + 1 < boardsize) knight_move(ADD, x, y, 1, 2);
    if (x - 2 > 0 && y - 1 > 0) knight_move(ADD, x, y, -2, -1);
    if (x - 2 > 0 && y + 1 < boardsize) knight_move(ADD, x, y, -2, 1);
    if (x + 2 < boardsize && y - 1 > 0) knight_move(ADD, x, y, 2, -1);
    if (x + 2 < boardsize && y + 1 < boardsize) knight_move(ADD, x, y, 2, 1);
}

// for backtracking
void remove_chancellor(int x, int y) {
    unavailable_row[x] = AVAILABLE;
    unavailable_col[y] = AVAILABLE;

    int i;
    // free off rook moves
    for (i = 0; i < boardsize; i++) {
        board[x][i]--;
        if (board[x][i] == 0) num_possible_moves++;
        board[i][y]--;
        if (board[i][y] == 0) num_possible_moves++;
    }
    board[x][y]++; // remove extra

    // free off knight moves
    if (y - 2 > 0 && x - 1 > 0) knight_move(REMOVE, x, y, -1, -2);
    if (y - 2 > 0 && x + 1 < boardsize) knight_move(REMOVE, x, y, 1, -2);
    if (y + 2 < boardsize && x - 1 > 0) knight_move(REMOVE, x, y, -1, 2);
    if (y + 2 < boardsize && x + 1 < boardsize) knight_move(REMOVE, x, y, 1, 2);
    if (x - 2 > 0 && y - 1 > 0) knight_move(REMOVE, x, y, -2, -1);
    if (x - 2 > 0 && y + 1 < boardsize) knight_move(REMOVE, x, y, -2, 1);
    if (x + 2 < boardsize && y - 1 > 0) knight_move(REMOVE, x, y, 2, -1);
    if (x + 2 < boardsize && y + 1 < boardsize) knight_move(REMOVE, x, y, 2, 1);
}

// this will return x, y pairs as string
// reference: https://stackoverflow.com/questions/5242524/converting-int-to-string-in-c
char* concatenate(int x, int y) {
    // get the lengths of the integer
    int length_x = snprintf(NULL, 0, "%d", x);
    int length_y = snprintf(NULL, 0, "%d", y);
    // allocate strings
    char* string = malloc(length_x + 1);
    char* string_y = malloc(length_y + 1);
    // typecast to string
    snprintf(string, length_x + 1, "%d", x);
    snprintf(string_y, length_y + 1, "%d", y);
    // string concatenate
    strcat(string, ",");
    strcat(string, string_y);
    free(string_y); // free extra array
    return string;
}

// returns a the list of candidates
char* generate_candidates() {
    
}

int main() {
    // get the array size
    while (TRUE) {
        printf("Enter board size (must be >1): ");
        scanf("%d", &boardsize);
        if (boardsize > 1) break;
    }
    // initialize number of moves
    num_possible_moves = (int) pow((double) boardsize, (double) 2);

    // generate arrays
    char option[boardsize + 2][boardsize + 2];
    int nopts[boardsize + 2];

    int i, j;
    board = (int **) malloc(boardsize * sizeof(int*));
    for (i = 0; i < boardsize; i++) {
        board[i] = (int *) malloc(sizeof(int) * boardsize);
    }
    unavailable_col = (int *) malloc(sizeof(int) * boardsize);
    unavailable_row = (int *) malloc(sizeof(int) * boardsize);

    // initialize arrays to 0
    for (i = 0; i < boardsize + 2; i++) {
        for (j = 0; j < boardsize + 2; j++) {
            if (i < boardsize && j < boardsize) board[i][j] = 0;
            option[i][j] = 0;
        }
        if (i < boardsize) {
            unavailable_row[i] = 0;
            unavailable_col[i] = 0;
        }
        nopts[i] = 0;
    }

    add_chancellor(4, 5);
    add_chancellor(5, 6);
    add_chancellor(6, 7);
    remove_chancellor(5, 6);
    remove_chancellor(6, 7);
    print_board();
    printf("%d\n", num_possible_moves);

    // deallocate arrays
    for (i = 0; i < boardsize; i++) {
        free(board[i]);
    }
    free(board);
    free(unavailable_col);
    free(unavailable_row);
    return 0;
}