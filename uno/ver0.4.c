#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// TO DO: add support for playing two/more cards at once

int giveCard(); // will give a random card to someone
int giveColor(int card); // will give a color to the card
void interpreter(int cards[100][2], int cardamount, int card); // this will show the user all of his/her/their cards
int checkTurn(int current[2], int whatYouEnter[2]); //this will make sure that the turn you do is correct.
void processSkip(int whomad, int turn); // this will just change the turn for who it is skipped to
void processReverse(int whomad, int turn); // this is the same as the above function as in a 1v1, a reverse is basically a skip
void processPlus(int whomad, int cards[100][2], int amount, int amounts[2]); //amount = 4 = plus 4, etc.
void processChangeColor(int whomad, int currentCard[2], int amounts[2], int cards[100][2]); //will just change the value in currentCard
void interpretColor(int color); // this is just for changeColor.
int chooseCompTurn(int current[2], int cards[100][2], int amount);
void GiveNewCard(int cards[100][2], int amount, int whomad); // this is only going to give a card if there aren't any playable cards. does this even need its own function? idk
void FixCards(int cards[100][2], int amount); /*currently, the code has an issue:
if i play lets say the card at cards[5], there's an empty space. if i just remove one from amount, then the card at cards[7] isn't interpreted and there's an empty space
this function will fix that by findnig whenever cards[i][1] or [0] has 1000 which i'll run after every turn + put 1000 when i've used a card*/

void testingstuff(){
    FILE *test = fopen("meow.txt", "a");
    int meow;
    int meow1[100][2];
    for(int i = 0; i < 100; i++){
        meow1[i][0] = giveCard();
        meow1[i][1] = giveColor(meow1[i][0]);
        //fprintf(test, "%s", interpreter(meow, 100)); //hardcodign since its only for testing. UGH
    }
    interpreter(meow1, 7, 1000);
    fclose(test);
}

int main(){
    srand(time(NULL));

    int comCards[100][2] = {{0}, {0}};
    int playerCards[100][2] = {{0}, {0}}; // first column is for card, 2nd is for color
    int amounts[2] = {0, 0}; // this is amount of cards the user/computer have; 1 is for computer, 0 is for user. harcoding the start cards to fix a bug
    int turn = 0; // this assigns who's turn it is. since the computer plays first, its now the user's turn. 1 = comp, 0 = user
    int currentCard[2] = {0}; // the first value is the card, then the color
    int changeColor; //this will hold the value ofr the color that you're changing the color to
    int numOfCards[15][2] = {0}; // this will hold the amount of each card that the user/player has (index 0 = player). will just manually update it each time
    int colOfCards[15][2] = {0}; // same but for color. return val from that here.

    // testingstuff();

    for(int i = 0; i < 7; i++){
        comCards[i][0] = giveCard();
        amounts[1]++;
        comCards[i][1] = giveColor(comCards[i][0]);
        playerCards[i][0] = giveCard();
        amounts[0]++;
        playerCards[i][1] = giveColor(playerCards[i][0]);
    }

    // change of mind. will rewrite this initial bit.
    //printf("Welcome!!\nYou have been given 7 cards to start with. They are: ");
    //interpreter(playerCards, amounts[0], 1000);
    printf("\n\n");
    interpreter(comCards, amounts[1], 1000);
    processPlus(0, comCards, 2, amounts);
    interpreter(comCards, amounts[1], 1000);
    printf("\n\n");
    printf("%d\n", amounts[1]);

}

void FixCards(int cards[100][2], int amount){
    int actualNum;
    for(int i = 0; i < amount; i++){
        if(cards[i][0] != 1000){
            actualNum++; // this will now find the actual number of cards
        }

        if(cards[i][0] == 1000){
            for(int j = i; j < amount; j++){
                cards[j][0] = cards[j+1][1];
                cards[j][1] = cards[j+1][1];
            }
        }
    }

    amount = actualNum; // update the number of cards to be correct


}

void GiveNewCard(int cards[100][2], int amount, int whomad){
    if(whomad == 0){
        printf("You have no cards that you could play, so this card was given to you: ");
        cards[amount + 1][0] = giveCard();
        cards[amount + 1][1] = giveColor(cards[amount + 1][0]);
        amount++;
        interpreter(cards, amount, amount);
    }

    if(whomad == 1){
        cards[amount + 1][0] = giveCard();
        cards[amount + 1][1] = giveColor(cards[amount + 1][0]);
        amount++;
    } //technically here i could've just put those 3 lines and then in whomad==0 done printf/interpreter but oh well    
}

int chooseCompTurn(int current[2], int cards[100][2], int amount){ //moving this down because for some reason the repeating functions cmoe here.
    // if the user put a plus 2 or plus 4 AND there's a plus 2 / 4 in backup, i'll use that. yes its not in the rules but i've always played it like that sooo
    // if there are 2 cards that you cna play, do that. will add support for that later
    // otherwise just rand it? i think i had a fairly decent one in one of my earlier versions  
    // changed this to int so that i can just return the card number that the computer will use. will do the rest in main()
    // IDEA: make it a whole array instead. this could also be held in main() so i can add/remove? nah. gonna be difficult changing it :3    
    
    for(int i = 0; i < amount; i++){
        if(cards[i][0] == 12 || cards[i][0] == 13 && current [0] == 13 || current [0] == 12){
            return i; // idea here is that if the user played a +2/+4, we play it. hopefully this won't mess up lol
        }

        int UseGoodCard = rand() % (3 - 1 + 1) + 1;
        if(UseGoodCard == 2 && cards[i][0] == 14 || cards[i][0] == 11 || cards[i][0] == 10){
            return i;
        }

        if(cards[i][0] == current[0]){
            return i;
        }

        if(cards[i][1] == current[1]){
            return i;
        }

        
    }

    while(cards[amount][0] != 12 || cards[amount][0] != 13 || cards[amount][0] != 14 || cards[amount][0] != current[0] || cards[amount][1] != current[1]){
        GiveNewCard(cards, amount, 1); // i think this will work???
    }
    
}

void processChangeColor(int whomad, int currentCard[2], int amounts[2], int cards[100][2]){
    int color; // same roygbiv order as before; 1=red, etc
    if(whomad == 0){
        do{
            printf("What color do you want to change the color to?\nType 1 for red, 2 for yellow, 3 for green, and 4 for blue\n");
            scanf("%d", &color);
            if(color != 1 || color != 2 || color != 3 || color !=4){
                //there's 100% a faster/better way to do the above line, i just don't want to ponder about it? idk lol.
                printf("Please enter a valid color\n");
            }
        } while(color > 4 && color < 1);
        //JUST REALIZED THAT YOU HAVE TO CHOOSE A COLOR FOR A PLUS 4. FIX THIS. will call this from processPlus

        currentCard[1] = color;
        currentCard[0] = 1000;
        /*printf("Choose your next card. You have the following cards:\n");
        inetpreter(cards, amounts[0], 1000); */
        //this is so much more complicated than i thought... ngl. i could just take this back to main function and ask the user to put a card there? yeah.

    }

    if(whomad == 1){
        color = rand() % (4 - 1 + 1) + 1;
        currentCard[1] = color;
        interpretColor(currentCard[1]);
        currentCard[0] = 1000; //this is temp. add the next card for the computer once i finish the function for it.
    }
}

void processPlus(int whomad, int cards[100][2], int amount, int amounts[2]){ 
    if(whomad == 1){
        printf("You've been given %d extra cards as the computer used a Plus %d. They are:\n", amount, amount);

        for(int i = 0; i < amount; i++){
            cards[amounts[0]][0] = giveCard(); 
            cards[amounts[0]][1] = giveColor(cards[amounts[0]][0]);
            interpreter(cards, amounts[0], amounts[0]);
            amounts[0]++;
        }
    }

    if(whomad == 0){
        printf("You've given a Plus %d to the computer!!\n", amount);
        for(int i = 0; i < amount; i++){
            cards[amounts[1]][0] = giveCard(); 
            cards[amounts[1]][1] = giveColor(cards[amounts[1]][0]);
            amounts[1]++;
        } //hopefully this works, lol.
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

//moving this here
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
        system("pause");
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
