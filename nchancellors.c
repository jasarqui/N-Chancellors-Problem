#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// define boolean
#define FALSE 0
#define TRUE 1
// define constants
#define START 0
#define AVAILABLE 0
#define UNAVAILABLE 1

// define globals
// this is to minimize the parameters passed
int boardsize, num_candidates, move, num_boards;
int* nopts, *rows_with_chancellor, *chancellor_position;
int** options;

// returns the list of candidates
void generate_candidates() {
    num_candidates = 0;
    int i, j;

    // check first if row already has existing move
    if (rows_with_chancellor[move] == 1) {
        // check if move is valid
        int flag = AVAILABLE;

        // check rook moves
        for (j = 1; j < move; j++) {
            // check top of stacks
            if (chancellor_position[move] == options[j][nopts[j]]) {
                flag = UNAVAILABLE;
                break;
            }
        }

        // check knight moves
        // check previous move
        if (move > 1 && (options[move-1][nopts[move-1]] == chancellor_position[move] - 2 || 
            options[move-1][nopts[move-1]] == chancellor_position[move] + 2)) flag = UNAVAILABLE;
        // check 2 moves before
        else if (move > 2 && (options[move-2][nopts[move-2]] == chancellor_position[move] - 1 || 
            options[move-2][nopts[move-2]] == chancellor_position[move] + 1)) flag = UNAVAILABLE;

        // move is valid
        if (flag == AVAILABLE) {
            options[move][num_candidates+1] = chancellor_position[move];
            num_candidates++;
        }
    // there are no moves in the row
    } else if (rows_with_chancellor[move] == 0) {
        for (i = boardsize; i > 0; i--) {
            // check rook moves
            int flag = AVAILABLE;
            for (j = 1; j < move; j++) {
                // check top of stacks
                if (i == options[j][nopts[j]]) {
                    flag = UNAVAILABLE;
                    break;
                }
            }

            if (flag == UNAVAILABLE) continue;
            // check knight moves
            // check previous move
            else if (move > 1 && (options[move-1][nopts[move-1]] == i - 2 || 
                options[move-1][nopts[move-1]] == i + 2)) continue;
            // check 2 moves before
            else if (move > 2 && (options[move-2][nopts[move-2]] == i - 1 || 
                options[move-2][nopts[move-2]] == i + 1)) continue;
            // add as candidate
            else {
                options[move][num_candidates+1] = i;
                num_candidates++;
            }
        }
    }
    // else, there are more than one
    // hence do nothing
}

int main() {
    // read file
    FILE* fp, *fout;
    char buffer[255];
    fp = fopen("board.in", "r");

    // get the number of boards
    fgets(buffer, 255, fp);
    num_boards = atoi(buffer);

    // declare utility variables
    int i, j, n, invalid_board;
    clock_t start, end;
    double cpu_time_used;

    // clear file contents first for reset
    fclose(fopen("solutions.out", "w"));

    // get the boards
    for (n = 0; n < num_boards; n++) {
        fgets(buffer, 255, fp);
        boardsize = atoi(buffer);
        invalid_board = 0;

        // initialize arrays
        nopts = (int *) malloc(sizeof(int) * (boardsize + 2));
        rows_with_chancellor = (int *) malloc(sizeof(int) * (boardsize + 2));
        chancellor_position = (int *) malloc(sizeof(int) * (boardsize + 2));
        options = (int **) malloc(sizeof(int*) * (boardsize + 2));
        for (i = 0; i < boardsize + 2; i++) {
            options[i] = (int *) malloc(sizeof(int) * (boardsize + 2));
            // initialize to 0
            for (j = 0; j < boardsize + 2; j++) {
                options[i][j] = 0;
            }
            nopts[i] = 0;
            rows_with_chancellor[i] = 0;
        }

        // get the board initialization
        for (i = 1; i <= boardsize; i++) {
            fgets(buffer, 255, fp);

            char *rest = NULL;
            char *token;

            int col = 1;
            // check for chancellors already present
            // ref: https://stackoverflow.com/questions/15961253/c-correct-usage-of-strtok-r
            for (token = strtok_r(buffer, " ", &rest); token != NULL; token = strtok_r(NULL, " ", &rest)) {
                // check for edited rows
                if (atoi(token) != 0) {
                    chancellor_position[i] = col;
                    rows_with_chancellor[i]++;
                    // check row validity
                    if (rows_with_chancellor[i] > 1) invalid_board = TRUE;
                }
                col++;
            }
        }

        // backtrack utilities
        int num_of_solutions = 0;
        move = 0;
        nopts[START] = 1;

        // open file for appending
        fout = fopen("solutions.out", "a");
        fprintf(fout, "Board %d with size %dx%d\n", n+1, boardsize, boardsize);
        if (invalid_board == TRUE) {
            // more than one chancellors in one row
            fprintf(fout, ">> No solution found.\n");
            continue;
        }

        // backtracking
        start = clock();
        while (nopts[START] > 0) {
            if (nopts[move] > 0) {
                move++;

                if (move == boardsize + 1) { // solution found
                    for (i = 1; i <= boardsize; i++) {
                        fprintf(fout, "(%d, %d) ", options[i][nopts[i]] - 1, i - 1);
                    }
                    fprintf(fout, "\n");
                    num_of_solutions++;         
                } else { // push
                    generate_candidates();
                    nopts[move] = num_candidates;    
                }

            } else { // pop
                move--;
                nopts[move]--;
            }
        }
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        // print formatting for number of solutions found
        if (num_of_solutions == 1) {
            fprintf(fout, ">> %d solution found in %f seconds.\n\n", num_of_solutions, cpu_time_used);
        } else if (num_of_solutions == 0) {
            fprintf(fout, ">> No solution found in %f seconds.\n\n", cpu_time_used);
        } else {
            fprintf(fout, ">> %d solutions found in %f seconds.\n\n", num_of_solutions, cpu_time_used);
        }

        // deallocate arrays
        for (i = 0; i < boardsize + 2; i++) {
            free(options[i]);
        }
        free(options);
        free(nopts);
        fclose(fout);
    }

    printf("File generated at 'solutions.out'.\n");
    return 0;
}