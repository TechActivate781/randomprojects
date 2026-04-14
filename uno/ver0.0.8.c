#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int giveCard(); // will give a random card to someone
int giveColor(int card); // will give a color to the card
void interpreter(int cards[100][2], int cardamount  ); // this will show the user all of his/her/their cards
int checkTurn(int current[2], int whatYouEnter[2]); //this will make sure that the turn you do is correct.
void processSkip(int whomad, int turn); // this will just change the turn for who it is skipped to
void processReverse(int whomad, int turn); // this is the same as the above function as in a 1v1, a reverse is basically a skip
void processPlus(int whomad, int cards[100][2], int amonut, int amounts[2]); //amount = 4 = plus 4, etc.

void testingstuff(){
    FILE *test = fopen("meow.txt", "a");
    int meow;
    int meow1[100][2];
    for(int i = 0; i < 100; i++){
        meow1[i][0] = giveCard();
        meow1[i][1] = giveColor(meow1[i][0]);
        //fprintf(test, "%s", interpreter(meow, 100)); //hardcodign since its only for testing. UGH
    }
    interpreter(meow1, 7);
    fclose(test);
}

int main(){
    srand(time(NULL));

    int comCards[100][2] = {{0}, {0}};
    int playerCards[100][2] = {{0}, {0}}; // first column is for card, 2nd is for color
    int amounts[2] = {0, 0}; // this is amount of cards the user/computer have; 1 is for computer, 0 is for user
    int turn = 0; // this assigns who's turn it is. since the computer plays first, its now the user's turn. 1 = comp, 0 = user
    int currentCard[2] = {0}; // the first value is the card, then the color
    int changeColor; //this will hold the value ofr the color that you're changing the color to

    testingstuff();

    printf("Welcome!!\nYou have been given 7 cards to start with. They are: ");
}

void processPlus(int whomad, int cards[100][2], int amonut, int amounts[2]){ //work in progress, will get the main menu going rn.
    if(whomad == 1){
        printf("You've been given %d extra cards. They are:\n", amonut);

    }
}

void processReverse(int whomad, int turn){
    if(whomad == 1){
        printf("You have reversed the turns! It is the computer's turn again!\n");
        turn = 1;
    }

    if(whomad == 0){
        printf("You have reversed the turns! It is your turn again!");
        turn = 0;
    }
}

void processSkip(int whomad, int turn){
    //whomad = 1 = the ocmputer threw the skip turn
    if(whomad == 1){
        printf("Your turn has been skipped!\n");
        turn = 1;
    }

    if(whomad == 0){
        printf("You have skipped the computer's turn! It is your turn again!");
        turn = 0;
    }
}

int checkTurn(int current[2], int whatYouEnter[2]){
    //we'll return a 0 here if its okie, and return a 1 if its not okie
    if(current[1] == whatYouEnter[1]){
        return 0;
    }

    if(current[2] == whatYouEnter[2]){
        return 0;
    }

    if(whatYouEnter[1] == 13 || whatYouEnter[2] == 14){
        return 0;
    } // i think this is all that matters?
}

int giveCard(){
    return rand() % (14 - 0 + 1) + 0; // 10 = skip, 11 = reverse, 12 = plus2, 13 = plus4, 14=changecolor
}

int giveColor(int card){
    int test = rand() % (4 - 1 + 1) + 1;
    
    if(card == 13 || card == 14){
        return 0; // 0 is black. otherwise its in roygbiv in terms of what the colors are.
    }

    return test;
}

void interpreter(int cards[100][2], int cardamount){
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
