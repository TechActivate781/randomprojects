#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// this is probably shit code lol

int giveCards(int intforcards[100], int colora[100]){
    int num = 7;
    // +2 +4 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, change color, reverse, skip
    srand(time(NULL));
    // 0=0. 1=1. 2=2. 3=3. 4=4. 5=5. 6=6. 7=7. 8=8. 9=9. 10=change. 11=rev. 12=skip 13=+2 14=+4
    int randnumber;
    for(int i = 0; i < 7; i++){
        randnumber = rand() % (14-0+1);
        if(randnumber == 10 || randnumber == 14){
            randnumber = rand() % (14-0) + 0; // to decrease chances of +4 or change color as there are fewer of them
        }
        intforcards[i] = randnumber;
        int color;
        color = (rand() % 4) + 1; // gives all cards a color
        colora[i] = color;
    }

    for(int i = 0; i < 7; i++){
        if(intforcards[i] == 10 || intforcards[i] == 14){
            intforcards[i] = 0; // change color cards or +4 ones don't have a color. no color = black 
        }
    }

    return 7;
}

void interpreter(int cards[100], int color[100], int size){
    char colors[100][10];
    char cardds[100][10];
    for(int i = 0; i < size; i++){
        switch(color[i]){
            case 0:
            strcpy(colors[i], "Black");
            break;

            case 1:
            strcpy(colors[i], "Blue");
            break;

            case 2:
            strcpy(colors[i], "Green");
            break;

            case 3:
            strcpy(colors[i], "Yellow");
            break;

            case 4:
            strcpy(colors[i], "Red");
            break;
        }
    }

    for(int i = 0; i < size; i++){
        switch(cards[i]){
            case 0:
            strcpy(cardds[i], "Zero");
            break;

            case 1:
            strcpy(cardds[i], "One");
            break;

            case 2:
            strcpy(cardds[i], "Two");
            break;

            case 3:
            strcpy(cardds[i], "Three");
            break;

            case 4:
            strcpy(cardds[i], "Four");
            break;

            case 5:
            strcpy(cardds[i], "Five");
            break;

            case 6:
            strcpy(cardds[i], "Six");
            break;

            case 7:
            strcpy(cardds[i], "Seven");
            break;

            case 8:
            strcpy(cardds[i], "Eight");
            break;

            case 9:
            strcpy(cardds[i], "Nine");
            break;

            case 10:
            strcpy(cardds[i], "Change color");
            break;

            case 11:
            strcpy(cardds[i], "Reverse");
            break;

            case 12:
            strcpy(cardds[i], "Skip turn");
            break;

            case 13:
            strcpy(cardds[i], "Draw two"); // i think its called this but i usually call it +2
            break;

            case 14:
            strcpy(cardds[i], "Draw four");
            break;
        }
    }

    printf("You have %d cards. They are:\n", size);
    for(int i = 0; i < size; i++){
        printf("A %s %s\n", colors[i], cardds[i]);
    }
}

int main(){
    int cards[100] = {0}; // fuck you if you have > 100 cards + only 400 bytes soo....
    int color[100] = {0};
    int acards = giveCards(cards, color);
    for(int i = 0; i < acards; i++){
        printf("%d %d\n", cards[i], color[i]);
    }
    int size = sizeof(cards) / 4;
    interpreter(cards, color, acards);
}
