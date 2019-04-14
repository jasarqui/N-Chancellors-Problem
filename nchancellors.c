#include <stdio.h>
#include <stdlib.h>
// define boolean
#define FALSE 0
#define TRUE 1
// define constants
#define START 0
#define Y 0
#define X 1
#define AVAILABLE 0
#define UNAVAILABLE 1

// define globals
// this is to minimize the parameters passed
int boardsize, num_candidates, move;
int* nopts;
int** options;

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
    // ----- replace from here for file reading -----
    // get the array size
    while (TRUE) {
        printf("Enter board size (must be >1): ");
        scanf("%d", &boardsize);
        if (boardsize > 1) break;
    }

    // generate arrays
    int i, j;
    nopts = (int *) malloc(sizeof(int) * (boardsize + 2));
    options = (int **) malloc(sizeof(int*) * (boardsize + 2));
    for (i = 0; i < boardsize + 2; i++) {
        options[i] = (int *) malloc(sizeof(int) * (boardsize + 2));
        for (j = 0; j < boardsize + 2; j++) {
            options[i][j] = 0;
        }
        nopts[i] = 0;
    }
    // ----- up to here ----- 

    // backtrack utilities
    int num_of_solutions = 0;
    move = 0;
    nopts[START] = 1;

    // backtracking
    while (nopts[START] > 0) {
        if (nopts[move] > 0) {
            move++;

            if (move == boardsize + 1) {
                printf("Solution Found: \n");
                for (i = 1; i <= boardsize; i++) {
                    printf("(%d, %d) ", options[i][nopts[i]] - 1, i - 1);
                }
                printf("\n");
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

    printf("\nNumber of Solutions Found: %d\n", num_of_solutions);
    // deallocate arrays
    for (i = 0; i < boardsize + 2; i++) {
        free(options[i]);
    }
    free(options);
    free(nopts);

    return 0;
}