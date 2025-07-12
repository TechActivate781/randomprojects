#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
//4:47:34
// lost this app so rebuilding it oh well
// messing with prototyping

int GetComputerTurn();
void ChooseWinner(int player, int computer);

int main(){
    system("cls");
    int isRunning = 1;
    while(isRunning == 1){
    srand(time(NULL));
    int player;
    int computerChoice = GetComputerTurn();
    printf("Welcome\nType 1 for rock, 2 for paper, and 3 for scissor\n");
    scanf("%d", &player);
    getchar();
    ChooseWinner(player, computerChoice);
    printf("Type Y to play again; type N to not play again\n");
    char choice;
    scanf("%c", &choice);
    if(choice == 'N' || choice == 'n'){
        isRunning = 0;
    }

    else if(choice == 'Y' || choice == 'y'){
        
    }

    else{
        printf("Invalid choice. Exiting\n");
        isRunning = 0;
    }
}

    return 0;
}

int GetComputerTurn(){
    int max = 3;
    int min = 1;
    return (rand() % (max - min + 1)) + min;
};

void ChooseWinner(int player, int computer){
    if(player > 3 || player < 1){
        printf("Invalid turn\nExiting\n");
        exit(0);
    }

    if(player == 1 && computer == 3){
        printf("You won! The computer chose scissor\n");
    }

    else if(player == 2 && computer == 1){
        printf("You won! The computer chose rock\n");
    }

    else if(player == 3 && computer == 2){
        printf("You won! The computer chose paper\n");
    }

    else if(computer == 1 && player == 3){
        printf("You lost! The computer chose rock\n");
    }

    else if(computer == 2 && player == 1){
        printf("You lost! The computer chose paper\n");
    }

    else if(computer == 3 && player == 2){
        printf("You lost! The computer chose scissor\n");
    }

    else if(computer == player){
        printf("The game has ended in a tie!\n");
    }

    
}
