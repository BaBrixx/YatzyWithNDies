/*
//  main.c
//  Yatzy with N dice
//
//  Created by Bastian Brix Hansen on 29.10.2021.
//  Copyright © 2021 Bastian Brix Hansen. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Shoud be implemented?
enum round {ones, twos, threes, fours, fives, sixes, bonus,
            onePair, twoPairs, threeKind, fourKind, smallStraight, 
            largeStraight, fullHouse, chance, yatzy};
*/

#define DIE_MAX_EYES 6

void rollMultipleDies(int, int*);
void dicePrEye(int, int*, int*);
void playUpper(int, int*, int*);
void playOnePair(int, int*, int*);
void playTwoPairs(int, int*, int*);
void playOfAKind(int, int*, int*);
void playStraight(int, int*, int*);
void playFullHouse(int, int*, int*);
void playChance(int, int*, int*);
void playYatzy(int, int*, int*);
void printScore(int*);

int main(void) {
    int *dicePtr, *score, dice, programActive = 1;

    /* Seeding the rand function */
    srand(time(0));

    while (programActive == 1) {
        /* Acquiring the number of dice */
        printf("The number of dice: ");
        scanf("%d", &dice);

        /* Allocating memory to the array of doubles */
        dicePtr = (int*) malloc(dice * sizeof(int));
        score = (int*) malloc(16 * sizeof(int));

        if (dicePtr == NULL) {
            printf("Memory not allocated.\n");
            exit(0);
        }
        else {
            playUpper(dice, dicePtr, score);
            playOnePair(dice, dicePtr, score);
            playTwoPairs(dice, dicePtr, score);
            playOfAKind(dice, dicePtr, score);
            playStraight(dice, dicePtr, score);
            playFullHouse(dice, dicePtr, score);
            playChance(dice, dicePtr, score);
            playYatzy(dice, dicePtr, score);

            /* Printing the score */
            printScore(score);

            /* Deallocating the memory for the array */
            free(dicePtr);
            free(score);

            char tempContinue;

            printf("\nWould you like to continue? (y/n)\n");
            scanf(" %c", &tempContinue);

            if (tempContinue != 'y') {
                programActive = 0;
            }

        }
    }

    return EXIT_SUCCESS;
}

void rollMultipleDies(int rollTimes, int *arrayPointer) {
    for (int i = 0; i < rollTimes; i++) {
        arrayPointer[i] = (rand() % DIE_MAX_EYES) + 1;
    }
}

/* Function to count number of dice with x eyes */
void dicePrEye(int dice, int *dicePtr, int *eyesArray){
    for (int currDie = 0; currDie < dice; currDie++) {
        switch (dicePtr[currDie]) {
            case 1: eyesArray[0]++; break;
            case 2: eyesArray[1]++; break;
            case 3: eyesArray[2]++; break;
            case 4: eyesArray[3]++; break;
            case 5: eyesArray[4]++; break;
            case 6: eyesArray[5]++; break;
        }
    }
}

/* Function to play the upper section of yatzy */
void playUpper(int dice, int *dicePtr, int *scoreArray) {
    int schFor, upperTotal = 0;

    for (schFor = 1; schFor <= 6; schFor++) {
        rollMultipleDies(dice, dicePtr);

        int currDice = 0;
        int diceFound = 0;
        while ((currDice) < dice && diceFound < 5) {
            if (dicePtr[currDice] == schFor) {
                diceFound++;
            }
            currDice++;
        }

        printf("Your got %d dice with %d eyes and the rolled dice were: ", diceFound, schFor);
        for (int i = 0; i < dice; i++) {
            printf("%d, ",dicePtr[i]);
        }
        printf("\n");
        
        /* Adding score to the score array */
        scoreArray[schFor - 1] = diceFound * schFor;
        upperTotal += diceFound * schFor;
    }
    
    if (upperTotal >= 63) {
        scoreArray[6] = 50;
        printf("You got %d and therefore got an extra %d points!\n", upperTotal, scoreArray[6]);
    }
    else {
        scoreArray[6] = 0;
        printf("You got %d and therefore got %d extra points!\n", upperTotal, scoreArray[6]);
    }
}

/* Function for the one pair round */
void playOnePair(int dice, int *dicePtr, int *scoreArray) {
    /* Rolling the dice */
    rollMultipleDies(dice, dicePtr);

    int diceWithNEyes[] = {0,0,0,0,0,0};
    dicePrEye(dice, dicePtr, diceWithNEyes);

    /* Could have used a while loop but this is easier to read and takes almost the same amount of lines */
    if (diceWithNEyes[5] >= 2) scoreArray[7] = 12;
    else if (diceWithNEyes[4] >= 2) scoreArray[7] = 10;
    else if (diceWithNEyes[3] >= 2) scoreArray[7] = 8;
    else if (diceWithNEyes[2] >= 2) scoreArray[7] = 6;
    else if (diceWithNEyes[1] >= 2) scoreArray[7] = 4;
    else if (diceWithNEyes[0] >= 2) scoreArray[7] = 2;
    else scoreArray[7] = 0;

    printf("\nYour rolled dice were: ");
    for (int i = 0; i < dice; i++) {
        printf("%d, ",dicePtr[i]);
    }
    printf("\nYou got a pair of %d and therefore got %d points.\n", scoreArray[7]/2, scoreArray[7]);
}

/* Function for the two pairs round */
void playTwoPairs(int dice, int *dicePtr, int *scoreArray) {
    /* Rolling the dice */
    rollMultipleDies(dice, dicePtr);

    int diceWithNEyes[] = {0,0,0,0,0,0};
    dicePrEye(dice, dicePtr, diceWithNEyes);

    /* Initiating temporary pair integers */
    int i = 6, firstPair = 0, secondPair = 0;

    while (i > 0 && (firstPair == 0 || secondPair == 0)) {
        if (diceWithNEyes[i - 1] >= 2){
            if (firstPair == 0) {
                firstPair = i;
            }
            else if (firstPair != i) {
                secondPair = i;
            }
        }
        i--;
    }

    if (firstPair != 0 && secondPair != 0) {
        scoreArray[8] = (2 * firstPair) + (2 * secondPair);
    }

    printf("\nYour rolled dice were: ");
    for (int i = 0; i < dice; i++) {
        printf("%d, ",dicePtr[i]);
    }
    printf("\nYou got a pair of %d and a pair of %d. You therefore got %d points.\n", firstPair, secondPair, scoreArray[8]);
}

/* Function for the three and four of a kind rounds */
void playOfAKind(int dice, int *dicePtr, int *scoreArray) {
    int threeWSameEyes = 0, fourWSameEyes = 0;

    for (int i = 3; i < 5; i++) {
        /* Rolling the dice */
        rollMultipleDies(dice, dicePtr);

        int diceWithNEyes[] = {0,0,0,0,0,0};
        dicePrEye(dice, dicePtr, diceWithNEyes);

        /* While loop for checking if number of dice with j eyes is larger than i (3 or 4) */
        int j = 6, diseFound = 0;
        while (j > 0 && !diseFound) {
            if (diceWithNEyes[j - 1] >= i){
                (i == 3) ? (scoreArray[9] = 3 * j) : (scoreArray[10] = 4 * j);
                diseFound = 1;
            }
            j--;
        }

        /* Printing result */
        printf("\nYour rolled dice were: ");
        for (int j = 0; j < dice; j++) {
            printf("%d, ",dicePtr[j]);
        }
        printf("\nYou got %s of %d and therefore got %d points.\n", (i == 3) ? "three" : "four"
                                                                    , (i == 3) ? scoreArray[9] / 3 : scoreArray[10] / 3
                                                                    , (i == 3) ? scoreArray[9] : scoreArray[10]);
    }
}

/* Function for the small and large straight rounds */
void playStraight(int dice, int *dicePtr, int *scoreArray) {
    /* For loop for checking the two ranges 1-5 and 2-6 */
    for (int i = 1; i < 3; i++) {
        /* Rolling the dice */
        rollMultipleDies(dice, dicePtr);

        int diceWithNEyes[] = {0,0,0,0,0,0};
        dicePrEye(dice, dicePtr, diceWithNEyes);

        int dieMissing = 0;
        
        /* For loop for checking the range current range */
        for (int j = i; j < 5 + i; j++) {
            if (diceWithNEyes[j - 1] == 0) {
                dieMissing = 1;
            }
        }

        /* Adding points to the score array if none of the dices are missing in the the current range */
        if (dieMissing != 1) {
            (i == 1) ? (scoreArray[11] = 15) : (scoreArray[12] = 20);
        } 
        
        /* Printing the result */
        printf("\nYour rolled dice were: ");
        for (int j = 0; j < dice; j++) {
            printf("%d, ",dicePtr[j]);
        }
        printf("\nYou %s %s and therefore got %d points.\n", (dieMissing) ? "didnt get" : "got"
                                                           , (i == 1) ? "small straight" : "large straight"
                                                           , (i == 1) ? scoreArray[11] : scoreArray[12]);
    }
}

/* Function for the full hoouse round */
void playFullHouse(int dice, int *dicePtr, int *scoreArray) {
    /* Rolling the dice */
    rollMultipleDies(dice, dicePtr);

    int diceWithNEyes[] = {0,0,0,0,0,0};
    dicePrEye(dice, dicePtr, diceWithNEyes);

    int threeOfKind = 0, twoOfKind = 0;

    /* For loop for checking for respectivly 3 dice with x eyes and 2 dice with y eyes */
    for (int i = 2; i > 0; i--) {
        int j = 6, diceFound = 0;

        while (j > 0 && diceFound != 1) {
            if (diceWithNEyes[j - 1] >= i + 1 && threeOfKind != j) {
                (i == 2) ? (threeOfKind = j) : (twoOfKind = j);
                diceFound = 1;
            }
            j--;
        }
    }

    printf("\nYour rolled dice were: ");
    for (int i = 0; i < dice; i++) {
        printf("%d, ",dicePtr[i]);
    }

    if (threeOfKind != 0 && twoOfKind != 0) {
        scoreArray[13] = (3 * threeOfKind) + (2 * twoOfKind);
        printf("\nYou got two %d and three %d therefore got %d points.\n", twoOfKind, threeOfKind, scoreArray[13]);
    }
    else {
        printf("\nYou did not get full house and therefore didnt get any points.\n");
    }
}

/* Function for the chance round */
void playChance(int dice, int *dicePtr, int *scoreArray) {
    /* Rolling the dice */
    rollMultipleDies(dice, dicePtr);

    int diceWithNEyes[] = {0,0,0,0,0,0};
    dicePrEye(dice, dicePtr, diceWithNEyes);

    int diceNeeded = 5;
    int tempSum = 0;

    for (int i = 6; i > 0; i--) {
        if (diceWithNEyes[i - 1] > diceNeeded) {
            tempSum += diceNeeded * i;
            diceNeeded -= diceNeeded;
        }
        else {
            tempSum += diceWithNEyes[i - 1] * i;
            diceNeeded -= diceWithNEyes[i - 1];
        }
    }

    scoreArray[14] = tempSum;

    printf("\nYour rolled dice were: ");
    for (int i = 0; i < dice; i++) {
        printf("%d, ",dicePtr[i]);
    }
    printf("\nThe sum of your five largest dice is %d points.\n",  scoreArray[14]);
}

/* Function for the yatzy round */
void playYatzy(int dice, int *dicePtr, int *scoreArray) {
    /* Rolling the dice */
    rollMultipleDies(dice, dicePtr);

    int diceWithNEyes[] = {0,0,0,0,0,0};
    dicePrEye(dice, dicePtr, diceWithNEyes);

    int i = 6, yatzyNumber = 0;

    while (i > 0 && yatzyNumber == 0) {
        if (diceWithNEyes[i - 1] >= 5) {
            yatzyNumber = i;
            scoreArray[15] = 50;
        }
        i--;
    }

    printf("\nYour rolled dice were: ");
    for (int i = 0; i < dice; i++) {
        printf("%d, ",dicePtr[i]);
    }
    if (yatzyNumber != 0) {
        printf("\nYou got yatzy with the number %d and recieved 50 points.\n", yatzyNumber);
    }
    else {
        printf("Sadly, you didnt get yatzy.\n");
    }
}

void printScore(int *scoreArray){
    /* Calculate the total score */
    int totalScore = 0;
    for (int i = 0; i < 16; i++) {
        totalScore += scoreArray[i];
    }
    /* Printing the results in a table */ 
    printf("\n");
    printf("*-------Yatzy--Results-------*\n");
    printf("*                            *\n");
    printf("*-------Upper--Section-------*\n");
    printf("-  Ones:             %d\n", scoreArray[0]);
    printf("-  Twos:             %d\n", scoreArray[1]);
    printf("-  Threes:           %d\n", scoreArray[2]);
    printf("-  Fours:            %d\n", scoreArray[3]);
    printf("-  Fives:            %d\n", scoreArray[4]);
    printf("-  Sixes:            %d\n", scoreArray[5]);
    printf("*--------Upper--Total--------*\n");
    printf("-  Total:            %d\n", scoreArray[0] + scoreArray[1] + scoreArray[2] + scoreArray[3] + scoreArray[4] + scoreArray[5]);
    printf("*-------Lower--Section-------*\n");
    printf("-  Bonus:            %d\n", scoreArray[6]);
    printf("-  One Pair:         %d\n", scoreArray[7]);
    printf("-  Two pairs:        %d\n", scoreArray[8]);
    printf("-  Three of a kind:  %d\n", scoreArray[9]);
    printf("-  Four of a kind:   %d\n", scoreArray[10]);
    printf("-  Small straight:   %d\n", scoreArray[11]);
    printf("-  Large straight:   %d\n", scoreArray[12]);
    printf("-  Full house:       %d\n", scoreArray[13]);
    printf("-  Chance:           %d\n", scoreArray[14]);
    printf("-  Yatzy:            %d\n", scoreArray[15]);
    printf("*--------Total--Score--------*\n");
    printf("-  Total:            %d\n", totalScore);
}