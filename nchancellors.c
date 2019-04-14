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
#define START 0
#define NO_SOLUTION 0
#define SOLUTION_FOUND 1

// define globals
// this is to minimize the parameters passed
int boardsize, num_possible_moves, num_candidates, move, num_of_solutions;
int* unavailable_col;
int* unavailable_row;
int** board;
int*** options;
char* solution_string;
char* solutions;

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
    if (y - 2 >= 0 && x - 1 >= 0) knight_move(ADD, x, y, -1, -2);
    if (y - 2 >= 0 && x + 1 < boardsize) knight_move(ADD, x, y, 1, -2);
    if (y + 2 < boardsize && x - 1 >= 0) knight_move(ADD, x, y, -1, 2);
    if (y + 2 < boardsize && x + 1 < boardsize) knight_move(ADD, x, y, 1, 2);
    if (x - 2 >= 0 && y - 1 >= 0) knight_move(ADD, x, y, -2, -1);
    if (x - 2 >= 0 && y + 1 < boardsize) knight_move(ADD, x, y, -2, 1);
    if (x + 2 < boardsize && y - 1 >= 0) knight_move(ADD, x, y, 2, -1);
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
    if (y - 2 >= 0 && x - 1 >= 0) knight_move(REMOVE, x, y, -1, -2);
    if (y - 2 >= 0 && x + 1 < boardsize) knight_move(REMOVE, x, y, 1, -2);
    if (y + 2 < boardsize && x - 1 >= 0) knight_move(REMOVE, x, y, -1, 2);
    if (y + 2 < boardsize && x + 1 < boardsize) knight_move(REMOVE, x, y, 1, 2);
    if (x - 2 >= 0 && y - 1 >= 0) knight_move(REMOVE, x, y, -2, -1);
    if (x - 2 >= 0 && y + 1 < boardsize) knight_move(REMOVE, x, y, -2, 1);
    if (x + 2 < boardsize && y - 1 >= 0) knight_move(REMOVE, x, y, 2, -1);
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
void generate_candidates() {
    int i, j;
    num_candidates = 0;

    for (i = boardsize - 1; i >= 0; i--) {
        if (unavailable_row[i] == UNAVAILABLE) continue;
        for (j = boardsize - 1; j >= 0; j--) {
            if (unavailable_col[j] == UNAVAILABLE) continue;
            if (board[i][j] == AVAILABLE) {
                options[move][num_candidates+1][0] = i;
                options[move][num_candidates+1][1] = j;
                num_candidates++;
            }
        }
    }
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
    int nopts[boardsize + 2];
    int i, j;
    // board
    board = (int **) malloc(boardsize * sizeof(int*));
    for (i = 0; i < boardsize; i++) {
        board[i] = (int *) malloc(sizeof(int) * boardsize);
    }
    // candidate matrix
    options = (int ***) malloc((boardsize + 2) * sizeof(int**));
    for (i = 0; i < boardsize + 2; i++) {
        options[i] = (int **) malloc(sizeof(int*) * (num_possible_moves + 2));
        for (j = 0; j < num_possible_moves + 2; j++) {
            options[i][j] = (int *) malloc(sizeof(int) * 2);
        }
    }
    // utility arrays
    unavailable_col = (int *) malloc(sizeof(int) * boardsize);
    unavailable_row = (int *) malloc(sizeof(int) * boardsize);
    solutions = malloc (1);
    solutions[0] = '\0';

    // initialize arrays to 0
    for (i = 0; i < boardsize + 2; i++) {
        for (j = 0; j < num_possible_moves + 2; j++) {
            if (i < boardsize && j < boardsize) board[i][j] = 0;
            options[i][j][0] = 0;
            options[i][j][1] = 0;
        }

        if (i < boardsize) {
            unavailable_row[i] = 0;
            unavailable_col[i] = 0;
        }
        nopts[i] = 0;
    }

    // backtrack utilities
    move = 0;
    nopts[START] = 1;

    // backtracking
    while (nopts[START] > 0) {
        if (nopts[move] > 0) {
            move++;

            if (move == boardsize + 1 && num_candidates == 1) {
                // add the last move first
                add_chancellor(options[move-1][nopts[move-1]][0], options[move-1][nopts[move-1]][1]);

                // save board state in a string
                char* board_state;
                solution_string = malloc(num_possible_moves);
                solution_string[0] = '\0';
                for (i = 0; i < boardsize; i++) {
                    for (j = 0; j < boardsize; j++) {
                        int length = snprintf(NULL, 0, "%d", board[i][j]);
                        board_state = malloc(length + 1);
                        snprintf(board_state, length + 1, "%d", board[i][j]);
                        strcat(solution_string, board_state);
                    }
                }
                free(board_state);

                int solution_flag = SOLUTION_FOUND;
                if (solutions[0] != '\0') {
                    char *solution;
                    // copy first the solutions before tokenizing
                    char *solution_copy = malloc (1);
                    solution_copy[0] = '\0';
                    strcpy(solution_copy, solutions);

                    solution = strtok(solution_copy, ",");
                    while (solution != NULL) {
                        // if there is a similar solution
                        if (strcmp(solution, solution_string) == 0) {
                            solution_flag = NO_SOLUTION;
                            break;
                        }
                        // update
                        solution = strtok (NULL, ",");
                    }

                    if (solution_flag == SOLUTION_FOUND) {
                        // unique solution found
                        printf("Solution Found: \n");
                        for (i = 1; i <= boardsize; i++) {
                            printf("(%d, %d) ", options[i][nopts[i]][0], options[i][nopts[i]][1]);
                        }
                        printf("\n");
                        
                        strcat(solutions, ",");
                        strcat(solutions, solution_string);
                        num_of_solutions++;
                    }

                    free(solution_copy);
                } else {
                    strcat(solutions, solution_string);
                    // unique solution found
                    printf("Solution Found: \n");
                    for (i = 1; i <= boardsize; i++) {
                        printf("(%d, %d) ", options[i][nopts[i]][0], options[i][nopts[i]][1]);
                    }
                    printf("\n");
                    num_of_solutions++;
                }

            } else if (move == 1) {
                generate_candidates();
                nopts[move] = num_candidates;
            } else { // push
                // add the chancellor from previous move to the board
                add_chancellor(options[move-1][nopts[move-1]][0], options[move-1][nopts[move-1]][1]);
                generate_candidates();
                nopts[move] = num_candidates;    
            }

        } else { // pop
            move--;
            remove_chancellor(options[move][nopts[move]][0], options[move][nopts[move]][1]);
            nopts[move]--;
        }
    }

    printf("\nNumber of Solutions Found: %d\n", num_of_solutions);
    // deallocate arrays
    for (i = 0; i < boardsize; i++) {
        free(board[i]);
    }
    free(board);
    free(unavailable_col);
    free(unavailable_row);
    free(solutions);
    return 0;
}