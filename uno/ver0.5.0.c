#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>
#include <stdbool.h>

void Logging(FILE* path, int PlayerCards[100][2], int ComCards[100][2], int Amounts[2], int CurrentCard[2], int Turn, int TurnNum);

int GiveCard(); // will give a random card to someone
int GiveColor(int Card); // will give a color to the card
void Interpreter(int Cards[100][2], int CardAmount, int Card, int ShowCard, int CurrentCard[2]); // this will show the user all of his/her/their cards
void InterpretColor(int Color); // this is for just printing a color - use for a change color

void ProcessSkip(int WhoMad, int Turn);
void ProcessReverse(int WhoMad, int Turn); //this is bascially the same as above
void ProcessPlusTwo(int WhoMad, int Cards[100][2], int Amounts[2]);
void ProcessPlusFour(int WhoMad, int CurrentCard[2], int Cards[100][2], int Amounts[2], int OpponentCards[100][2]);
void ProcessChangeColor(int WhoMad, int CurrentCard[2], int Cards[100][2], int Amounts[2]);

void MiddlePerson(int Cop, int PlayerCards[100][2], int ComCards[100][2], int Amounts[2], int CurrentCard[2], int ChosenCards[2], int Turn); // idea here is that when a number is chosen it'll go through here before calling the current function??? everything will have to be passed ig
// cop is computer or player - if this is 1, that means that we're doing computer stuff, otherwise player
int FixCards(int Cards[100][2], int Amount);

int CheckIfCardIsRight(int CurrentCard[2], int ChosenCards[2]);
int ChooseComputerTurn(int Cards[100][2], int Amounts[2], int CurrentCard[2], int IsItTheStart); //last int basically asking if its the start of the game; if so, it'll just give a random card



int main(){//this is like the tenth time i've redone this whole project.
    bool LogOrNot;
    printf("Logging does not collect any personal data. Should an unexpected move/bug occur, you may send me the log.txt file generated as an issue on the GitHub.\nWould you like to log the game to a file (1 or 0): ");
    int LogOrNotInInt = 100; // im actually embarassed it took 40 minutes for me to realize i have to define it outside
    
    while(LogOrNotInInt != 0 || LogOrNotInInt != 1){
        scanf("%d", &LogOrNotInInt);
        if(LogOrNotInInt == 0 || LogOrNotInInt == 1){
            break;
        }
        printf("You haven't written a correct input - try again: ");
    }

    if(LogOrNotInInt == 0){
        LogOrNot = false;
    }
    
    if(LogOrNotInInt == 1){
        LogOrNot = true;
    }
    
    /*printf("%d", LogOrNotInInt);
    
    if(LogOrNotInInt == true){
        printf("\n\n\ntesttest\n\n\n");
    }*/

    srand(time(NULL));

    int ComCards[100][2] = {{0}, {0}};
    int PlayerCards[100][2] = {{0}, {0}}; // first column is for card, 2nd is for color
    int Amounts[2] = {0, 0}; // this is amount of cards the user/computer have; 1 is for computer, 0 is for user. harcoding the start cards to fix a bug
    int Turn = 0; // this assigns who's turn it is. since the computer plays first, its now the user's turn. 1 = comp, 0 = user
    int CurrentCard[2] = {0}; // the first value is the card, then the color
    int ChangeColor; //this will hold the value ofr the color that you're changing the color to
    int ChosenCards[2] = {0, 0}; // moving this up so that we can use it earlier
    int TurnNum = 0;

    int Test;
    Test = 0;

    for(int i = 0; i < 7; i++){
        ComCards[i][0] = GiveCard();
        Amounts[1]++;
        ComCards[i][1] = GiveColor(ComCards[i][0]);
        PlayerCards[i][0] = GiveCard();
        Amounts[0]++;
        PlayerCards[i][1] = GiveColor(PlayerCards[i][0]);
    }
    
    //printf("test"); // temp to see where it crashes
    
    if(LogOrNot == true){ 
        FILE *Log = fopen("log.txt", "w"); 
        Logging(Log, PlayerCards, ComCards, Amounts, CurrentCard, Turn, TurnNum);
        fclose(Log);
    } // oh my days. it was failing because i was using one equal instead of two...
    
    int CardToPlay = ChooseComputerTurn(ComCards, Amounts, CurrentCard, 1);    
    CurrentCard[0] = ComCards[CardToPlay - 1][0];
    CurrentCard[1] = ComCards[CardToPlay - 1][1];
    Amounts[1]--; 
    ComCards[CardToPlay - 1][0] = 0;
    ComCards[CardToPlay - 1][1] = 0;
    FixCards(ComCards, Amounts[1]);
    TurnNum++;
    MiddlePerson(1, PlayerCards, ComCards, Amounts, CurrentCard, ChosenCards, Turn);
    if(LogOrNot == true){
        FILE *Log = fopen("log.txt", "a"); // open/close the file every time to fix it?
        Logging(Log, PlayerCards, ComCards, Amounts, CurrentCard, Turn, TurnNum);
        fclose(Log);
    }

    /*printf("%d\n", CardToPlay);
    Interpreter(ComCards, Amounts[1], 1000, 0, 0); // just a quick test
    */

    int TurnToPlay; // turn for player
    int IsCardRight = 0; // will be one once we see if the card is correct

    while(Amounts[0] != 0 || Amounts[1] != 0){
        Interpreter(PlayerCards, Amounts[0], 1000, 1, CurrentCard);
        printf("\nPlay a card - type the number: ");
        while(IsCardRight == 0){
            scanf("%d", &CardToPlay);
            CardToPlay--; // this is because the 1st card the user sees is the 0th card in the array
            ChosenCards[0] = PlayerCards[CardToPlay][0];
            ChosenCards[1] = PlayerCards[CardToPlay][1];
            IsCardRight = CheckIfCardIsRight(CurrentCard, ChosenCards);
            if(IsCardRight == 0){
                printf("\nYou are unable to play this card based on the current card - try again: ");
            }
        } 

        CurrentCard[0] = PlayerCards[CardToPlay][0];
        CurrentCard[1] = PlayerCards[CardToPlay][1];
        Turn = 1; // now teh computer's turn
        Amounts[0]--;
        PlayerCards[CardToPlay][0] = 0;
        PlayerCards[CardToPlay][1] = 0;
        FixCards(PlayerCards, Amounts[0]);

        printf("You've played this card: ");
        Interpreter(PlayerCards, Amounts[0], CardToPlay, 0, 0);
        TurnNum++;
        MiddlePerson(0, PlayerCards, ComCards, Amounts, CurrentCard, ChosenCards, Turn);
        if(LogOrNot == true){
            FILE *Log = fopen("log.txt", "a");
            Logging(Log, PlayerCards, ComCards, Amounts, CurrentCard, Turn, TurnNum);
            fclose(Log);
        }
        
        if(Amounts[0] == 0){
            break; // otherwise the computer would get to play after you've won.
            // not needed below as there it is the last thing in the loop
        }
        
        CardToPlay = ChooseComputerTurn(ComCards, Amounts, CurrentCard, 1);    
        CurrentCard[0] = ComCards[CardToPlay - 1][0];
        CurrentCard[1] = ComCards[CardToPlay - 1][1];
        Amounts[1]--; 
        ComCards[CardToPlay - 1][0] = 0;
        ComCards[CardToPlay - 1][1] = 0;
        FixCards(ComCards, Amounts[1]);
        TurnNum++;
        MiddlePerson(1, PlayerCards, ComCards, Amounts, CurrentCard, ChosenCards, Turn);
        if(LogOrNot == true){
            FILE *Log = fopen("log.txt", "a");
            Logging(Log, PlayerCards, ComCards, Amounts, CurrentCard, Turn, TurnNum);
            fclose(Log);
        }
    }

    if(Amounts[0] == 0){
        printf("Congratulations! You won!!\n");
    }

    if(Amounts[1] == 0){
        printf("The computer has won\n");
    }

    return 0;
}

void Logging(FILE* path, int PlayerCards[100][2], int ComCards[100][2], int Amounts[2], int CurrentCard[2], int Turn, int TurnNum){
    fprintf(path, "Turn number: %d:\n", TurnNum);
    fprintf(path, "PlayerCards dump: ");
    for(int i = 0; i < Amounts[0]; i++){
        fprintf(path, "%d %d   ", PlayerCards[i][0], PlayerCards[i][1]); // good enough (not)
    }
    fprintf(path, "\nComCards dump: ");
    for(int i = 0; i < Amounts[1]; i++){
        fprintf(path, "%d %d   ", ComCards[i][0], ComCards[i][1]);
    }
    fprintf(path, "\nAmounts: %d %d", Amounts[0], Amounts[1]);

    fprintf(path, "\nCurrentCard: %d %d", CurrentCard[0], CurrentCard[1]);
    fprintf(path, "\nTurn: %d", Turn);
    fprintf(path, "\nTurnNum: %d\n", TurnNum);

    fprintf(path, "\n\n"); // just to make the ends a bit clearer.
    
}

void MiddlePerson(int Cop, int PlayerCards[100][2], int ComCards[100][2], int Amounts[2], int CurrentCard[2], int ChosenCards[2], int Turn){

    if(Cop == 0){
        // this is it's the player's turn we're processing. we've already checked that the card is right prior to calling this func

        switch(ChosenCards[0]){
            case 10:
            ProcessSkip(0, Turn);
            break;

            case 11:
            ProcessReverse(0, Turn);
            break;

            case 12:
            ProcessPlusTwo(0, ComCards, Amounts);
            break;

            case 13:
            ProcessPlusFour(0, CurrentCard, ComCards, Amounts, PlayerCards);
            break;

            case 14:
            ProcessChangeColor(0, CurrentCard, PlayerCards, Amounts);
            break;

            default:
            break;
        } // this is for the good cards. for other ones.. no real need to call this function

        if(Cop == 1){
            switch(ChosenCards[0]){
            case 10:
            ProcessSkip(1, Turn);
            break;

            case 11:
            ProcessReverse(1, Turn);
            break;

            case 12:
            ProcessPlusTwo(1, PlayerCards, Amounts);
            break;

            case 13:
            ProcessPlusFour(1, CurrentCard, PlayerCards, Amounts, ComCards);
            break;

            case 14:
            ProcessChangeColor(1, CurrentCard, ComCards, Amounts);
            break;

            default:
            break; // wonder if this is even needed
        }
        }

    }
}

int FixCards(int Cards[100][2], int Amount){
    int test = 0;
    int test2;
    int test3 = 0;
    int counter = 0;
    
    do{
        for(int i = 0; i <= Amount; i++){
            if(Cards[i][0] == 0 && Cards[i][1] == 0){
                test = i+1;
                Cards[i][0] = Cards[test][0];
                Cards[i][1] = Cards[test][1];
                for(int j = 0; j <= Amount - i; j++){
                    test2 = test + 1;
                    Cards[test][0] = Cards[test2][0];
                    Cards[test][1] = Cards[test2][1];
                    test2++;
                    test++;
                }
                
                Cards[Amount][1] = 0;
                Cards[Amount][0] = 0; // what have i just done.
            }
        }
        test3++; 
    } while(test3 != Amount); // probably the most ineffecient code i've ever made.
    
    for(int i = 0; i <= Amount; i++){
        if(Cards[i][0] == 0 && Cards[i][1] == 0){
            counter++;
        }
    }
    
    //counter-=3; // add one more because the last cards[amount][1]  was 0,0
    
    int final = Amount - counter + 1; // this is a weird fix but idk why.
    return final;
}


void ProcessPlusFour(int WhoMad, int CurrentCard[2], int Cards[100][2], int Amounts[2], int OpponentCards[100][2]){
    // this will sure be fun. i dont want this to be linked to main change color, so i'll have my own stuff here
    
    if(WhoMad == 0){
        printf("Four cards have been given to the computer\n");
        for(int i = 0; i < 4; i++){
            Cards[Amounts[1]][0] = GiveCard();
            Cards[Amounts[1]][1] = GiveColor(Cards[Amounts[1]][0]);
            Amounts[1]++;
        }
        
        ProcessChangeColor(0, CurrentCard, OpponentCards, Amounts);
        //passing othercards here as its the user's cards that matter here.
        
        // i don't think we have to do more here
    }
    
    
    if(WhoMad == 1){
        printf("You have been given four cards. They are:\n");
        
        for(int i = 0; i < 4; i++){
        Cards[Amounts[0]][0] = GiveCard(); 
        Cards[Amounts[0]][1] = GiveColor(Cards[Amounts[0]][0]);
        Amounts[0]++;
        
        Interpreter(Cards, Amounts[0], (Amounts[0]) -1, 0, 0);
    }

    int ComputerCardChosen = ChooseComputerTurn(OpponentCards, Amounts, CurrentCard, 0);
    CurrentCard[0] = OpponentCards[ComputerCardChosen][0];
    CurrentCard[1] = OpponentCards[ComputerCardChosen][1];

    printf("The computer played this card: ");
    Interpreter(OpponentCards, Amounts[1], ComputerCardChosen, 0, 0); // we'll just pass the card before removing it from the cmoputer *shrug*
    

    Amounts[1]--;
    OpponentCards[ComputerCardChosen][0] = 0;
    OpponentCards[ComputerCardChosen][1] = 0;  // that's it no?
    }
}


void ProcessPlusTwo(int WhoMad, int Cards[100][2], int Amounts[2]){
    if(WhoMad == 1){
        printf("You have been given two cards. They are:\n");
        for(int i = 0; i < 2; i++){
        Cards[Amounts[0]][0] = GiveCard(); // i think the issue here is the amuont[0] + 1
        Cards[Amounts[0]][1] = GiveColor(Cards[Amounts[0]][0]);
        Amounts[0]++;
        Interpreter(Cards, Amounts[0], (Amounts[0]) -1, 0, 0);
    }
    
    }
    
    if(WhoMad == 0){
        for(int i = 0; i < 2; i++){
        Cards[Amounts[1]][0] = GiveCard();
        Cards[Amounts[1]][1] = GiveColor(Cards[Amounts[1]][0]);
        Amounts[1]++;
    }
    }
}

void ProcessChangeColor(int WhoMad, int CurrentCard[2], int Cards[100][2], int Amounts[2]){
    int ColorToChangeTo;
    int CardToChoose;

    if(WhoMad == 0){
        //we'll basically find a suitable card, play that, change the color to it. easy
        // chooseComputerTurn(int cards[100][2], int amounts[2], int currentCard[2], int IsItTheStart)
        // pasting this here as i don't want to scroll down constantly
        
        int ComputerCardChange = ChooseComputerTurn(Cards, Amounts, CurrentCard, 0); //remember to remove change color from deck.
        //IS THIS A BUG THAT WE'RE DOING IT TO COMPUTERCHARDCHANGE INSTEAD OF CARDTOCHOOSE
        CurrentCard[0] = Cards[CardToChoose][0];
        CurrentCard[1] = Cards[CardToChoose][1];
        Amounts[1]--;
        Cards[CardToChoose][0] = 0;
        Cards[CardToChoose][1] = 0;
    } // much easier here because the computer won't put a wrong card, lol...

    if(WhoMad == 1){
        printf("Choose a color to change the color to\n");
        printf("Type 1 for red\nType 2 for yellow\nType 3 for green\nType 4 for blue\n");
        
        
        int JustForTest = 0; // idea is. if there's no card of the color the user changed it to, 
        // we'll keep thsi to zero and before the loop ends we'll tell them to do it again.

        while(1){
        scanf("%d", &ColorToChangeTo);
        //printf("test1 %d %d\n", colortochangeto, justfortest);
        if(ColorToChangeTo >= 5 || ColorToChangeTo <= 0){
            printf("You have entered an incorrect color. Please write a color again\n");
            continue; // this'll fix it.
        }

        for(int i = 0; i < Amounts[0]; i++){
                if(Cards[i][1] == ColorToChangeTo){
                    JustForTest = 1;
                    break;
                }
            }
            
            if(JustForTest == 0){
                printf("Choose a color again - you do not have a card of that color\n");
            }
            
            if(JustForTest == 1){
                //printf("quicktest\n");
                break;
            }

    }

    
    printf("Choose a card to play: ");
    Interpreter(Cards, Amounts[0], 1000, 0, 0);
    
    while(1){
        
    scanf("%d", &CardToChoose); 
    CardToChoose--; //since 1st card for user is 0th card in array
    
    if(Cards[CardToChoose][1] != ColorToChangeTo){
        printf("You chose an invalid card based on the color you chose - please choose again\n");
    }
    
    else{
        break;
    }
    
    // now that a card has been chosen, we must change the current card, amount of cards, num. of cards
    
    CurrentCard[0] = Cards[CardToChoose][0];
    CurrentCard[1] = Cards[CardToChoose][1];
    Amounts[0]--;
    Cards[CardToChoose][0] = 0;
    Cards[CardToChoose][1] = 0; // ig that's it - change color isn't being removed here as it may be needed
}
}
}

int ChooseComputerTurn(int Cards[100][2], int Amounts[2], int CurrentCard[2], int IsItTheStart){
    if(IsItTheStart == 1){
        // technically i can hardcode this as 7 because we know that there'll only be 7 cards at the start
        return rand() % (7 - 1 + 1) + 1;
    }

    // dont use amounts[0] lol
    while(1){
    for(int i = 0; i < Amounts[1]; i++){
        //randomly realized that with this method, if there's a plus 2 and you put a plus 2, it'd be two to you and two to PC rather than both to you.. hmm.. FIX THIS
        if(CurrentCard[0] == 12 && Cards[i][0] == 12){
            return i;
        }
        // i also just realized, these will all have to be in separate for functions because otherwise it'd always do the first possible thing rather than what you want, hm
        // i wonder if this could be done from switch..? or nested for?
    }

    for(int i = 0; i < Amounts[1]; i++){
        if(CurrentCard[0] == 12 && Cards[i][0] == 13){
            return i; // if the user put a +2, we're going with a +4.. uh...
        }
    }

    for(int i = 0; i < Amounts[i]; i++){
        if(CurrentCard[0] == 10 && Cards[i][0] == 10 || CurrentCard[0] == 11 && Cards[i][0] == 1){
            return i; // if the user put a plus/skip... ig we put that too??
        }
    }

    // the issue i'm realizing here is that this will be super easy to predict....

    for(int i = 0; i < Amounts[i]; i++){
        if(CurrentCard[0] == Cards[i][0] || CurrentCard[1] == Cards[i][1]){
            return i; 
        }
    }

    // if we're here, that means that the computer had no cards it could do - this means we keep on giving it new cards until we satisfy this.
    // but that means we shoulddo the whole thing in a loop...

    printf("The computer took a new card as it had no possible cards\n");
    Cards[Amounts[1]][0] = GiveCard();
    Cards[Amounts[1]][1] = GiveColor(Cards[Amounts[1]][0]); // i think this is fine.

}}

int CheckIfCardIsRight(int CurrentCard[2], int ChosenCards[2]){
    if(ChosenCards[0] == 14 || ChosenCards[0] == 13){
        return 1; // 1 will be returned if its a legal turn
    }
    
    if(CurrentCard[0] == ChosenCards[0] || CurrentCard[1] == ChosenCards[1]){
        return 1;
    }

    return 0; 
}

void ProcessSkip(int WhoMad, int Turn){
    if(WhoMad == 1){
        printf("Your turn has been skipped!\n");
        Turn = 1;
    }
    
    if(WhoMad == 0){
        printf("You have skipped the computers turn!\n");
        Turn = 0;
    }
}

void ProcessReverse(int WhoMad, int Turn){
    if(WhoMad == 1){
        printf("It is the computers turn as the computer chose to reverse it!\n");
        Turn = 1;
    }
    
    if(WhoMad == 0){
        printf("It is your turn as you chose to reverse it!\n");
        Turn = 0;
    }
}

int GiveCard(){
    int Card = rand() % (14 - 0 + 1) + 0; // 10 = skip, 11 = reverse, 12 = plus2, 13 = plus4, 14=changecolor

    // if the card is a +4 or a change color, there's a 50% chance that it'll regive a card

    if(Card == 13 || Card == 14 && (rand() % (2 - 1 + 1) + 1) == 2){
        Card = rand() % (14 - 0 + 1) + 0;
    }

    return Card;
}

int GiveColor(int Card){
    int Color = rand() % (4 - 1 + 1) + 1;
    
    if(Card == 13 || Card == 14){
        return 0; // 0 is black. otherwise its in roygbiv in terms of what the colors are (red=1, yellow=2, etc).
    }

    return Color;
}

// all the repetitive functions come down here lol.

void InterpretColor(int Color){
    switch(Color){
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

void Interpreter(int Cards[100][2], int CardAmount, int Card, int ShowCard, int CurrentCard[2]){

    if(Card == 1000){
        printf("You have these cards:\n");
    for(int i = 0; i < CardAmount; i++){
        switch(Cards[i][1]){
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

        switch(Cards[i][0]){
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
            switch(Cards[Card][1]){
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

        switch(Cards[Card][0]){
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
    
    if(ShowCard == 1){
        printf("Current card is: ");
        switch(CurrentCard[1]){
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
        
        switch(CurrentCard[0]){
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
