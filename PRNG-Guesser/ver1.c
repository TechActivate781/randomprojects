// note: for best usage, requires app to be called stuff.exe
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

// ver0.1

int main(){
    srand(time(NULL));
    int randNum = (rand() % 100) + 1; // number between 1 and 100
    system("cls"); // removing junk
    printf("Guess the number!\n");
    int guess;
    int guesses = 0;
    do{
        scanf("%d", &guess);
        guesses++;
        //if(guess == randNum){
            //printf("You guessed the number!\n");
            // was gonna use break; but its do while so no real need. actually hold up lol its anyways breaking out of the loop so this isnt even needed lmao
        //}

        if(guess > randNum){
            printf("Too high. Guess the number again\n");
        }

        if(guess < randNum){
            printf("Too low. Guess the number again\n");
        }
    }while(guess != randNum);

    printf("You guessed the number (took you %d guess (s))\nWould you like to try again?\nType 1 for Y; type N for no\n", guesses);
    char option; // was gonna use bool but forget its thingy after the % and i just forget the name of that too
    getchar(); // to be safe
    scanf("%c", &option);
    if(option == 'Y'){
        system("stuff.exe"); //file is called stuff.exe so it will restart the app. dirty way to do it lol. later version will use loop
    }

    else if(option == 'N'){
        system("pause");
        return 0;
    }

    else{
        printf("Unkown input; exiting\n");
        system("pause");
        return 0;
    }


}



