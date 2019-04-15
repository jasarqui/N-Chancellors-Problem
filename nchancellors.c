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
int* nopts, *rows_with_chancellor;
int** options;
clock_t start, end;
double cpu_time_used;

// returns the list of candidates
void generate_candidates() {
    num_candidates = 0;

    int i, j;
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
        else if (options[move-1][nopts[move-1]] == i - 2 || options[move-1][nopts[move-1]] == i + 2) continue;
        // check 2 moves before
        else if (move > 2 && (options[move-2][nopts[move-2]] == i - 1 || options[move-2][nopts[move-2]] == i + 1)) continue;
        // add as candidate
        else {
            options[move][num_candidates+1] = i;
            num_candidates++;
        }
    }
}

// returns all possible start
void starting_candidates() {
    num_candidates = 0;

    int i;
    for (i = boardsize; i > 0; i--) {
        options[move][num_candidates+1] = i;
        num_candidates++;
    }
}

int main() {
    // read file
    FILE* fp;
    char buffer[255];

    fp = fopen("board.in", "r");
    // get the number of boards
    fgets(buffer, 255, fp);
    num_boards = atoi(buffer);

    int i, j, n;
    // get the board
    for (n = 0; n < num_boards; n++) {
        fgets(buffer, 255, fp);
        boardsize = atoi(buffer);

        // initialize arrays
        nopts = (int *) malloc(sizeof(int) * (boardsize + 2));
        rows_with_chancellor = (int *) malloc(sizeof(int) * (boardsize + 2));
        options = (int **) malloc(sizeof(int*) * (boardsize + 2));
        for (i = 0; i < boardsize + 2; i++) {
            options[i] = (int *) malloc(sizeof(int) * (boardsize + 2));
            for (j = 0; j < boardsize + 2; j++) {
                options[i][j] = 0;
            }
            nopts[i] = 0;
        }

        // get the board initialization
        for (i = 1; i <= boardsize; i++) {
            fgets(buffer, 255, fp);

            char *rest = NULL;
            char *token;

            int col = 1;
            // splits a row/line by spaces, then assigns them to the board
            // ref: https://stackoverflow.com/questions/15961253/c-correct-usage-of-strtok-r
            for (token = strtok_r(buffer, " ", &rest); token != NULL; token = strtok_r(NULL, " ", &rest)) {
                options[i][col] = atoi(token);

                // check for edited rows
                if (atoi(token) != 0) rows_with_chancellor[i] = TRUE;
                col++;
            }
        }

        // backtrack the current board
        printf("Boardsize: %d\n", boardsize);
        // for (i = 1; i <= boardsize; i++) {
        //     printf("%d ", rows_with_chancellor[i]);
        // }
        // printf("\n");

        // backtrack utilities
        int num_of_solutions = 0;
        move = 0;
        nopts[START] = 1;

        // backtracking
        start = clock();
        while (nopts[START] > 0) {
            if (nopts[move] > 0) {
                move++;

                if (move == boardsize + 1) {
                    // printf("Solution Found: \n");
                    // for (i = 1; i <= boardsize; i++) {
                    //     printf("(%d, %d) ", options[i][nopts[i]] - 1, i - 1);
                    // }
                    // printf("\n");
                    num_of_solutions++;                    
                } else if (move == 1) {
                    starting_candidates();
                    nopts[move] = num_candidates;
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

        printf(">> %d solutions found in %f seconds.\n", num_of_solutions, cpu_time_used);
        // deallocate arrays
        for (i = 0; i < boardsize + 2; i++) {
            free(options[i]);
        }
        free(options);
        free(nopts);
    }

    return 0;
}