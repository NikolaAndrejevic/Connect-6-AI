/* 
 * This program runs a connect 6 game between the computer and a human player
 * where the size of the board and blocked spots are chosen, and then the
 * players alternate turns until one player wins or all the spots are filled.
 * This program was used in the competition which was set up so that two of
 * these programs are run simultaneously and each computer acts as the code's
 * "human" player.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/resource.h>




//This function will print the current board on the screen

void printBoard(int n, char board[21][21]) {
    int x, y;
    for (x = 0; x < n; x++) {
        for (y = 0; y < n; y++)
            printf("%c", board [x][y]);
        printf("\n");

    }
}
//This function creates a board of all blanks of given size

void createBoard(int n, char board[21][21]) {
    int x, y;
    for (x = 0; x < n; x++)
        for (y = 0; y < n; y++)
            board [x][y] = 'U';
    printBoard(n, board);
}

//Asks user for a Move and plays the turn 

void humanTurn(int n, char board [21][21], bool colour) {
    bool done = false;
    int x, y;
    while (!done) {
        if (colour) {
            printf("Enter White Move (ROW COL): ");
            scanf("%d %d", &x, &y);
            if (board[x][y] == 'U') {
                board[x][y] = 'W';
                printBoard(n, board);
                done = true;
            } else if (x == -1 && y == -1)
                done = true;
            else if (x <= -1 || y <= -1 || x >= n || y >= n)
                printf("Out of range row or column\n");
            else
                printf("That square is already occupied or blocked\n");

        } else if (!colour) {
            printf("Enter Black Move (ROW COL): ");
            scanf("%d %d", &x, &y);
            if (board[x][y] == 'U') {
                board[x][y] = 'B';
                printBoard(n, board);
                done = true;
            } else if (x <= -1 || y <= -1 || x >= n || y >= n)
                printf("Out of range row or column\n");
            else
                printf("That square is already occupied or blocked\n");
        }
    }
}



//This function will find the longest chain of the same piece at any point

int findLongest(char board[21][21], int x, int y, bool Player) {
    char check;
    if (Player)
        check = 'W';
    else if (!Player)
        check = 'B';

    int i, j, longest = 0;


    for (i = 0; board[x][y + i + 1] == check; i++);
    for (j = 0; board[x][y - j - 1] == check && (y - j - 1 >= 0); j++);
    if (j + i + 1 > longest)
        longest = j + i + 1;
    for (i = 0; board[x + i + 1][y] == check; i++);
    for (j = 0; board[x - j - 1][y] == check && (x - j - 1 >= 0); j++);
    if (j + i + 1 > longest)
        longest = j + i + 1;
    for (i = 0; board[x + i + 1][y + i + 1] == check; i++);
    for (j = 0; board[x - j - 1][y - j - 1] == check && (x - j - 1 >= 0) && (y - j - 1 >= 0); j++);
    if (j + i + 1 > longest)
        longest = j + i + 1;
    for (i = 0; board[x + i + 1][y - i - 1] == check && (y - i - 1 >= 0); i++);
    for (j = 0; board[x - j - 1][y + j + 1] == check && (x - j - 1 >= 0); j++);
    if (j + i + 1 > longest)
        longest = j + i + 1;


    return longest;
}

//Resets a copy of the board used in functions below to the actual value of 
//the board.
void resetBoardCopy(int n, char boardCopy[21][21], char board[21][21]) {
    int x, y;
    for (x = 0; x < n; x++)
        for (y = 0; y < n; y++)
            boardCopy[x][y] = board[x][y];
}
/*Checks to see if the board has any spot where the computer can play and win 
 the game. Immediately plays at this spot if it exists.*/
bool connect6(int n, char board[21][21], bool humanPlayer, char compPlayerC) {
    int x, y;
    for (x = 0; x < n; x++)
        for (y = 0; y < n; y++) {
            if (board[x][y] == 'U' && findLongest(board, x, y, !humanPlayer) >= 6) {
                board[x][y] = compPlayerC;
                printf("Computer moves %c at %d %d\n", compPlayerC, x, y);
                printBoard(n, board);
                return true;
            }
        }
    return false;
}

/*Checks to see if the board has any spot that if played on can cause a segment
 of 5 in  a row which creates 2 opportunities for 6 in a row. Plays on this
 spot if it exists because the game can be won on the next move.*/
bool connect5(int n, char board[21][21], bool humanPlayer, char humanPlayerC, char compPlayerC) {
    int x1, y1, x2, y2, score, largestScore = 0, moveX, moveY;
    for (x1 = 0; x1 < n; x1++)
        for (y1 = 0; y1 < n; y1++) {
            if (board[x1][y1] == 'U' && findLongest(board, x1, y1, !humanPlayer) == 5) {
                char boardCopy[21][21];
                resetBoardCopy(n, boardCopy, board);
                boardCopy[x1][y1] = compPlayerC;
                score = 0;
                for (x2 = 0; x2 < n; x2++)
                    for (y2 = 0; y2 < n; y2++) {
                        if (board[x2][y2] == 'U' && findLongest(boardCopy, x2, y2, !humanPlayer) >= 6)
                            score++;
                    }
                if (score > largestScore) {
                    largestScore = score;
                    moveX = x1;
                    moveY = y1;
                }
            }
        }
    if (largestScore >= 2) {
        board[moveX][moveY] = compPlayerC;
        printf("Computer moves %c at %d %d\n", compPlayerC, moveX, moveY);
        printBoard(n, board);
        return true;
    }
    return false;
}

/*Checks to see if the board has any spot where the opponent can play and win 
 the game. Immediately blocks this spot if it exists.*/
bool block6(int n, char board[21][21], bool humanPlayer, char compPlayerC) {
    int x, y;
    for (x = 0; x < n; x++)
        for (y = 0; y < n; y++) {
            if (board[x][y] == 'U' && findLongest(board, x, y, humanPlayer) >= 6) {
                board[x][y] = compPlayerC;
                printf("Computer moves %c at %d %d\n", compPlayerC, x, y);
                printBoard(n, board);
                return true;
            }
        }
    return false;
}

/*Checks to see if the board has any spot that if played on can cause a segment
 of 5 in  a row which creates 2 opportunities for 6 in a row. Blocks this spot
 if it exists.*/
int block5(int n, char board[21][21], bool humanPlayer, char humanPlayerC, char compPlayerC) {
    int x1, y1, x2, y2, score, largestScore = 0, moveX, moveY;
    for (x1 = 0; x1 < n; x1++)
        for (y1 = 0; y1 < n; y1++) {
            if (board[x1][y1] == 'U' && findLongest(board, x1, y1, humanPlayer) == 5) {
                char boardCopy[21][21];
                resetBoardCopy(n, boardCopy, board);
                boardCopy[x1][y1] = humanPlayerC;
                score = 0;
                for (x2 = 0; x2 < n; x2++)
                    for (y2 = 0; y2 < n; y2++) {
                        if (board[x2][y2] == 'U' && findLongest(boardCopy, x2, y2, humanPlayer) >= 6)
                            score++;
                    }
                if (score > largestScore) {
                    largestScore = score;
                    moveX = x1;
                    moveY = y1;
                }
            }
        }
    if (largestScore >= 2) {
        board[moveX][moveY] = compPlayerC;
        printf("Computer moves %c at %d %d\n", compPlayerC, moveX, moveY);
        printBoard(n, board);
        return true;
    }
    return false;
}

/*Uses an algorithm to check if the particular spot is a danger spot for either
 player. A danger has the opportunity to provide a sure win and should be
 blocked/exploited depending on which player has it.*/
ifBlockException(int n, char board[21][21], bool humanPlayer, char humanPlayerC, char compPlayerC, int x, int y) {
    int j, i, score = 0, finalScore = 0;
    bool done = false;

    board[x][y] = humanPlayerC;
    for (i = -3; i <= 2 && !done; i++)
        if ((x + i >= 0) && (x + i < n)) {
            if (board[x + i][y] == humanPlayerC)
                score++;
            if (board[x + i][y] == 'R' && board[x + i][y] == compPlayerC)
                break;
            if (score >= 4 && (board[x + 1][y] == humanPlayerC || board[x - 1][y] == humanPlayerC)) {
                finalScore++;
                done = true;
            }
        }
    score = 0;
    for (i = -2; i <= 3 && !done; i++)
        if ((x + i >= 0) && (x + i < n)) {
            if (board[x + i][y] == humanPlayerC)
                score++;
            if (board[x + i][y] == 'R' && board[x + i][y] == compPlayerC)
                break;
            if (score >= 4 && (board[x + 1][y] == humanPlayerC || board[x - 1][y] == humanPlayerC)) {
                finalScore++;
                done = true;
            }
        }
    score = 0;
    done = false;
    for (i = -3; i <= 2 && !done; i++)
        if ((y + i >= 0) && (y + i < n)) {
            if (board[x][y + i] == humanPlayerC)
                score++;
            if (board[x][y + i] == 'R' && board[x][y + i] == compPlayerC)
                break;
            if (score >= 4 && (board[x][y + 1] == humanPlayerC || board[x][y - 1] == humanPlayerC)) {
                finalScore++;
                done = true;
            }
        }
    score = 0;
    for (i = -2; i <= 3 && !done; i++)
        if ((y + i >= 0) && (y + i < n)) {
            if (board[x][y + i] == humanPlayerC)
                score++;
            if (board[x][y + i] == 'R' && board[x][y + i] == compPlayerC)
                break;
            if (score >= 4 && (board[x][y + 1] == humanPlayerC || board[x][y - 1] == humanPlayerC)) {
                finalScore++;
                done = true;
            }
        }
    score = 0;
    done = false;
    for (i = -3; i <= 2 && !done; i++)
        if ((y + i >= 0) && (x + i >= 0) && (y + i < n) && (y + i < n)) {
            if (board[x + i][y + i] == humanPlayerC)
                score++;
            if (board[x + i][y + i] == 'R' && board[x + i][y + 1] == compPlayerC)
                break;
            if (score >= 4 && (board[x + 1][y + 1] == humanPlayerC || board[x - 1][y - 1] == humanPlayerC)) {
                finalScore++;
                done = true;
            }
        }
    score = 0;
    for (i = -2; i <= 3 && !done; i++)
        if ((y + i >= 0) && (x + i >= 0) && (y + i < n) && (x + i < n)) {
            if (board[x + i][y + i] == humanPlayerC)
                score++;
            if (board[x + i][y + i] == 'R' && board[x + i][y + 1] == compPlayerC)
                break;
            if (score >= 4 && (board[x + 1][y + 1] == humanPlayerC || board[x - 1][y - 1] == humanPlayerC)) {
                finalScore++;
                done = true;
            }
        }
    score = 0;
    done = false;
    for (i = -3; i <= 2 && !done; i++)
        if ((y + i >= 0) && (x - i >= 0) && (y + i < n) && (x - i < n)) {
            if (board[x - i][y + i] == humanPlayerC)
                score++;
            if (board[x - i][y + i] == 'R' && board[x - i][y + 1] == compPlayerC)
                break;
            if (score >= 4 && (board[x - 1][y + 1] == humanPlayerC || board[x + 1][y - 1] == humanPlayerC)) {
                finalScore++;
                done = true;
            }
        }
    score = 0;
    for (i = -2; i <= 3 && !done; i++)
        if ((y + i >= 0) && (x - i >= 0) && (y + i < n) && (x - i < n)) {
            if (board[x - i][y + i] == humanPlayerC)
                score++;
            if (board[x - i][y + i] == 'R' && board[x - i][y + 1] == compPlayerC)
                break;
            if (score >= 4 && (board[x - 1][y + 1] == humanPlayerC || board[x + 1][y - 1] == humanPlayerC)) {
                finalScore++;
                done = true;
            }
        }
    board[x][y] = 'U';
    if (finalScore >= 2)
        return true;
    else
        return false;
}

/*Uses the ifBlockException function to check potential risks spots that need
 to be blocked. It then checks the board 2 turns further to see if the
 opponent has a sure win and if this spot needs to be blocked.
 (checks for sure win 3 moves in advance) */
blockException(int n, char board[21][21], bool humanPlayer, char humanPlayerC, char compPlayerC) {
    int x1, y1, x2, y2, y3, x3, totalScore = 0;
    char boardCopy1[21][21], boardCopy2[21][21];
    for (x1 = 0; x1 < n; x1++)
        for (y1 = 0; y1 < n; y1++)
            if (board[x1][y1] == 'U' && ifBlockException(n, board, humanPlayer, humanPlayerC, compPlayerC, x1, y1)) {
                resetBoardCopy(n, boardCopy1, board);
                boardCopy1[x1][y1] = humanPlayerC;
                resetBoardCopy(n, boardCopy2, boardCopy1);

                for (x2 = 0; x2 < n; x2++)
                    for (y2 = 0; y2 < n; y2++) {
                        if (boardCopy2[x2][y2] == 'U' && findLongest(boardCopy2, x2, y2, humanPlayer) == 5)
                            boardCopy2[x2][y2] = humanPlayerC;
                    }
                for (x2 = 0; x2 < n; x2++)
                    for (y2 = 0; y2 < n; y2++)
                        if (boardCopy2[x2][y2] == 'U' && findLongest(boardCopy2, x2, y2, humanPlayer) >= 6)
                            totalScore++;
                for (x2 = 0; x2 < n; x2++)
                    for (y2 = 0; y2 < n; y2++)
                        if (boardCopy1[x2][y2] == 'U' && findLongest(boardCopy1, x2, y2, humanPlayer) >= 6)
                            totalScore++;


                if (totalScore >= 4) {
                    board[x1][y1] = compPlayerC;
                    printf("Computer moves %c at %d %d\n", compPlayerC, x1, y1);
                    printBoard(n, board);
                    return true;
                }
            }
    return false;

}
/*Uses the ifBlockException function to check potential win spots that can be
 played on. It then checks the board 2 turns further to see if the
 computer has a sure win and if this spot should be played on.
 (checks for sure win 3 moves in advance) */
connectTrap(int n, char board[21][21], bool humanPlayer, char humanPlayerC, char compPlayerC) {
    int x1, y1, x2, y2, y3, x3, totalScore = 0;
    char boardCopy1[21][21], boardCopy2[21][21];
    for (x1 = 0; x1 < n; x1++)
        for (y1 = 0; y1 < n; y1++)
            if (board[x1][y1] == 'U' && ifBlockException(n, board, !humanPlayer, compPlayerC, humanPlayerC, x1, y1)) {
                resetBoardCopy(n, boardCopy1, board);
                boardCopy1[x1][y1] = compPlayerC;
                resetBoardCopy(n, boardCopy2, boardCopy1);

                for (x2 = 0; x2 < n; x2++)
                    for (y2 = 0; y2 < n; y2++) {
                        if (boardCopy2[x2][y2] == 'U' && findLongest(boardCopy2, x2, y2, !humanPlayer) == 5)
                            boardCopy2[x2][y2] = compPlayerC;
                    }
                for (x2 = 0; x2 < n; x2++)
                    for (y2 = 0; y2 < n; y2++)
                        if (boardCopy2[x2][y2] == 'U' && findLongest(boardCopy2, x2, y2, !humanPlayer) >= 6)
                            totalScore++;
                for (x2 = 0; x2 < n; x2++)
                    for (y2 = 0; y2 < n; y2++)
                        if (boardCopy1[x2][y2] == 'U' && findLongest(boardCopy1, x2, y2, !humanPlayer) >= 6)
                            totalScore++;


                if (totalScore >= 4) {
                    board[x1][y1] = compPlayerC;
                    printf("Computer moves %c at %d %d\n", compPlayerC, x1, y1);
                    printBoard(n, board);
                    return true;
                }
            }
    return false;

}
/*Checks the amount of free space for the spot on the board that is passed.
 Free area is defined below. */
int largestFree(int n, char board[21][21], bool humanPlayer, char humanPlayerC, char compPlayerC, int x, int y) {
    int left, right, up, down, upRight, upLeft, downRight, downLeft, score;
    for (left = 0; y - left - 1 >= 0 && left <= 4 && (board[x][y - left - 1] == compPlayerC || board[x][y - left - 1] == 'U'); left++);
    for (right = 0; y + right + 1 >= 0 && right <= 4 && (board[x][y + right + 1] == compPlayerC || board[x][y + right + 1] == 'U'); right++);
    for (up = 0; x - up - 1 >= 0 && up <= 4 && (board[x - up - 1][y] == compPlayerC || board[x - up - 1][y] == 'U'); up++);
    for (down = 0; x + down + 1 >= 0 && down <= 4 && (board[x + down + 1][y] == compPlayerC || board[x + down + 1][y] == 'U'); down++);
    for (upLeft = 0; x - upLeft - 1 >= 0 && y - upLeft - 1 >= 0 && upLeft <= 4 && (board[x - upLeft - 1][y - upLeft - 1] == compPlayerC || board[x - upLeft - 1][y - upLeft - 1] == 'U'); upLeft++);
    for (upRight = 0; x - upRight - 1 >= 0 && y + upRight + 1 >= 0 && upRight <= 4 && (board[x - upRight - 1][y + upRight + 1] == compPlayerC || board[x - upRight - 1][y + upRight + 1] == 'U'); upRight++);
    for (downRight = 0; x + downRight + 1 >= 0 && y + downRight + 1 >= 0 && downRight <= 4 && (board[x + downRight + 1][y + downRight + 1] == compPlayerC || board[x + downRight + 1][y + downRight + 1] == 'U'); downRight++);
    for (downLeft = 0; x + downLeft + 1 >= 0 && y - downLeft - 1 >= 0 && downLeft <= 4 && (board[x + downLeft + 1][y - downLeft - 1] == compPlayerC || board[x + downLeft + 1][y - downLeft - 1] == 'U'); downLeft++);
    score = left + right + up + down + upRight + upLeft + downRight + downLeft;
    return score;
}
/*Loops through the entire board to find the spot with the most free space 
 around it. Free area is defined by amount of spots in each direction that are 
 either or already contain the computer's piece. */
void largestFreeLoop(int n, char board[21][21], bool humanPlayer, char humanPlayerC, char compPlayerC) {
    int x, y, score = -1, bestScore = -1, xMove = 0, yMove = 0;
    for (x = 0; x < n; x++)
        for (y = 0; y < n; y++)
            if (board[x][y] == 'U') {
                score = largestFree(n, board, humanPlayer, humanPlayerC, compPlayerC, x, y);
                if (score > bestScore) {
                    bestScore = score;
                    xMove = x;
                    yMove = y;
                }
            }
    board[xMove][yMove] = compPlayerC;
    printf("Computer moves %c at %d %d\n", compPlayerC, xMove, yMove);
    printBoard(n, board);

}

/*All of these functions will set the passed board to the blueprint of the
 trap the contained. The passed blueprint board can then be used to compare to
 the game board. */
bool trap1(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 8; x1++)
        for (y1 = 0; y1 < 8; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[0][0] = 'E';
    trapBoard[3][0] = 'E';
    trapBoard[3][2] = 'E';
    trapBoard[3][3] = 'X';
    trapBoard[3][4] = 'E';
    trapBoard[3][6] = 'E';
    trapBoard[1][2] = 'E';
    trapBoard[4][4] = 'E';
    trapBoard[6][6] = 'E';
    trapBoard[5][2] = 'E';
    trapBoard[7][2] = 'E';
    trapBoard[1][1] = 'M';
    trapBoard[2][2] = 'M';
    trapBoard[3][1] = 'M';
    trapBoard[4][2] = 'M';
    trapBoard[6][2] = 'M';
    trapBoard[3][5] = 'M';
    trapBoard[5][5] = 'M';
}

bool trap2(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 8; x1++)
        for (y1 = 0; y1 < 8; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[0][4] = 'E';
    trapBoard[0][7] = 'E';
    trapBoard[2][0] = 'E';
    trapBoard[3][4] = 'X';
    trapBoard[2][2] = 'E';
    trapBoard[2][4] = 'E';
    trapBoard[2][6] = 'E';
    trapBoard[4][3] = 'E';
    trapBoard[4][4] = 'E';
    trapBoard[6][4] = 'E';
    trapBoard[6][1] = 'E';
    trapBoard[1][4] = 'M';
    trapBoard[1][6] = 'M';
    trapBoard[2][1] = 'M';
    trapBoard[2][3] = 'M';
    trapBoard[2][5] = 'M';
    trapBoard[5][2] = 'M';
    trapBoard[5][4] = 'M';
}

bool trap3(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 8; x1++)
        for (y1 = 0; y1 < 8; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[0][5] = 'E';
    trapBoard[1][1] = 'E';
    trapBoard[3][3] = 'E';
    trapBoard[4][4] = 'X';
    trapBoard[4][1] = 'E';
    trapBoard[4][3] = 'E';
    trapBoard[4][5] = 'E';
    trapBoard[4][7] = 'E';
    trapBoard[6][5] = 'E';
    trapBoard[7][7] = 'E';
    trapBoard[2][5] = 'E';
    trapBoard[1][5] = 'M';
    trapBoard[2][2] = 'M';
    trapBoard[3][5] = 'M';
    trapBoard[4][2] = 'M';
    trapBoard[4][6] = 'M';
    trapBoard[5][5] = 'M';
    trapBoard[6][6] = 'M';
}

bool trap4(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 8; x1++)
        for (y1 = 0; y1 < 8; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[1][3] = 'E';
    trapBoard[1][6] = 'E';
    trapBoard[3][3] = 'E';
    trapBoard[4][3] = 'X';
    trapBoard[3][4] = 'E';
    trapBoard[5][1] = 'E';
    trapBoard[5][3] = 'E';
    trapBoard[5][5] = 'E';
    trapBoard[5][7] = 'E';
    trapBoard[7][0] = 'E';
    trapBoard[7][3] = 'E';
    trapBoard[2][3] = 'M';
    trapBoard[2][5] = 'M';
    trapBoard[5][2] = 'M';
    trapBoard[5][4] = 'M';
    trapBoard[5][6] = 'M';
    trapBoard[6][1] = 'M';
    trapBoard[6][3] = 'M';
}

bool trap5(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 8; x1++)
        for (y1 = 0; y1 < 8; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[0][6] = 'E';
    trapBoard[2][4] = 'E';
    trapBoard[3][0] = 'E';
    trapBoard[4][2] = 'X';
    trapBoard[1][2] = 'E';
    trapBoard[3][2] = 'E';
    trapBoard[3][4] = 'E';
    trapBoard[3][6] = 'E';
    trapBoard[5][2] = 'E';
    trapBoard[6][0] = 'E';
    trapBoard[7][2] = 'E';
    trapBoard[1][5] = 'M';
    trapBoard[2][2] = 'M';
    trapBoard[3][1] = 'M';
    trapBoard[3][3] = 'M';
    trapBoard[3][5] = 'M';
    trapBoard[5][1] = 'M';
    trapBoard[6][2] = 'M';
}

bool trap6(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 8; x1++)
        for (y1 = 0; y1 < 8; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[1][0] = 'E';
    trapBoard[1][3] = 'E';
    trapBoard[3][2] = 'E';
    trapBoard[5][4] = 'X';
    trapBoard[5][1] = 'E';
    trapBoard[5][3] = 'E';
    trapBoard[5][5] = 'E';
    trapBoard[5][7] = 'E';
    trapBoard[7][3] = 'E';
    trapBoard[7][6] = 'E';
    trapBoard[3][3] = 'E';
    trapBoard[2][1] = 'M';
    trapBoard[2][3] = 'M';
    trapBoard[4][3] = 'M';
    trapBoard[5][2] = 'M';
    trapBoard[5][6] = 'M';
    trapBoard[6][3] = 'M';
    trapBoard[6][5] = 'M';
}

bool trap7(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 8; x1++)
        for (y1 = 0; y1 < 8; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[0][4] = 'E';
    trapBoard[1][6] = 'E';
    trapBoard[2][4] = 'E';
    trapBoard[3][4] = 'X';
    trapBoard[4][0] = 'E';
    trapBoard[4][2] = 'E';
    trapBoard[4][4] = 'E';
    trapBoard[4][6] = 'E';
    trapBoard[5][2] = 'E';
    trapBoard[6][4] = 'E';
    trapBoard[7][0] = 'E';
    trapBoard[1][4] = 'M';
    trapBoard[2][5] = 'M';
    trapBoard[4][1] = 'M';
    trapBoard[4][3] = 'M';
    trapBoard[4][5] = 'M';
    trapBoard[5][4] = 'M';
    trapBoard[6][1] = 'M';
}

bool trap8(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 8; x1++)
        for (y1 = 0; y1 < 8; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[0][1] = 'E';
    trapBoard[0][4] = 'E';
    trapBoard[2][0] = 'E';
    trapBoard[2][3] = 'X';
    trapBoard[2][2] = 'E';
    trapBoard[2][4] = 'E';
    trapBoard[2][6] = 'E';
    trapBoard[4][4] = 'E';
    trapBoard[4][5] = 'E';
    trapBoard[6][4] = 'E';
    trapBoard[6][7] = 'E';
    trapBoard[1][2] = 'M';
    trapBoard[1][4] = 'M';
    trapBoard[2][1] = 'M';
    trapBoard[2][5] = 'M';
    trapBoard[3][4] = 'M';
    trapBoard[5][4] = 'M';
    trapBoard[5][6] = 'M';
}

bool trap9(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 10; x1++)
        for (y1 = 0; y1 < 10; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[0][1] = 'E';
    trapBoard[4][0] = 'E';
    trapBoard[3][4] = 'E';
    trapBoard[4][1] = 'X';
    trapBoard[4][4] = 'E';
    trapBoard[4][5] = 'E';
    trapBoard[6][1] = 'E';
    trapBoard[8][4] = 'E';
    trapBoard[1][1] = 'M';
    trapBoard[2][1] = 'M';
    trapBoard[3][1] = 'M';
    trapBoard[4][2] = 'M';
    trapBoard[4][3] = 'M';
    trapBoard[5][4] = 'M';
    trapBoard[6][4] = 'M';
    trapBoard[7][4] = 'M';
}

bool trap10(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 10; x1++)
        for (y1 = 0; y1 < 10; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[0][1] = 'E';
    trapBoard[4][0] = 'E';
    trapBoard[3][4] = 'E';
    trapBoard[4][4] = 'X';
    trapBoard[4][1] = 'E';
    trapBoard[4][5] = 'E';
    trapBoard[5][1] = 'E';
    trapBoard[8][4] = 'E';
    trapBoard[1][1] = 'M';
    trapBoard[2][1] = 'M';
    trapBoard[3][1] = 'M';
    trapBoard[4][2] = 'M';
    trapBoard[4][3] = 'M';
    trapBoard[5][4] = 'M';
    trapBoard[6][4] = 'M';
    trapBoard[7][4] = 'M';
}

bool trap11(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 10; x1++)
        for (y1 = 0; y1 < 10; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[0][4] = 'E';
    trapBoard[1][3] = 'E';
    trapBoard[1][8] = 'E';
    trapBoard[1][4] = 'X';
    trapBoard[4][4] = 'E';
    trapBoard[4][5] = 'E';
    trapBoard[5][4] = 'E';
    trapBoard[4][0] = 'E';
    trapBoard[1][5] = 'M';
    trapBoard[1][6] = 'M';
    trapBoard[1][7] = 'M';
    trapBoard[2][4] = 'M';
    trapBoard[3][4] = 'M';
    trapBoard[4][3] = 'M';
    trapBoard[4][2] = 'M';
    trapBoard[4][1] = 'M';
}

bool trap12(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 10; x1++)
        for (y1 = 0; y1 < 10; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[0][4] = 'E';
    trapBoard[1][3] = 'E';
    trapBoard[1][8] = 'E';
    trapBoard[4][4] = 'X';
    trapBoard[1][4] = 'E';
    trapBoard[4][5] = 'E';
    trapBoard[5][4] = 'E';
    trapBoard[4][0] = 'E';
    trapBoard[1][5] = 'M';
    trapBoard[1][6] = 'M';
    trapBoard[1][7] = 'M';
    trapBoard[2][4] = 'M';
    trapBoard[3][4] = 'M';
    trapBoard[4][3] = 'M';
    trapBoard[4][2] = 'M';
    trapBoard[4][1] = 'M';
}

bool trap13(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 10; x1++)
        for (y1 = 0; y1 < 10; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[0][1] = 'E';
    trapBoard[1][0] = 'E';
    trapBoard[1][1] = 'E';
    trapBoard[4][1] = 'X';
    trapBoard[1][5] = 'E';
    trapBoard[4][0] = 'E';
    trapBoard[5][1] = 'E';
    trapBoard[4][5] = 'E';
    trapBoard[1][2] = 'M';
    trapBoard[1][3] = 'M';
    trapBoard[1][4] = 'M';
    trapBoard[2][1] = 'M';
    trapBoard[3][1] = 'M';
    trapBoard[4][2] = 'M';
    trapBoard[4][3] = 'M';
    trapBoard[4][4] = 'M';
}

bool trap14(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 10; x1++)
        for (y1 = 0; y1 < 10; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[0][1] = 'E';
    trapBoard[0][4] = 'E';
    trapBoard[4][0] = 'E';
    trapBoard[4][4] = 'X';
    trapBoard[5][4] = 'E';
    trapBoard[4][5] = 'E';
    trapBoard[4][1] = 'E';
    trapBoard[5][1] = 'E';
    trapBoard[1][1] = 'M';
    trapBoard[2][1] = 'M';
    trapBoard[3][1] = 'M';
    trapBoard[4][2] = 'M';
    trapBoard[4][3] = 'M';
    trapBoard[3][4] = 'M';
    trapBoard[2][4] = 'M';
    trapBoard[1][4] = 'M';
}

bool trap15(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 10; x1++)
        for (y1 = 0; y1 < 10; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[1][0] = 'E';
    trapBoard[0][4] = 'E';
    trapBoard[1][5] = 'E';
    trapBoard[1][4] = 'X';
    trapBoard[4][0] = 'E';
    trapBoard[4][4] = 'E';
    trapBoard[5][4] = 'E';
    trapBoard[4][5] = 'E';
    trapBoard[1][1] = 'M';
    trapBoard[1][2] = 'M';
    trapBoard[1][3] = 'M';
    trapBoard[2][4] = 'M';
    trapBoard[3][4] = 'M';
    trapBoard[4][3] = 'M';
    trapBoard[4][2] = 'M';
    trapBoard[4][1] = 'M';
}

bool trap16(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 10; x1++)
        for (y1 = 0; y1 < 10; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[1][0] = 'E';
    trapBoard[0][1] = 'E';
    trapBoard[0][4] = 'E';
    trapBoard[1][1] = 'X';
    trapBoard[1][4] = 'E';
    trapBoard[1][5] = 'E';
    trapBoard[5][4] = 'E';
    trapBoard[5][1] = 'E';
    trapBoard[4][1] = 'M';
    trapBoard[3][1] = 'M';
    trapBoard[2][1] = 'M';
    trapBoard[1][2] = 'M';
    trapBoard[1][3] = 'M';
    trapBoard[2][4] = 'M';
    trapBoard[3][4] = 'M';
    trapBoard[4][4] = 'M';
}

bool trap17(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 10; x1++)
        for (y1 = 0; y1 < 10; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[1][0] = 'E';
    trapBoard[1][4] = 'E';
    trapBoard[0][4] = 'E';
    trapBoard[4][4] = 'X';
    trapBoard[1][5] = 'E';
    trapBoard[4][8] = 'E';
    trapBoard[4][3] = 'E';
    trapBoard[5][4] = 'E';
    trapBoard[1][1] = 'M';
    trapBoard[1][2] = 'M';
    trapBoard[1][3] = 'M';
    trapBoard[2][4] = 'M';
    trapBoard[3][4] = 'M';
    trapBoard[4][5] = 'M';
    trapBoard[4][6] = 'M';
    trapBoard[4][7] = 'M';
}

bool trap18(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 10; x1++)
        for (y1 = 0; y1 < 10; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[0][4] = 'E';
    trapBoard[3][1] = 'E';
    trapBoard[4][0] = 'E';
    trapBoard[4][4] = 'X';
    trapBoard[4][1] = 'E';
    trapBoard[4][5] = 'E';
    trapBoard[5][4] = 'E';
    trapBoard[8][1] = 'E';
    trapBoard[1][4] = 'M';
    trapBoard[2][4] = 'M';
    trapBoard[3][4] = 'M';
    trapBoard[4][3] = 'M';
    trapBoard[4][2] = 'M';
    trapBoard[5][1] = 'M';
    trapBoard[6][1] = 'M';
    trapBoard[7][1] = 'M';
}

bool trap19(int n, char trapBoard[10][10]) {
    int x1, y1;
    for (x1 = 0; x1 < 10; x1++)
        for (y1 = 0; y1 < 10; y1++)
            trapBoard[x1][y1] = 'U';
    trapBoard[0][3] = 'E';
    trapBoard[0][5] = 'E';
    trapBoard[1][4] = 'E';
    trapBoard[2][5] = 'X';
    trapBoard[5][0] = 'E';
    trapBoard[6][1] = 'E';
    trapBoard[5][8] = 'E';
    trapBoard[1][6] = 'E';
    trapBoard[2][3] = 'M';
    trapBoard[3][2] = 'M';
    trapBoard[3][4] = 'M';
    trapBoard[3][6] = 'M';
    trapBoard[4][1] = 'M';
    trapBoard[4][3] = 'M';
    trapBoard[4][7] = 'M';
    trapBoard[5][2] = 'M';
}

/*Checks if the computer is closer to completing the trap passed compared to 
 * other traps checked and saves the location to play next if this particular
 * trap is closest to being completed at some point on the board.*/

bool nextTrap(int n, char board[21][21], bool humanPlayer, char humanPlayerC, char compPlayerC, char trapBoard[10][10], int * bestScore, int * moveX, int * moveY, int finalCount) {
    int x1, y1, x2, y2, score = 0, trapStartx, trapStarty;
    bool valid, trapSet = false;
    for (x1 = 0; x1 < n; x1++)
        for (y1 = 0; y1 < n; y1++) {
            valid = true;
            score = 0;
            for (x2 = 0; x2 < 10 && valid; x2++)
                for (y2 = 0; y2 < 10 && valid; y2++) {
                    if (trapBoard[x2][y2] == 'E' && board[x2 + x1][y2 + y1] != 'U')
                        valid = false;
                    else if (trapBoard[x2][y2] == 'M' && board[x2 + x1][y2 + y1] == compPlayerC)
                        score++;
                    else if (trapBoard[x2][y2] == 'M' && board[x2 + x1][y2 + y1] != 'U')
                        valid = false;
                    else if (trapBoard[x2][y2] == 'X' && board[x2 + x1][y2 + y1] != 'U')
                        valid = false;
                }
            if ((score > *bestScore) && valid) {
                *bestScore = score;
                trapStartx = x1;
                trapStarty = y1;
                trapSet = true;

                if (score >= finalCount)
                    for (x1 = 0; x1 < 10; x1++)
                        for (y1 = 0; y1 < 10; y1++)
                            if (trapBoard[x1][y1] == 'X') {
                                *moveX = x1 + trapStartx;
                                *moveY = y1 + trapStarty;
                                return true;
                            }
            }
        }
    if (trapSet) {
        for (x1 = 0; x1 < n; x1++)
            for (y1 = 0; y1 < n; y1++)
                if (x1 < 8 && y1 < 8 && trapBoard [x1][y1] == 'M' && (board[x1 + trapStartx][y1 + trapStarty]) == 'U') {
                    *moveX = x1 + trapStartx;
                    *moveY = y1 + trapStarty;
                    return false;
                }
    }
    return false;
}

//This function detects whether the opposing (human)player is one set away from
//completing the trap passed at any spot on the board
bool detectTrap(int n, char board[21][21], bool humanPlayer, char humanPlayerC, char compPlayerC, char trapBoard[10][10]) {
    int x1, y1, x2, y2;
    bool trapSet = true;
    for (x1 = 0; x1 < n; x1++)
        for (y1 = 0; y1 < n; y1++) {
            trapSet = true;


            for (x2 = 0; x2 < 8 && trapSet; x2++)
                for (y2 = 0; y2 < 8 && trapSet; y2++) {

                    if (trapBoard[x2][y2] == 'M' && board[x2 + x1][y2 + y1] != humanPlayerC)
                        trapSet = false;
                    if (trapBoard[x2][y2] == 'E' && board[x2 + x1][y2 + y1] != 'U' && board[x2 + x1][y2 + y1] != humanPlayer)
                        trapSet = false;
                    if (trapBoard[x2][y2] == 'X' && board[x2 + x1][y2 + y1] != 'U')
                        trapSet = false;
                }
            if (trapSet)
                for (x2 = 0; x2 < 8; x2++)
                    for (y2 = 0; y2 < 8; y2++)
                        if (trapBoard[x2][y2] == 'X' && board[x1 + x2][y1 + y2] == 'U') {
                            board[x1 + x2][y1 + y2] = compPlayerC;
                            printf("Computer moves %c at %d %d\n", compPlayerC, x1 + x2, y1 + y2);
                            printBoard(n, board);
                            return true;
                        }

        }
    return false;
}

/*This function creates a blueprint trap board for every trap layout saved,
 * and for every trap checks how close it is to completing that trap at each
 * orientation. It will then play at the spot where it is closest to completing
 * any trap.*/

bool traps(int n, char board[21][21], bool humanPlayer, char humanPlayerC, char compPlayerC) {
    int bestScore = -1;
    int moveX = -1;
    int moveY = -1;
    char trapBoard[10][10];
    bool done = false;
    trap1(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 7))
        done = true;
    trap2(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 7))
        done = true;
    trap3(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 7))
        done = true;
    trap4(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 7))
        done = true;
    trap5(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 7))
        done = true;
    trap6(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 7))
        done = true;
    trap7(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 7))
        done = true;
    trap8(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 7))
        done = true;
    trap9(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 8))
        done = true;
    trap10(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 8))
        done = true;
    trap11(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 8))
        done = true;
    trap12(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 8))
        done = true;
    trap13(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 8))
        done = true;
    trap14(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 8))
        done = true;
    trap15(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 8))
        done = true;
    trap16(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 8))
        done = true;
    trap17(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 8))
        done = true;
    trap18(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 8))
        done = true;
    trap19(n, trapBoard);
    if (!done && nextTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard, &bestScore, &moveX, &moveY, 8))
        done = true;
    if (moveX == -1 && moveY == -1)
        return false;
    else if (board[moveX][moveY] == 'U') {
        board[moveX][moveY] = compPlayerC;
        printf("Computer moves %c at %d %d\n", compPlayerC, moveX, moveY);
        printBoard(n, board);
        return true;
    }
    return false;
}

/*This function creates a blueprint trap board for every trap layout saved,
 * and then checks if the enemy has this trap created in order to block it using
 * the detectTrap function. */
bool detectEnemyTraps(int n, char board[21][21], bool humanPlayer, char humanPlayerC, char compPlayerC) {
    char trapBoard[10][10];
    trap1(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap2(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap3(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap4(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap5(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap6(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap7(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap8(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap9(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap10(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap11(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap12(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap13(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap14(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap15(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap16(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap17(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap18(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    trap19(n, trapBoard);
    if (detectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC, trapBoard))
        return true;
    return false;
}
//This function checks if it's the computers first move of the game
bool firstMove(int n, char board[21][21], char compPlayerC) {
    int x, y;
    for (x = 0; x < n; x++)
        for (y = 0; y < n; y++)
            if (board[x][y] == compPlayerC)
                return false;
    return true;
}

/*This function plays a computer turns by going through a series of functions 
 * in order of priority and plays if that functions find that type of move.
 * The last function in the order will always play a move until the game is over
 * and this function is made to only make one move per turn.
 */
void computerTurn(int n, char board[21][21], bool humanPlayer) {
    int x, y;
    char humanPlayerC, compPlayerC;
    if (humanPlayer) {
        humanPlayerC = 'W';
        compPlayerC = 'B';
    } else if (!humanPlayer) {
        humanPlayerC = 'B';
        compPlayerC = 'W';
    }
    struct rusage usage; // a structure to hold "resource usage" (including time)
    struct timeval start, end; // will hold the start and end times
    getrusage(RUSAGE_SELF, &usage);
    start = usage.ru_utime;
    double time_start = start.tv_sec + start.tv_usec / 1000000.0; // in seconds
    /* PLACE THE CODE YOU WISH TO TIME HERE */
    if (firstMove(n, board, compPlayerC))
        largestFreeLoop(n, board, humanPlayer, humanPlayerC, compPlayerC);
    else if (connect6(n, board, humanPlayer, compPlayerC));
    else if (block6(n, board, humanPlayer, compPlayerC));
    else if (connect5(n, board, humanPlayer, humanPlayerC, compPlayerC));
    else if (block5(n, board, humanPlayer, humanPlayerC, compPlayerC));
    else if (connectTrap(n, board, humanPlayer, humanPlayerC, compPlayerC));
    else if (blockException(n, board, humanPlayer, humanPlayerC, compPlayerC));
    //Detect Enemy Traps takes up a lot of time and may not be worth using since
    //enemies may not have such complex traps
    else if (detectEnemyTraps(n, board, humanPlayer, humanPlayerC, compPlayerC));  
    else if (traps(n, board, humanPlayer, humanPlayerC, compPlayerC));
    else largestFreeLoop(n, board, humanPlayer, humanPlayerC, compPlayerC);


    /* PLACE THE CODE YOU WISH TO TIME HERE */
    getrusage(RUSAGE_SELF, &usage);
    end = usage.ru_utime;
    double time_end = end.tv_sec + end.tv_usec / 1000000.0; // in seconds
    double total_time = time_end - time_start; // total_time now holds the time
    printf("Total Time: %lf\n", total_time);

}
//This function will check for a winner and return the winner or no winner.

char checkWinner(int n, char board[21][21]) {
    int x, y;
    bool found = false;
    bool spot;
    char winner = 'N';
    for (x = 0; x < n && !found; x++)
        for (y = 0; y < n && !found; y++) {
            if (board[x][y] == 'W')
                spot = true;
            else if (board[x][y] == 'B')
                spot = false;
            if ((board[x][y] == 'W' || board[x][y] == 'B') && findLongest(board, x, y, spot) >= 6)
                winner = board[x][y];
        }
    if (winner == 'B')
        printf("\nBlack player wins.\n");
    if (winner == 'W')
        printf("\nWhite player wins.\n");

    return winner;

}
//Checks if the board is full

bool isBoardFull(int n, char board[21][21]) {
    bool notFull = false;
    int x, y;
    for (x = 0; x < n && !notFull; x++)
        for (y = 0; y < n && !notFull; y++) {
            if (board[x][y] == 'U')
                notFull = true;
        }
    if (!notFull)
        printf("Draw!\n");

    return !notFull;
}

/*This function alternates between user and the computer and handles
moves for both.*/
void playGame(int n, char board[21][21], bool humanPlayer) {
    int x = 0, y = 0;
    bool turn = !humanPlayer; //false is computer
    while (true) {
        if (turn) {
            humanTurn(n, board, humanPlayer);
            turn = !turn;
            if (checkWinner(n, board) != 'N')
                break;
            if (isBoardFull(n, board))
                break;
        }
        if (!turn) {
            computerTurn(n, board, humanPlayer);
            turn = !turn;
            if (checkWinner(n, board) != 'N')
                break;
            if (isBoardFull(n, board))

                break;
        }

    }
    return;
}

//This function will loop to set all the blocked spaces that the user determines

void setBlocked(int n, char board[21][21]) {
    int x, y;
    printf("Enter position (ROW COL) of blocked square; (-1 -1) to finish: ");
    scanf("%d %d", &x, &y);
    while (!(x == -1 && y == -1)) {
        if (board[x][y] == 'U') {
            board[x][y] = 'R';
            printBoard(n, board);
        } else if (x <= -1 || y <= -1 || x >= n || y >= n)
            printf("Out of range row or column\n");

        else
            printf("Position already occupied, try again\n");
        printf("Enter position (ROW COL) of blocked square; (-1 -1) to finish: ");
        scanf("%d %d", &x, &y);
    }
    return;
}

//The main game function which calls all other functions to be initialized.

int main(void) {
    int size;
    printf("Enter board dimensions (n), n>=6 and n<=21: ");
    scanf("%d", &size);
    char board[21][21];
    createBoard(size, board);
    char compPlayer[10];
    bool humanUserWhite;
    printf("Computer playing B or W?: ");
    scanf("%s", &compPlayer);
    if (compPlayer[0] == 'B')
        humanUserWhite = true;
    else
        humanUserWhite = false;
    setBlocked(size, board);
    playGame(size, board, humanUserWhite);


    return (EXIT_SUCCESS);
}