#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// plan: first give cards to robot/human, then decide what to do in turns, then deck/other stuff.
// 0-9 = card. 10 = reverse 11=skip 12=+2 13=+4 14=change color. 1=red 2=yellow 3=green 4=blue.
int giveCard();
int giveColor(int cards[100], int card); // cards[100] are all the cards. card is the amount of cards.
int givefirstcards(); 
int interpreter(int ucard[100], int ucolor[100], int cardamonut, int num); // this will be fun. 
int fix(int ucard[100], int ucolor[100]); // i don't know why this exists.
// REWRITE THE WHOLE THING AGAIN. CAN'T RUN THE SAME FUNCTION AGAIN AND AGAIN.
void debuggingshit(){
    // not for use in main program.
    FILE *debugoutput = fopen("test.txt", "a");
    int test[100];
    int test1[100];
    for(int i = 0; i < 100; i++){
    test[i] = giveCard();
    test1[i] = giveColor(test, i);
    //fprintf(debugoutput, "%d %d\n", test, test1); THIS IS FOR THE PREVIOUS VERSION OF THIS

    // new ver below; this shows the actual cards to test the interpreter:
    /*char test2[100] = {};
    test2 = interpreter(test[i], test1[i], i); 
    fprintf(debugoutput, "%s\n", test2); this is going to be a mess to rewrite because its printf in the interpreter ughhhhh*/
}   
    
    fclose(debugoutput);
}
int compplay(int size);
// size is the amount of cards in total.

int compTurn(int card, int color, int comp_size, int cards[100], int colors[100], int play);
// card = current card. color = current color. comp_size = the amount of cards that are in the computer's deck. cards[100] = all cards. colors[100] = all colors. int play = the turn you will play.

int checkplayerTurn();
//function of this (lol) is to make sure that the player gives a valid card.

int main(){
    srand(time(NULL));
    int user_card[100] = {0};
    int comp_card[100] = {0};
    int user_color[100] = {0};
    int comp_color[100] = {0};
    int user_size = 0;
    int comp_size = 0;

    for(int i = 0; i < 7; i++){ 
        user_card[i] = giveCard();
        user_color[i] = giveColor(user_card, user_size);
        user_size++;
        comp_card[i] = giveCard();
        comp_color[i] = giveColor(comp_card, comp_size);
        comp_size++;
    }
    /*user_size--;
    comp_size--;*/

    int turnNum = 0; // 0 = no. of turns have been played. since first turn is computer, odd number = turn of player, even num = turn of computer.
    // this will be used later when we want to see if the card it gives can be played or not. below variables will also be used for the aforementioned thing.

    int currentNum;
    int currentCol;

    interpreter(user_card, user_color, user_size, 1000);
    int turn = compplay(comp_size);
    //printf("The computer played this card: ");
    printf("%d %d\n", user_size, comp_size);
    printf("%d", user_color[user_size - 1]);
    turnNum++;
    currentNum = comp_card[turn];
    currentCol = comp_color[turn];
    interpreter(user_card, user_color, user_size, 6);
    //interpreter(comp_card, comp_color, comp_size, turn); // originally, i was going to put this in a function. changed my mind when i realized that its only the first turn that
    // it doesnt matter what you do. REAL GAME STARTS NOW!



}

int compTurn(int card, int color, int comp_size, int cards[100], int colors[100], int play){
    if(cards[play] == 13 || cards[play] == 14){
        return play; // 
    }

    if(color != colors[play] && card != cards[play]){
        return 1000;
    }

    return play;
}

int compplay(int size){
    return rand() % (size - 0 + 1) + 0;
}

int giveCard(){
    return rand() % (14 - 0 + 1) + 0;
}

int giveColor(int cards[100], int card){
    if(cards[card] == 14 || cards[card] == 13){
        return 0;
    }
    
    else{
        return rand() % (4 - 1 + 1) + 1;
    }
    
}

int interpreter(int ucard[100], int ucolor[100], int cardamount, int num){
    if(num != 1000){
        switch(ucolor[num]){
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
            printf("A ");
            break;
        }

        switch(ucard[num]){
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
            printf(" reverse\n");
            break;

            case 11:
            printf(" skip turn\n");
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

    else{
    printf("You have these cards:\n");
    for(int i = 0; i < cardamount; i++){
        switch(ucolor[i]){
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

        switch(ucard[i]){
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
            printf(" reverse\n");
            break;

            case 11:
            printf(" skip turn\n");
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
}
