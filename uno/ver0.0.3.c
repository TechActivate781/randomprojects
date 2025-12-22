#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// plan: first give cards to robot/human, then decide what to do in turns, then deck/other stuff.
// 0-9 = card. 11 = reverse 12=skip 13=+2 14=+4 15=change color. 1=red 2=yellow 3=green 4=blue.
int giveCard();
int giveColor();
int givefirstcards();
int interpreter(int ucard[100], int ucolor[100], int cardamonut); // this will be fun.
int fix(int ucard[100], int ucolor[100]);
// REWRITE THE WHOLE THING AGAIN. CAN'T RUN THE SAME FUNCTION AGAIN AND AGAIN.
void debuggingshit(){
    // not for use in main program.
    FILE *debugoutput = fopen("test.txt", "a");
    for(int i = 0; i < 100; i++){
    fprintf(debugoutput, "%d %d\n", giveCard(), giveColor());
    }
    fclose(debugoutput);
}

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
        user_color[i] = giveColor();
        user_size++;
        comp_card[i] = giveCard();
        comp_color[i] = giveColor();
        comp_size++;
    }

    interpreter(user_card, user_color, user_size);

    /*for(int i = 0; i < 7; i++){
        printf("%d %d\n", user_card[i], user_color[i]);
        printf("%d %d\n", comp_card[i], comp_color[i]);
    } just to check.*/ 
}

int giveCard(){
    return rand() % (14 - 0 + 1) + 0;
}

int giveColor(){
    return rand() % (5 - 1 + 1) + 1;
}

int interpreter(int ucard[100], int ucolor[100], int cardamount){
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
            //uhh
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
