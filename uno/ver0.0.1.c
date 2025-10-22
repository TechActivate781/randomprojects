#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

void giveCards(int intforcards[100], int color[100]){
    int num = 7;
    // +2 +4 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, change color, reverse, skip
    srand(time(NULL));
    // 0=0. 1=1. 2=2. 3=3. 4=4. 5=5. 6=6. 7=7. 8=8. 9=9. 10=change. 11=rev. 12=skip 13=+2 14=+4
    int randnumber;
    for(int i = 0; i < 7; i++){
        randnumber = rand() % (14-0) + 0;
        if(randnumber == 10 || randnumber == 11 || randnumber == 12 || randnumber == 13 || randnumber == 14){
            randnumber = rand() % (14-0) + 0; // to decrease chances of the rest
        }
        intforcards[i] = randnumber;
        if(intforcards[i] != 10 || intforcards[i] != 14){
            // continue later
        }
    }
}

int main(){
    int cards[100] = {0}; // fuck you if you have > 100 cards + only 400 bytes soo....
    int color[100] = {0};
    giveCards(cards, color);
    for(int i = 0; i < 7; i++){
        printf("%d ", cards[i]);
    }
}
