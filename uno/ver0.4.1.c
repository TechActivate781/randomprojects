#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


int giveCard(); // will give a random card to someone
int giveColor(int card); // will give a color to the card
void interpreter(int cards[100][2], int cardamount, int card); // this will show the user all of his/her/their cards
void interpretColor(int color); // this is for just printing a color - use for a change color

void processSkip(int whomad, int turn);
void processReverse(int whomad, int turn); //this is bascially the same as above
void processPlusTwo(int whomad, int cards[100][2], int amount[2]);
void processPlusFour(int whomad, int rightNowCards[2], int cards[100][2], int amount[2], int opponentcards[100][2]);
void processChangeColor(int whomad, int currentCard[2]);

void MiddleMan(); // idea here is that when a number is chosen it'll go through here before calling the current function??? everything will have to be passed ig

int CheckIfCardIsRight(int rightnowcards[2], int chosencards[2]);
int chooseComputerTurn(int cards[100][2], int amounts[2], int currentCard[2], int IsItTheStart); //last int basically asking if its the start of the game; if so, it'll just give a random card

int main(){//this is like the tenth time i've redone this whole project.
    srand(time(NULL));

    int comCards[100][2] = {{0}, {0}};
    int playerCards[100][2] = {{0}, {0}}; // first column is for card, 2nd is for color
    int amounts[2] = {0, 0}; // this is amount of cards the user/computer have; 1 is for computer, 0 is for user. harcoding the start cards to fix a bug
    int turn = 0; // this assigns who's turn it is. since the computer plays first, its now the user's turn. 1 = comp, 0 = user
    int currentCard[2] = {0}; // the first value is the card, then the color
    int changeColor; //this will hold the value ofr the color that you're changing the color to

    int test;
    test = 0;

    for(int i = 0; i < 7; i++){
        comCards[i][0] = giveCard();
        amounts[1]++;
        comCards[i][1] = giveColor(comCards[i][0]);
        playerCards[i][0] = giveCard();
        amounts[0]++;
        playerCards[i][1] = giveColor(playerCards[i][0]);
    }
    
    
    
}
void processPlusFour(int whomad, int rightNowCards[2], int cards[100][2], int amount[2], int othercards[100][2]){
    // this will sure be fun. i dont want this to be linked to main change color, so i'll have my own stuff here
    
    if(whomad == 1){
        printf("You have been given four cards. They are:\n");
        
        for(int i = 0; i < 4; i++){
        cards[amount[0]][0] = giveCard(); 
        cards[amount[0]][1] = giveColor(cards[amount[0]][0]);
        amount[0]++;
        
        interpreter(cards, amount[0], (amount[0]) -1);
        
        // i'll just make it give the first card that has a color. im too lazy
        // actually no. finish this once i've done the choosecard. i'll use that here. bruh
        int color; // variable for what we'll change the color to
        
        for(int i = 0; i < amount[1]; i++){
            if(othercards[i][1] == 1 || othercards[i][1] == 2 || othercards[i][1] == 3 || othercards[i][1] == 4){
                color = othercards[i][1];
                break;

            }
        }
    }
    }
}


void processPlusTwo(int whomad, int cards[100][2], int amount[2]){
    if(whomad == 1){
        printf("You have been given two cards. They are:\n");
        for(int i = 0; i < 2; i++){
        cards[amount[0]][0] = giveCard(); // i think the issue here is the amuont[0] + 1
        cards[amount[0]][1] = giveColor(cards[amount[0]][0]);
        amount[0]++;
        interpreter(cards, amount[0], (amount[0]) -1);
    }
    
    }
    
    if(whomad == 0){
        for(int i = 0; i < 2; i++){
        cards[amount[1]][0] = giveCard();
        cards[amount[1]][1] = giveColor(cards[amount[1]][0]);
        amount[1]++;
    }
    }
}

void processChangeColor(int whomad, int currentCard[2]){

}

int chooseComputerTurn(int cards[100][2], int amounts[2], int currentCard[2], int IsItTheStart){
    if(IsItTheStart == 1){
        // technically i can hardcode this as 7 because we know that there'll only be 7 cards at the start
        return rand() % (7 - 1 + 1) + 1;
    }

    // dont use amounts[0] lol
    while(1){
    for(int i = 0; i < amounts[1]; i++){
        //randomly realized that with this method, if there's a plus 2 and you put a plus 2, it'd be two to you and two to PC rather than both to you.. hmm.. FIX THIS
        if(currentCard[0] == 12 && cards[i][0] == 12){
            return i;
        }
        // i also just realized, these will all have to be in separate for functions because otherwise it'd always do the first possible thing rather than what you want, hm
        // i wonder if this could be done from switch..? or nested for?
    }

    for(int i = 0; i < amounts[1]; i++){
        if(currentCard[0] == 12 && cards[i][0] == 13){
            return i; // if the user put a +2, we're going with a +4.. uh...
        }
    }

    for(int i = 0; i < amounts[i]; i++){
        if(currentCard[0] == 10 && cards[i][0] == 10 || currentCard[0] == 11 && cards[i][0] == 1){
            return i; // if the user put a plus/skip... ig we put that too??
        }
    }

    // the issue i'm realizing here is that this will be super easy to predict....

    for(int i = 0; i < amounts[i]; i++){
        if(currentCard[0] == cards[i][0] || currentCard[1] == cards[i][1]){
            return i; 
        }
    }

    // if we're here, that means that the computer had no cards it could do - this means we keep on giving it new cards until we satisfy this.
    // but that means we shoulddo the whole thing in a loop...

    cards[amounts[1]][0] = giveCard();
    cards[amounts[1]][1] = giveColor(cards[amounts[1]][0]); // i think this is fine.

}}

int CheckIfCardIsRight(int rightnowcards[2], int chosencards[2]){
    if(chosencards[0] == 14 || chosencards[0] == 13){
        return 0; // 0 will be returned if its a legal turn
    }
    
    if(rightnowcards[0] == chosencards[0] || rightnowcards[1] == chosencards[1]){
        return 0;
    }

    return 1; 
}

void processSkip(int whomad, int turn){
    if(whomad == 1){
        printf("Your turn has been skipped!\n");
        turn = 1;
    }
    
    if(whomad == 0){
        printf("You have skipped the computers turn!\n");
        turn = 0;
    }
}

void processReverse(int whomad, int turn){
    if(whomad == 1){
        printf("It is the computers turn as the computer chose to reverse it!\n");
        turn = 1;
    }
    
    if(whomad == 0){
        printf("It is your turn as you chose to reverse it!\n");
        turn = 0;
    }
}

int giveCard(){
    return rand() % (14 - 0 + 1) + 0; // 10 = skip, 11 = reverse, 12 = plus2, 13 = plus4, 14=changecolor
}

int giveColor(int card){
    int test = rand() % (4 - 1 + 1) + 1;
    
    if(card == 13 || card == 14){
        return 0; // 0 is black. otherwise its in roygbiv in terms of what the colors are (red=1, yellow=2, etc).
    }

    return test;
}

// all the repetitive functions come down here lol.

void interpretColor(int color){
    switch(color){
        case 1:
        printf("The color has been changed to red\n"); //might change this to just red if i need it somewhere else/make another value that will be passed to here if i need to do just color.
        break;

        case 2:
        printf("The color has been changed to yellow\n");
        break;

        case 3:
        printf("The color has been changed to green\n");
        break;

        case 4:
        printf("The color has been changed to blue\n");
        break;

        default:
        printf("Something went wrong. Please report the cards you used to get here on the GitHub\n");
        printf("Exiting the app now.\n");
    }
}

void interpreter(int cards[100][2], int cardamount, int card){

    if(card == 1000){
        printf("You have these cards:\n");
    for(int i = 0; i < cardamount; i++){
        switch(cards[i][1]){
            case 0:
            printf("");
            break;

            case 1:
            printf("A red");
            break;

            case 2:
            printf("A yellow");
            break;

            case 3:
            printf("A green");
            break;

            case 4:
            printf("A blue");
            break;

            default:
            printf("");
            break;
        }

        switch(cards[i][0]){
            case 0:
            printf(" 0\n");
            break;

            case 1:
            printf(" 1\n");
            break;

            case 2:
            printf(" 2\n");
            break;

            case 3:
            printf(" 3\n");
            break;

            case 4:
            printf(" 4\n");
            break;

            case 5:
            printf(" 5\n");
            break;

            case 6:
            printf(" 6\n");
            break;

            case 7:
            printf(" 7\n");
            break;

            case 8:
            printf(" 8\n");
            break;

            case 9:
            printf(" 9\n");
            break;

            case 10:
            printf(" skip turn\n");
            break;

            case 11:
            printf(" reverse\n");
            break;

            case 12:
            printf(" plus two\n");
            break;

            case 13:
            printf("Plus four\n");
            break;

            case 14:
            printf("Change color (wild)\n");
            break;
        }
    }
    }

    else{
            switch(cards[card][1]){
            case 0:
            printf("");
            break;

            case 1:
            printf("A red");
            break;

            case 2:
            printf("A yellow");
            break;

            case 3:
            printf("A green");
            break;

            case 4:
            printf("A blue");
            break;

            default:
            printf("");
            break;
        }

        switch(cards[card][0]){
            case 0:
            printf(" 0\n");
            break;

            case 1:
            printf(" 1\n");
            break;

            case 2:
            printf(" 2\n");
            break;

            case 3:
            printf(" 3\n");
            break;

            case 4:
            printf(" 4\n");
            break;

            case 5:
            printf(" 5\n");
            break;

            case 6:
            printf(" 6\n");
            break;

            case 7:
            printf(" 7\n");
            break;

            case 8:
            printf(" 8\n");
            break;

            case 9:
            printf(" 9\n");
            break;

            case 10:
            printf(" skip turn\n");
            break;

            case 11:
            printf(" reverse\n");
            break;

            case 12:
            printf(" plus two\n");
            break;

            case 13:
            printf("Plus four\n");
            break;

            case 14:
            printf("Change color (wild)\n");
            break;
        }
    }
}
