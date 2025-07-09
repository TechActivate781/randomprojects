#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

/* ver0.2
changelog:
cleanup + better way of restarting app */

int main(){
    int isRunning = 1;
    while(isRunning == 1){
    srand(time(NULL));
    int randNum = (rand() % 100) + 1; // number between 1 and 100
    system("cls"); // removing junk
    printf("Guess the number!\n");
    int guess;
    int guesses = 0;
    do{
        scanf("%d", &guess);
        guesses++;

        if(guess > randNum){
            printf("Too high. Guess the number again\n");
        }

        if(guess < randNum){
            printf("Too low. Guess the number again\n");
        }
    }while(guess != randNum);

    printf("You guessed the number (took you %d guess (s))\nWould you like to try again?\nType 1 for Y; type N for no\n", guesses);
    char option;
    getchar(); // to be safe
    scanf("%c", &option);

    if(option == 'N'){
        system("pause");
        isRunning = 0;
    }

    else if(option == 'Y'){
        //not much to do 
    }

    else{
        printf("Unkown input; exiting\n");
        system("pause");
        isRunning = 0;
    }
    }
    return 0;
}



