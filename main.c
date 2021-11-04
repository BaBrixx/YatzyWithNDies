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

#define DIE_MAX_EYES 6

/* Using an enumerator to keep track of the scores in the score array */
enum round {ones, twos, threes, fours, fives, sixes, bonus,
            onePair, twoPairs, threeKind, fourKind, smallStraight, 
            largeStraight, fullHouse, chance, yatzy};

void playYatzyGame();
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
    int programActive = 1;

    /* Seeding the rand function */
    srand(time(0));

    /* While loop to keep the program running until the user decides to stop */
    while (programActive == 1) {
        char tempContinue;

        playYatzyGame();

        /* Prompting the user if it wants to continue or not */
        printf("\nWould you like to continue? (y/n)\n");
        scanf(" %c", &tempContinue);
        if (tempContinue != 'y') {
            programActive = 0;
        }
    }
    return EXIT_SUCCESS;
}

/* Function to play a game of Yatzy */
void playYatzyGame() {
    int *dicePtr, *score, dice, corrDiceInput = 0;

    // Continues until a valid input 
    do {
        /* Acquiring the number of dice */
        printf("The number of dice: ");
        int inputs = scanf("%d", &dice);

        // Making sure that the user didnt input a newline (\n)
        char c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        
        corrDiceInput = (inputs == 1 && (dice >= 5));
    } while (!corrDiceInput);

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
    }
}

/* Function to roll and randomize the dice in a given array */
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

    /* For loop for playing the upper section for the individual dice */
    for (schFor = 1; schFor <= 6; schFor++) {
        rollMultipleDies(dice, dicePtr);

        /* Counting the number of the specific die */
        int currDice = 0;
        int diceFound = 0;
        while ((currDice) < dice && diceFound < 5) {
            if (dicePtr[currDice] == schFor) {
                diceFound++;
            }
            currDice++;
        }

        /* Printing the results for each round */
        printf("Your got %d dice with %d eyes and the rolled dice were: ", diceFound, schFor);
        for (int i = 0; i < dice; i++) {
            printf("%d, ",dicePtr[i]);
        }
        printf("\n");
        
        /* Adding score to the score array */
        scoreArray[schFor - 1] = diceFound * schFor;
        upperTotal += diceFound * schFor;
    }
    
    /* Checking if the user is eligeble for the bonus */
    if (upperTotal >= 63) {
        scoreArray[6] = 50;
        printf("You got %d and therefore got an extra %d points!\n", upperTotal, scoreArray[bonus]);
    }
    else {
        scoreArray[6] = 0;
        printf("You got %d and therefore got %d extra points!\n", upperTotal, scoreArray[bonus]);
    }
}

/* Function for the one pair round */
void playOnePair(int dice, int *dicePtr, int *scoreArray) {
    /* Rolling the dice */
    rollMultipleDies(dice, dicePtr);

    int diceWithNEyes[] = {0,0,0,0,0,0};
    dicePrEye(dice, dicePtr, diceWithNEyes);

    /* Could have used a while loop but this is easier to read and takes almost the same amount of lines */
    if (diceWithNEyes[5] >= 2) scoreArray[onePair] = 12;
    else if (diceWithNEyes[4] >= 2) scoreArray[onePair] = 10;
    else if (diceWithNEyes[3] >= 2) scoreArray[onePair] = 8;
    else if (diceWithNEyes[2] >= 2) scoreArray[onePair] = 6;
    else if (diceWithNEyes[1] >= 2) scoreArray[onePair] = 4;
    else if (diceWithNEyes[0] >= 2) scoreArray[onePair] = 2;
    else scoreArray[onePair] = 0;

    /* Printing the result */
    printf("\nYour rolled dice were: ");
    for (int i = 0; i < dice; i++) {
        printf("%d, ",dicePtr[i]);
    }
    printf("\nYou got a pair of %d and therefore got %d points.\n", scoreArray[onePair]/2, scoreArray[onePair]);
}

/* Function for the two pairs round */
void playTwoPairs(int dice, int *dicePtr, int *scoreArray) {
    /* Rolling the dice */
    rollMultipleDies(dice, dicePtr);

    int diceWithNEyes[] = {0,0,0,0,0,0};
    dicePrEye(dice, dicePtr, diceWithNEyes);

    /* Initiating temporary pair integers */
    int i = 6, firstPair = 0, secondPair = 0;

    /* While loop for checking for pairs */
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

    /* Awarding the user points two different pairs are present */
    if (firstPair != 0 && secondPair != 0) {
        scoreArray[twoPairs] = (2 * firstPair) + (2 * secondPair);
    }

    printf("\nYour rolled dice were: ");
    for (int i = 0; i < dice; i++) {
        printf("%d, ",dicePtr[i]);
    }
    printf("\nYou got a pair of %d and a pair of %d. You therefore got %d points.\n", firstPair, secondPair, scoreArray[twoPairs]);
}

/* Function for the three and four of a kind rounds */
void playOfAKind(int dice, int *dicePtr, int *scoreArray) {
    int threeWSameEyes = 0, fourWSameEyes = 0;

    /* For loop for playing the two "Of A Kind" rounds */
    for (int i = 3; i < 5; i++) {
        /* Rolling the dice */
        rollMultipleDies(dice, dicePtr);

        int diceWithNEyes[] = {0,0,0,0,0,0};
        dicePrEye(dice, dicePtr, diceWithNEyes);

        /* While loop for checking if number of dice with j eyes is larger than i (3 or 4) */
        int j = 6, diseFound = 0;
        while (j > 0 && !diseFound) {
            if (diceWithNEyes[j - 1] >= i){
                (i == 3) ? (scoreArray[threeKind] = 3 * j) : (scoreArray[fourKind] = 4 * j);
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
                                                                  , (i == 3) ? scoreArray[threeKind] / 3 : scoreArray[fourKind] / 3
                                                                  , (i == 3) ? scoreArray[threeKind] : scoreArray[fourKind]);
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
            (i == 1) ? (scoreArray[smallStraight] = 15) : (scoreArray[largeStraight] = 20);
        } 
        
        /* Printing the result */
        printf("\nYour rolled dice were: ");
        for (int j = 0; j < dice; j++) {
            printf("%d, ",dicePtr[j]);
        }
        printf("\nYou %s %s and therefore got %d points.\n", (dieMissing) ? "didnt get" : "got"
                                                           , (i == 1) ? "small straight" : "large straight"
                                                           , (i == 1) ? scoreArray[smallStraight] : scoreArray[largeStraight]);
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

    /* Printing the results and awarding the user points */
    printf("\nYour rolled dice were: ");
    for (int i = 0; i < dice; i++) {
        printf("%d, ",dicePtr[i]);
    }

    if (threeOfKind != 0 && twoOfKind != 0) {
        scoreArray[fullHouse] = (3 * threeOfKind) + (2 * twoOfKind);
        printf("\nYou got two %d and three %d therefore got %d points.\n", twoOfKind, threeOfKind, scoreArray[fullHouse]);
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

    /* For loop for finding the sum of the five largest dice */
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

    /* Awarding the user points and printing the result */
    scoreArray[chance] = tempSum;

    printf("\nYour rolled dice were: ");
    for (int i = 0; i < dice; i++) {
        printf("%d, ",dicePtr[i]);
    }
    printf("\nThe sum of your five largest dice is %d points.\n",  scoreArray[chance]);
}

/* Function for the yatzy round */
void playYatzy(int dice, int *dicePtr, int *scoreArray) {
    /* Rolling the dice */
    rollMultipleDies(dice, dicePtr);

    int diceWithNEyes[] = {0,0,0,0,0,0};
    dicePrEye(dice, dicePtr, diceWithNEyes);

    int i = 6, yatzyNumber = 0;

    /* While loop for checking if the user got yatzy and awarding points*/
    while (i > 0 && yatzyNumber == 0) {
        if (diceWithNEyes[i - 1] >= 5) {
            yatzyNumber = i;
            scoreArray[yatzy] = 50;
        }
        i--;
    }

    /* Printing the results */
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

/* Function for printing the results */
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
    printf("-  Ones:             %d\n", scoreArray[ones]);
    printf("-  Twos:             %d\n", scoreArray[twos]);
    printf("-  Threes:           %d\n", scoreArray[threes]);
    printf("-  Fours:            %d\n", scoreArray[fours]);
    printf("-  Fives:            %d\n", scoreArray[fives]);
    printf("-  Sixes:            %d\n", scoreArray[sixes]);
    printf("*--------Upper--Total--------*\n");
    printf("-  Total:            %d\n", scoreArray[ones] + scoreArray[twos] + scoreArray[threes] + 
                                        scoreArray[fours] + scoreArray[fives] + scoreArray[sixes]);
    printf("*-------Lower--Section-------*\n");
    printf("-  Bonus:            %d\n", scoreArray[bonus]);
    printf("-  One Pair:         %d\n", scoreArray[onePair]);
    printf("-  Two pairs:        %d\n", scoreArray[twoPairs]);
    printf("-  Three of a kind:  %d\n", scoreArray[threeKind]);
    printf("-  Four of a kind:   %d\n", scoreArray[fourKind]);
    printf("-  Small straight:   %d\n", scoreArray[smallStraight]);
    printf("-  Large straight:   %d\n", scoreArray[largeStraight]);
    printf("-  Full house:       %d\n", scoreArray[fullHouse]);
    printf("-  Chance:           %d\n", scoreArray[chance]);
    printf("-  Yatzy:            %d\n", scoreArray[yatzy]);
    printf("*--------Total--Score--------*\n");
    printf("-  Total:            %d\n", totalScore);
}