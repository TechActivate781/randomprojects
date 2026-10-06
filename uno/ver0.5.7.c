// todo:
// middleperson still doesn't quite work
// check plus4/wild a few times
// issue with plus4, cyurrentCard doesn't seem to upadte.
// move the change of turns to the middleperson.
// though this seems to be an issue w/ changecolor.
// WHEN DOING CHANGECOLOR CHECK IF FIXCARDS NEEDS TO BE CALLED SOMEWHERE.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>

// MAJOR CHANGE - USE STRUCTS.

void Logging(FILE* path, int PlayerCards[100][2], int ComCards[100][2], int Amounts[2], int CurrentCard[2], int Turn, int TurnNum);

int GiveCard(); // will give a random card to someone
int GiveColor(int Card); // will give a color to the card
void Interpreter(int Cards[100][2], int CardAmount, int Card, int ShowCard, int CurrentCard[2]); // this will show the user all of his/her/their cards
void InterpretColor(int Color); // this is for just printing a color - use for a change color

int ProcessSkip(int WhoMad, int Turn);
int ProcessReverse(int WhoMad, int Turn); //this is bascially the same as above. changed to int since i need the value to update.. lol
void ProcessPlusTwo(int WhoMad, int Cards[100][2], int Amounts[2]);
void ProcessPlusFour(int WhoMad, int CurrentCard[2], int Cards[100][2], int Amounts[2], int OpponentCards[100][2]);
void ProcessChangeColor(int WhoMad, int CurrentCard[2], int Cards[100][2], int Amounts[2]);

void MiddlePerson(int Cop, int PlayerCards[100][2], int ComCards[100][2], int Amounts[2], int CurrentCard[2], int ChosenCards[2], int *Turn); // idea here is that when a number is chosen it'll go through here before calling the current function??? everything will have to be passed ig
// cop is computer or player - if this is 1, that means that we're doing computer stuff, otherwise player
int FixCards(int Cards[100][2], int Amount);

int CheckIfCardIsRight(int CurrentCard[2], int ChosenCards[2]);
int ChooseComputerTurn(int Cards[100][2], int Amounts[2], int CurrentCard[2], int IsItTheStart, int ChangeColor); //last int basically asking if its the start of the game; if so, it'll just give a random card

typedef struct{
    int Cards[100][2];
    int Amount;
}Hand; // meh, this can be a lter thing. i wanna make a prawwper ai first.

int main() { //this is like the tenth time i've redone this whole project.
	char Version[10] = "0.5.7";
	bool LogOrNot;
	printf("Logging does not collect any personal data (other than the date/time). Should an unexpected move/bug occur, you may send me the log.txt file generated as an issue on the GitHub.\nWould you like to log the game to a file (1 or 0): ");
	int LogOrNotInInt = 100; // im actually embarassed it took 40 minutes for me to realize i have to define it outside

	while(LogOrNotInInt != 0 || LogOrNotInInt != 1) {
		scanf("%d", &LogOrNotInInt);
		if(LogOrNotInInt == 0 || LogOrNotInInt == 1) {
			break;
		}
		printf("You haven't written a correct input - try again: ");
	}

	if(LogOrNotInInt == 0) {
		LogOrNot = false;
	}

	if(LogOrNotInInt == 1) {
		LogOrNot = true;
	}

	LogOrNotInInt = 1000; // being reused for below bit

	if(LogOrNot == true){
		printf("Would you like to append to an existing log.txt file in the current directory, or would you like to create a new one and potentially overrite an existing log.txt\n");
		printf("Type 1 to overwrite, type 0 to keep the file: ");

		while(LogOrNotInInt != 1 && LogOrNotInInt != 0){
			scanf("%d", &LogOrNotInInt);
			if(LogOrNotInInt != 1 && LogOrNotInInt != 0){
				printf("You didn't write a possible option - try again: ");
			}
		}

		printf("\n");
	}

	if(LogOrNotInInt == 1){
		FILE *Log = fopen("log.txt", "w");
		fclose(Log); // just writing quickly if they want us to
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
	int *pTurn = &Turn; // this is to pass into middleperson
	int CurrentCard[2] = {0}; // the first value is the card, then the color
	int ChangeColor; //this will hold the value ofr the color that you're changing the color to
	int ChosenCards[2] = {0, 0}; // moving this up so that we can use it earlier
	int TurnNum = 0;

	if(LogOrNot == true){
		FILE *Log = fopen("log.txt", "a");
		time_t TimeAndDate = time(&TimeAndDate);
		fprintf(Log, "UNO v%s; Developed by techactivate781~ :3. System date and time: %s", Version, ctime(&TimeAndDate));
		fprintf(Log, "Please do not cheat by reading the values of the computer.\n\n");
		fprintf(Log, "<----- Starting uno session; preparing game ----->\n");
		fprintf(Log, "Creating PlayerCards, ComCards, Turn, Amounts, CurrentCard, ChangeColor, ChosenColor, and TurnNum variables... DONE\n");
		fclose(Log);
		// there's zero inspiration from c:\windows\logs\dism\dism.log. idk what you're talking about.
		// i also find it funny how the logs are kinda hardcoded for now. this is because there's much larger issues than the log if the program doesn't get there.
		// maybe one day i'll do it properly.
	}

	int Test;
	Test = 0;

	for(int i = 0; i < 7; i++) {
		ComCards[i][0] = GiveCard();
		Amounts[1]++;
		ComCards[i][1] = GiveColor(ComCards[i][0]);
		PlayerCards[i][0] = GiveCard();
		Amounts[0]++;
		PlayerCards[i][1] = GiveColor(PlayerCards[i][0]);
	}

	//printf("test"); // temp to see where it crashes

	if(LogOrNot == true) {
		FILE *Log = fopen("log.txt", "a");
		fprintf(Log, "Giving a card to all cards... DONE\nGiving a color to all cards... DONE\n\n");
		fprintf(Log, "<----- Starting uno game ----->\n");
		Logging(Log, PlayerCards, ComCards, Amounts, CurrentCard, Turn, TurnNum);
		fclose(Log);
	} // oh my days. it was failing because i was using one equal instead of two...

	Turn = 0; // computer's turn
	int CardToPlay = ChooseComputerTurn(ComCards, Amounts, CurrentCard, 1, 0);
	CurrentCard[0] = ComCards[CardToPlay - 1][0];
	CurrentCard[1] = ComCards[CardToPlay - 1][1];
    ChosenCards[0] = CurrentCard[0];
    ChosenCards[1] = CurrentCard[1];
	Amounts[1]--;
	ComCards[CardToPlay - 1][0] = 0;
	ComCards[CardToPlay - 1][1] = 0;
	FixCards(ComCards, Amounts[1]);
	TurnNum++;
	MiddlePerson(1, PlayerCards, ComCards, Amounts, CurrentCard, ChosenCards, pTurn);

	/* interjecting our own cards here for testing:
	ChosenCards[0] = 14; // plus4
	ChosenCards[1] = 0; // 0
	CurrentCard[0] = 14;
	CurrentCard[1] = 0;

	// MiddlePerson(1, PlayerCards, ComCards, Amounts, CurrentCard, ChosenCards, Turn);
	ProcessChangeColor(1, CurrentCard, ComCards, Amounts);
	Turn = 0; // player's turn
	
	return 0;*/

	if(LogOrNot == true) {
		FILE *Log = fopen("log.txt", "a"); // open/close the file every time to fix it?
		Logging(Log, PlayerCards, ComCards, Amounts, CurrentCard, Turn, TurnNum);
		fclose(Log);
	}

	// return 0; // temp for bugtesting

	/*this is temp for while i attempt to fix the bug
        CurrentCard[0] = 7;
        CurrentCard[1] = 1; //red
        PlayerCards[0][0] = 13;
        PlayerCards[0][1] = 0;*/

	/*printf("%d\n", CardToPlay);
	Interpreter(ComCards, Amounts[1], 1000, 0, 0); // just a quick test
	*/

	int TurnToPlay; // turn for player
	int IsCardRight = 0; // will be one once we see if the card is correct
	int IsThereACardToPlay = 0;

	while(Amounts[0] != 0 || Amounts[1] != 0) {
		if(Turn == 0){
			// by doing it in a if, if it becomes the user's turn again, it'll make sure that it asks the user
			Interpreter(PlayerCards, Amounts[0], 1000, 1, CurrentCard);
			/*since we have the functionality for the computer to get a card from the deck, we may as well
		  	hardcode it here because it'll happen every time..?*/
        	//clearly there's a major bug here, smth is going wrong..? found it instantly. 
        	//MAJOR BUG. IT BREAKS OUT OF THE FIRST FOR BUT NOT THE MAIN WHILE.
        	// AND IT WORKS. first attempt gave me two cards as it was needed.
			while(IsThereACardToPlay == 0) {
				for(int i = 0; i < Amounts[0]; i++) {
					if(PlayerCards[i][0] == 13 || PlayerCards[i][0] == 14) {
						IsThereACardToPlay = 1;
						break; // no need for new card if you have +4/wild
					}

					if(PlayerCards[i][0] == CurrentCard[0]) {
						IsThereACardToPlay = 1;
						break; // you have a playable card
					}

					if(PlayerCards[i][1] == CurrentCard[1]) {
						IsThereACardToPlay = 1;
						break; // same as above.
					}
				}
			
				if(IsThereACardToPlay == 1){
			    	break; // there's a card you can play, so no need
				}	
			
				//	if you're here, that means you don't have any playable card
				PlayerCards[Amounts[0]][0] = GiveCard();
				PlayerCards[Amounts[0]][1] = GiveColor(PlayerCards[Amounts[0]][0]);
				Amounts[0]++;
				printf("Since you didn't have a suitable card to play, you were given this card: ");
				Interpreter(PlayerCards, Amounts[0], Amounts[0] - 1, 0, 0);
				// this will run as many times as it needs to before the player gets a playable card
			}

			printf("\nPlay a card - type the number: ");
		
			while(IsCardRight == 0){
				scanf("%d", &CardToPlay);
				CardToPlay--; // this is because the 1st card the user sees is the 0th card in the array
				ChosenCards[0] = PlayerCards[CardToPlay][0];
				ChosenCards[1] = PlayerCards[CardToPlay][1];
				IsCardRight = CheckIfCardIsRight(CurrentCard, ChosenCards);
				if(IsCardRight == 0) {
					printf("\nYou are unable to play this card based on the current card - try again: ");
				}
			}
        	printf("You've played this card: ");
			Interpreter(PlayerCards, Amounts[0], CardToPlay, 0, 0); // obviously this and the line above have to above when we remove the card...
			CurrentCard[0] = PlayerCards[CardToPlay][0];
			CurrentCard[1] = PlayerCards[CardToPlay][1];
			Amounts[0]--;
			PlayerCards[CardToPlay][0] = 0;
			PlayerCards[CardToPlay][1] = 0;
			FixCards(PlayerCards, Amounts[0]);
			TurnNum++;
			Turn = 1; // now teh computer's turn, same reason for putting this above as Turn = 0;
			MiddlePerson(0, PlayerCards, ComCards, Amounts, CurrentCard, ChosenCards, pTurn);
			if(LogOrNot == true) {
				FILE *Log = fopen("log.txt", "a");
				Logging(Log, PlayerCards, ComCards, Amounts, CurrentCard, Turn, TurnNum);
				fclose(Log);
			}

        	IsCardRight = 0; // this must be put to zero so that the correct loop will run again.

			if(Amounts[0] == 0) {
				break; // otherwise the computer would get to play after you've won.
				// not needed below as there it is the last thing in the loop
			}

		}

		if(Turn == 1){
			CardToPlay = ChooseComputerTurn(ComCards, Amounts, CurrentCard, 0, 0);
			CurrentCard[0] = ComCards[CardToPlay - 1][0];
			CurrentCard[1] = ComCards[CardToPlay - 1][1];
        	ChosenCards[0] = CurrentCard[0];
        	ChosenCards[1] = CurrentCard[1];
			Amounts[1]--;
			ComCards[CardToPlay - 1][0] = 0;
			ComCards[CardToPlay - 1][1] = 0;
			FixCards(ComCards, Amounts[1]);
			TurnNum++;
			Turn = 0; // really, this should be above. because then if MiddlePerson modified it.. it'll be overwritten? lol? 
			MiddlePerson(1, PlayerCards, ComCards, Amounts, CurrentCard, ChosenCards, pTurn);
			if(LogOrNot == true) {
				FILE *Log = fopen("log.txt", "a");
				Logging(Log, PlayerCards, ComCards, Amounts, CurrentCard, Turn, TurnNum);
				fclose(Log);
			}
		}
	}

	if(Amounts[0] == 0) {
		printf("Congratulations! You won!!\n");
	}

	if(Amounts[1] == 0) {
		printf("The computer has won\n");
	}

	return 0;
}

void Logging(FILE* path, int PlayerCards[100][2], int ComCards[100][2], int Amounts[2], int CurrentCard[2], int Turn, int TurnNum) {
	fprintf(path, "Turn number is %d\n", TurnNum);
	fprintf(path, "PlayerCards dump: ");
	for(int i = 0; i < Amounts[0]; i++) {
		fprintf(path, "%d %d   ", PlayerCards[i][0], PlayerCards[i][1]); // good enough (not)
	}
	fprintf(path, "\nComCards dump: ");
	for(int i = 0; i < Amounts[1]; i++) {
		fprintf(path, "%d %d   ", ComCards[i][0], ComCards[i][1]);
	}
	fprintf(path, "\nAmounts: %d %d", Amounts[0], Amounts[1]);

	fprintf(path, "\nCurrentCard: %d %d", CurrentCard[0], CurrentCard[1]);
	fprintf(path, "\nTurn: %d", Turn);
	fprintf(path, "\nTurnNum: %d\n", TurnNum);

	fprintf(path, "\n\n"); // just to make the ends a bit clearer.

}

void MiddlePerson(int Cop, int PlayerCards[100][2], int ComCards[100][2], int Amounts[2], int CurrentCard[2], int ChosenCards[2], int *Turn) {
    //printf("%d %d %d %d %d %d %d %d %d %d %d %d", Cop, PlayerCards[0][0], PlayerCards[0][1], ComCards[0][0], ComCards[0][1], Amounts[0], Amounts[1], CurrentCard[0], CurrentCard[1], ChosenCards[0], ChosenCards[1], Turn);


	if(Cop == 0) {
		// this is it's the player's turn we're processing. we've already checked that the card is right prior to calling this func

		switch(ChosenCards[0]) {
		case 10:
            //printf("meow");
			(*Turn) = ProcessSkip(0, (*Turn));
			break;

		case 11:
            //printf("uwu\n");
			(*Turn) = ProcessReverse(0, (*Turn));
			break;

		case 12:
            //printf("nyaaa~!!");
			ProcessPlusTwo(0, ComCards, Amounts);
			break;

		case 13:
            //printf(":3");
			ProcessPlusFour(0, CurrentCard, ComCards, Amounts, PlayerCards);
			break;

		case 14:
            //printf("test");
			ProcessChangeColor(0, CurrentCard, PlayerCards, Amounts);
			break;

		default:
			break;
		} // this is for the good cards. for other ones.. no real need to call this function
	} // ... i have no comments. i was putting if(cop == 1) inside if(cop == 0)... :(

		if(Cop == 1) {
			switch(ChosenCards[0]) {
			case 10:
				(*Turn) = ProcessSkip(1, (*Turn));
				break;

			case 11:
				(*Turn) = ProcessReverse(1, (*Turn));
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

int FixCards(int Cards[100][2], int Amount) {
	int test = 0;
	int test2;
	int test3 = 0;
	int counter = 0;

	do {
		for(int i = 0; i <= Amount; i++) {
			if(Cards[i][0] == 0 && Cards[i][1] == 0) {
				test = i+1;
				Cards[i][0] = Cards[test][0];
				Cards[i][1] = Cards[test][1];
				for(int j = 0; j <= Amount - i; j++) {
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

	for(int i = 0; i <= Amount; i++) {
		if(Cards[i][0] == 0 && Cards[i][1] == 0) {
			counter++;
		}
	}

	//counter-=3; // add one more because the last cards[amount][1]  was 0,0

	int final = Amount - counter + 1; // this is a weird fix but idk why.
	return final;
}


void ProcessPlusFour(int WhoMad, int CurrentCard[2], int Cards[100][2], int Amounts[2], int OpponentCards[100][2]) {
	// this will sure be fun. i dont want this to be linked to main change color, so i'll have my own stuff here

	if(WhoMad == 0) {
		printf("Four cards have been given to the computer\n");
		for(int i = 0; i < 4; i++) {
			Cards[Amounts[1]][0] = GiveCard();
			Cards[Amounts[1]][1] = GiveColor(Cards[Amounts[1]][0]);
			Amounts[1]++;
		}

		ProcessChangeColor(0, CurrentCard, OpponentCards, Amounts);
		//passing othercards here as its the user's cards that matter here.

		// i don't think we have to do more here
	}


	if(WhoMad == 1) {
		printf("You have been given four cards. They are:\n");

		for(int i = 0; i < 4; i++) {
			Cards[Amounts[0]][0] = GiveCard();
			Cards[Amounts[0]][1] = GiveColor(Cards[Amounts[0]][0]);
			Amounts[0]++;

			Interpreter(Cards, Amounts[0], (Amounts[0]) -1, 0, 0);
		}

		int ComputerCardChosen = ChooseComputerTurn(OpponentCards, Amounts, CurrentCard, 0, 1);
		CurrentCard[0] = OpponentCards[ComputerCardChosen][0];
		CurrentCard[1] = OpponentCards[ComputerCardChosen][1];

		printf("The computer played this card: ");
		Interpreter(OpponentCards, Amounts[1], ComputerCardChosen, 0, 0); // we'll just pass the card before removing it from the cmoputer *shrug*


		Amounts[1]--;
		OpponentCards[ComputerCardChosen][0] = 0;
		OpponentCards[ComputerCardChosen][1] = 0;  // that's it no?
	}
}


void ProcessPlusTwo(int WhoMad, int Cards[100][2], int Amounts[2]) {
	if(WhoMad == 1) {
		printf("You have been given two cards as the computer played a Plus 2. They are:\n");
		for(int i = 0; i < 2; i++) {
			Cards[Amounts[0]][0] = GiveCard(); // i think the issue here is the amuont[0] + 1
			Cards[Amounts[0]][1] = GiveColor(Cards[Amounts[0]][0]);
			Amounts[0]++;
			Interpreter(Cards, Amounts[0], (Amounts[0]) -1, 0, 0);
		}

	}

	if(WhoMad == 0) {
		printf("The computer has been given two cards!!\n"); // WHY DIDN'T I ADD THIS EARLIER.
		for(int i = 0; i < 2; i++) {
			Cards[Amounts[1]][0] = GiveCard();
			Cards[Amounts[1]][1] = GiveColor(Cards[Amounts[1]][0]);
			Amounts[1]++;
		}
	}
}

void ProcessChangeColor(int WhoMad, int CurrentCard[2], int Cards[100][2], int Amounts[2]) {
	int ColorToChangeTo;
	int CardToChoose;

	//FILE *log = fopen("log.txt", "a");
	//fprintf(log, "%d %d %d %d %d %d %d", WhoMad, CurrentCard[0], CurrentCard[1], Cards[0][0], Cards[0][1], Amounts[0], Amounts[1]);
    
    
	if(WhoMad == 1) {// oh my days all along hte issue was the 1 and 0 was swapped...
		//we'll basically find a suitable card, play that, change the color to it. easy
		// chooseComputerTurn(int cards[100][2], int amounts[2], int currentCard[2], int IsItTheStart)
		// pasting this here as i don't want to scroll down constantly
		int ComputerCardChange = ChooseComputerTurn(Cards, Amounts, CurrentCard, 0, 1); //remember to remove change color from deck.
		printf("The computer has played the following card by changing the color: ");
		Interpreter(Cards, Amounts[1], ComputerCardChange, 0, 0);
		//IS THIS A BUG THAT WE'RE DOING IT TO COMPUTERCHARDCHANGE INSTEAD OF CARDTOCHOOSE
		CurrentCard[0] = Cards[ComputerCardChange][0];
		CurrentCard[1] = Cards[ComputerCardChange][1];
		Amounts[1]--;
		Cards[ComputerCardChange][0] = 0;
		Cards[ComputerCardChange][1] = 0;
		FixCards(Cards, Amounts[1]);
	} // much easier here because the computer won't put a wrong card, lol...
	printf("test2");
	// todo: tell the user what color the computer made it to.
	//fprintf(log, "%d %d %d %d %d %d %d", WhoMad, CurrentCard[0], CurrentCard[1], Cards[0][0], Cards[0][1], Amounts[0], Amounts[1]);
    //fclose(log);
    
	if(WhoMad == 0) {
		printf("Choose a color to change the color to\n");
		printf("Type 1 for red\nType 2 for yellow\nType 3 for green\nType 4 for blue\n");


		int JustForTest = 0; // idea is. if there's no card of the color the user changed it to,
		// we'll keep thsi to zero and before the loop ends we'll tell them to do it again.

		while(1) {
			scanf("%d", &ColorToChangeTo);
			//printf("test1 %d %d\n", colortochangeto, justfortest);
			if(ColorToChangeTo >= 5 || ColorToChangeTo <= 0) {
				printf("You have entered an incorrect color. Please write a color again\n");
				continue; // this'll fix it.
			}

			for(int i = 0; i < Amounts[0]; i++) {
				if(Cards[i][1] == ColorToChangeTo) {
					JustForTest = 1;
					break;
				}
			}

			if(JustForTest == 0) {
				printf("Choose a color again - you do not have a card of that color\n");
			}

			if(JustForTest == 1) {
				//printf("quicktest\n");
				break;
			}

		}


		printf("Choose a card to play: ");
		Interpreter(Cards, Amounts[0], 1000, 0, 0);

		while(1) {

			scanf("%d", &CardToChoose);
			CardToChoose--; //since 1st card for user is 0th card in array

			if(Cards[CardToChoose][1] != ColorToChangeTo) {
				printf("You chose an invalid card based on the color you chose - please choose again\n");
			}

			else {
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

int ChooseComputerTurn(int Cards[100][2], int Amounts[2], int CurrentCard[2], int IsItTheStart, int ChangeColor) {
	// this is a very weird fix, but it's one i'm okay with doing until i eventuially rewrite everything
	if(IsItTheStart == 1) {
		// technically i can hardcode this as 7 because we know that there'll only be 7 cards at the start
		return rand() % (7 - 1 + 1) + 1;
	}

	int TotalColors[6] = {0}; // if the 0th index is zero, we have no cards that odn't have a color. if the 1st index is 5, we have 5 red cards, etc. now to fill these...
	int TotalCards[15] = {0};

	// honestly, before i start working on this, fairly vital to make the move to structs. actually no, because that's tedious and for when im more tired.

	int Temp = 0;
	for(int i = 0; i < Amounts[1]; i++){
		Temp = Cards[i][0];
		TotalCards[Temp]++;
		Temp = Cards[i][1];
		TotalColors[Temp]++;
	}

	for(int i = 0; i < 15; i++){
		printf("%d ", TotalCards[i]);
	}
	printf("\n\n");
	for(int i = 0; i < 5; i++){
		printf("%d", TotalColors[i]);
	} 
	printf("\n");
	

	// 1. change colors often.. i suppose we do that.
	// 2. save plus4/change color for when less cards are there - only use it when there are less than two available colors?
	// 3. we want to use colors with less cards first.
	// ideally, we'd have not only a weighting of what to play (different factors combine to give a weighting), but also have a list of what cards/colors we have.
	if(ChangeColor == 1){
	    int NumOfColorOfCard[4] = {0}; // thsi is the number of cards that have a color
	    for(int i = 0; i < Amounts[1]; i++){
	        if(Cards[i][1] == 1){
	            NumOfColorOfCard[0]++;
	        }
	        
	        if(Cards[i][1] == 2){
	            NumOfColorOfCard[1]++;
	        }
	        
	        if(Cards[i][1] == 3){
	            NumOfColorOfCard[2]++;
	        }
	        
	        if(Cards[i][1] == 4){
	            NumOfColorOfCard[3]++;
	        }
	    } // now we know how many of what card the computer has.
	    int ColorToChangeTo = rand() % 3 - 0 + 1 + 0;
	    int test;
	    while(1){
	        // very weird way of doing this, i know
	        test = rand() % 4 + 1 + 1 + 1; // what's the point of doing +0 above???
	        if(test == 1 && NumOfColorOfCard[0] != 0){
	            break;
	        }
	        
	        if(test == 2 && NumOfColorOfCard[1] != 0){
	            break;
	        }
	        
	        if(test == 3 && NumOfColorOfCard[2] != 0){
	            break;
	        }
	        
	        if(test == 4 && NumOfColorOfCard[3] != 0){
	            break;
	        }

	    }
	    
	    while(1){
            for(int i = 0; i < Amounts[1]; i++){
                if(Cards[i][1] == test){
                    return i; // a card was found.
                }
            }

			// if we're here, this basically means that the only card left was the plus4/changecolor.. really, shouldn't be like this and i should make a functionl for it but uhh
	    }
	    
	}

	// dont use amounts[0] lol
	while(1) {
		for(int i = 0; i < Amounts[1]; i++) {
			//randomly realized that with this method, if there's a plus 2 and you put a plus 2, it'd be two to you and two to PC rather than both to you.. hmm.. FIX THIS
			if(CurrentCard[0] == 12 && Cards[i][0] == 12) {
				return i;
			}
			// i also just realized, these will all have to be in separate for functions because otherwise it'd always do the first possible thing rather than what you want, hm
			// i wonder if this could be done from switch..? or nested for?
		}

		for(int i = 0; i < Amounts[1]; i++) {
			if(CurrentCard[0] == 12 && Cards[i][0] == 13) {
				return i; // if the user put a +2, we're going with a +4.. uh...
			}
		}

		for(int i = 0; i < Amounts[i]; i++) {
			if(CurrentCard[0] == 10 && Cards[i][0] == 10 || CurrentCard[0] == 11 && Cards[i][0] == 1) {
				return i; // if the user put a plus/skip... ig we put that too??
			}
		}

		// the issue i'm realizing here is that this will be super easy to predict....

		for(int i = 0; i < Amounts[i]; i++) {
			if(CurrentCard[0] == Cards[i][0] || CurrentCard[1] == Cards[i][1]) {
				return i;
			}
		}

		// if we're here, that means that the computer had no cards it could do - this means we keep on giving it new cards until we satisfy this.
		// but that means we shoulddo the whole thing in a loop...

		printf("The computer took a new card as it had no possible cards\n");
		Cards[Amounts[1]][0] = GiveCard();
		Cards[Amounts[1]][1] = GiveColor(Cards[Amounts[1]][0]); // i think this is fine.

	}
}

int CheckIfCardIsRight(int CurrentCard[2], int ChosenCards[2]) {
	if(ChosenCards[0] == 14 || ChosenCards[0] == 13) {
		return 1; // 1 will be returned if its a legal turn
	}

	if(CurrentCard[0] == ChosenCards[0] || CurrentCard[1] == ChosenCards[1]) {
		return 1;
	}

	return 0;
}

int ProcessSkip(int WhoMad, int Turn) {
	if(WhoMad == 1) {
		printf("Your turn has been skipped!\n");
		return 1;
	}

	if(WhoMad == 0) {
		printf("You have skipped the computers turn!\n");
		return 0;
	}
}

int ProcessReverse(int WhoMad, int Turn) {
	if(WhoMad == 1) {
		printf("It is the computers turn as the computer chose to reverse it!\n");
		return 1;
	}

	if(WhoMad == 0) {
		printf("It is your turn as you chose to reverse it!\n");
		return 0;
	}
}

int GiveCard() {
	int Card = rand() % (14 - 0 + 1) + 0; // 10 = skip, 11 = reverse, 12 = plus2, 13 = plus4, 14=changecolor

	// if the card is a +4 or a change color, there's a 50% chance that it'll regive a card

	if(Card == 13 || Card == 14 && (rand() % (2 - 1 + 1) + 1) == 2) {
		Card = rand() % (14 - 0 + 1) + 0;
	}

	return Card;
}

int GiveColor(int Card) {
	int Color = rand() % (4 - 1 + 1) + 1;

	if(Card == 13 || Card == 14) {
		return 0; // 0 is black. otherwise its in roygbiv in terms of what the colors are (red=1, yellow=2, etc).
	}

	return Color;
}

// all the repetitive functions come down here lol.

void InterpretColor(int Color) {
	switch(Color) {
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

void Interpreter(int Cards[100][2], int CardAmount, int Card, int ShowCard, int CurrentCard[2]) {

	if(Card == 1000) {
		printf("You have these cards:\n");
		for(int i = 0; i < CardAmount; i++) {
			switch(Cards[i][1]) {
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

			switch(Cards[i][0]) {
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

	else {
		switch(Cards[Card][1]) {
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

		switch(Cards[Card][0]) {
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

	if(ShowCard == 1) {
		printf("Current card is: ");
		switch(CurrentCard[1]) {
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

		switch(CurrentCard[0]) {
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
