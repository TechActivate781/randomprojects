/* todo:
reduce the whole point of main() - it's too large, put more things in different functions.

what would be cool... is... a compujter v computer...?? i really am coming up with too many ideas and implementing too little... [later]
make a limited number of cards - 108 - that cards can be gotten from. [sorta.. good enough :3] 
make it a 50-50 chance that the computer will start first. [later]
make it so that computer can't play plus4/changecolor as last card - at very least, second last. [sorta]

for 0.95, add a computer v computer mode - *should* be simple in theory + readd player v player.. 
for 1.00, add NO NEW FEATURES FOR THIS RELEASE. bugfix + tons of small rewrites where needed for 1.0, reduce scope of main(), make functinos clearer. tehn make some documentation of how it works
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

char Version[50] = "0.9 Release"; 
bool StrategyInProgress = false; // this is for when we do a plus4, etc, after a reverse/skip. global variables do seem a bit illegal though, this may be fixed in the future.
bool LogOrNot = false; // this is global so that LogErr() can use it needs to - otherwise LogOrNot would have to be passed to every function that could do an error 
// sure am using too many global variables... feels illegal.

typedef struct{
    int Cards[100][2];
    int Amount;
}Hand; // this is part of a major change to structs.

void Logging(FILE* path, Hand PlayerCards, Hand ComputerCards, int CurrentCard[2], int Turn, int TurnNum, float Analyze, int Last[3][2]);
void LogErr(int err);
int ValidateFTInput(); // this is for when we need a 0/1 input, really we've had to do this so many times i've put it in a func

int GiveCard(); // will give a random card to someone
int GiveColor(int Card); // will give a color to the card
void Interpreter(Hand User, int Card, int ShowCard, int CurrentCard[2]); // this will show the user all of his/her/their cards
void InterpretColor(int Color); // this is for just printing a color - use for a change color

int ProcessSkip(int WhoMad, int Turn);
int ProcessReverse(int WhoMad, int Turn); //this is bascially the same as above. changed to int since i need the value to update.. lol
void ProcessPlusTwo(int WhoMad, Hand *User);
void ProcessPlusFour(int WhoMad, int CurrentCard[2], Hand *User0, Hand *User1, float Analyze, int *Turn); // user0 = player, user1 = computer
void ProcessChangeColor(int WhoMad, int CurrentCard[2], Hand *User, float Analyze, Hand *OtherUser, int *Turn);

void MiddlePerson(int Cop, Hand *Player, Hand *Computer, int CurrentCard[2], int ChosenCards[2], int *Turn, float Analyze); // idea here is that when a number is chosen it'll go through here before calling the current function??? everything will have to be passed ig
// cop is computer or player - if this is 1, that means that we're doing computer stuff, otherwise player
int FixCards(Hand MainHand);
int FixCardsOld(int Cards[100][2], int Amount);

int MultipleCardsPerTurn(int ChosenCards[2], Hand *User);
int MultipleColorsPerTurn(int ChosenCards[2], Hand *User);
int CheckIfCardIsRight(int CurrentCard[2], int ChosenCards[2]);
void MultipleCards(int CurrentCard[2], Hand *User, int LastFourCards[4][2], Hand *Computer, int *pTurn, float Analyze);
void MultipleCardsForComputer(int CurrentCard[2], Hand *Computer, int LastFourCards[4][2], Hand *User, int *pTurn, float Analyze);

int ChooseComputerTurn(int Cards[100][2], int *Amounts, int CurrentCard[2], int IsItTheStart, int ChangeColor, float Analyze, Hand Computer); //second last int basically asking if its the start of the game; if so, it'll just give a random card
float AnalyzePlayer(int LastFewCards[4][2], Hand ComputerStuff);
void FixLastFew(int LastFewCards[4][2]);
int TestHeuristic(int LastFourCards[4][2], int LastThreeAmounts[3], int CurrentCard[2], Hand Computer, float Analyze, int RequestedReturn, int ColorOrCard, int ValueOfColorOrCard);
// requestedreturn is just checking do we want to return the value of the heuristic, or the card number that has the highest, or if we want to start a strat (will return 122 if so)
// if colororcard = 0, continue as normal. if colororcard = 1, then give a card which has a number of whatever valueofcolororcard is, if colororcard = 2, that for color.

int main() { 
	char OS[100]; // 0 = win32, 1 = win64, 2 = linux, 3 = freebsd
	#ifdef _WIN32
		strcpy(OS, "Win32");
	#endif
	
	#ifdef _WIN64
		strcpy(OS, "Win64");
	#endif

	#ifdef __linux__
		strcpy(OS, "Linux (pretty pwease no flame me for not knowing the exact distro)"); 
	#endif

	#ifdef __FreeBSD__
		strcpy(OS, "FreeBSD");
	#endif

	printf("Logging does not collect any personal data (other than the date/time). Should an unexpected move/bug occur, you may send me the unoLog.txt file generated as an issue on the GitHub.\nWould you like to log the game to a file (1 or 0): ");
	int LogOrNotInInt = ValidateFTInput();

	if(LogOrNotInInt == 0) {
		LogOrNot = false;
	}

	if(LogOrNotInInt == 1) {
		LogOrNot = true;
	}

	LogOrNotInInt = 1000; // being reused for below bit

	if(LogOrNot == true){
		printf("Would you like to append to an existing unoLog.txt file in the current directory, or would you like to create a new one and potentially overrite an existing unoLog.txt\n");
		printf("Type 1 to overwrite, type 0 to keep the file: ");

		LogOrNotInInt = ValidateFTInput(); // modifying this to use the newly made function
	}

	if(LogOrNotInInt == 1){
		FILE *pLog = fopen("unoLog.txt", "w");
		if(pLog == NULL){
			perror("Error opening file");
			return 1; // really, if it opens fine once, it should be fine all the time.
		}
		fclose(pLog); // just writing to the file if they want us to overwrite a previous log.txt
	}

	if(LogOrNotInInt == 0){
		// just for testing.
		FILE *pLog = fopen("unoLog.txt", "a");
		if(pLog == NULL){
			perror("Error opening file");
			return 1; // really, if it opens fine once, it should be fine all the time.
		}
		fclose(pLog); 
	}

	srand(time(NULL));

	printf("Would you like two players to play against each other? Type 1 if you want to, type 0 if you do not: ");
	int TwoPlayersPlaying = ValidateFTInput();
	if(TwoPlayersPlaying == 1){
		printf("This feature has been removed due to bugs. It will be readded in a future version\n");
		printf("The app is exiting with error code 0.\n");
		return 0;
	}

	/*int ComCards[100][2] = {{0}, {0}};
	int PlayerCards[100][2] = {{0}, {0}}; 
	int Amounts[2] = {0, 0}; // this is amount of cards the user/computer have; 1 is for computer, 0 is for user. 

	THIS IS THE PREVIOUS VERSION OF IT, KEEPING FOR NO REAL REASON*/

	Hand Player = {0};
	Hand Computer = {0};
	int Turn = 0; // this assigns who's turn it is. since the computer plays first, its now the user's turn. 1 = comp, 0 = user
	int *pTurn = &Turn; // this is to pass into middleperson
	int CurrentCard[2] = {0}; // the first value is the card, then the color
	int ChangeColor; //this will hold the value ofr the color that you're changing the color to. is this even used? idk
	int ChosenCards[2] = {0, 0}; // moving this up so that we can use it earlier
	int TurnNum = 0;

	if(LogOrNot == true){
		FILE *Log = fopen("unoLog.txt", "a");
		time_t TimeAndDate = time(&TimeAndDate);
		fprintf(Log, "UNO v%s; Developed by techactivate781~ :3. System date and time: %s", Version, ctime(&TimeAndDate));
		fprintf(Log, "Current Operating System: %s \n", OS);
		fprintf(Log, "Please do not cheat by reading the values of the computer.\n");
		fprintf(Log, "NOTE: If you are two players playing against each other, treat Computer values like the 2nd player's\n\n");
		fprintf(Log, "<----- Starting uno session; preparing game ----->\n");
		fprintf(Log, "Making the move to using structs [DONE]\n"); // lol
		if(TwoPlayersPlaying == 1){
		    fprintf(Log, "[NOTE] Two Player mode chosen\n");
		    fprintf(Log, "[NOTE] Player vs Player mode. Expect bugs.\n");
		}
		if(TwoPlayersPlaying == 0){
		    fprintf(Log, "[NOTE] Computer vs Player mode chosen\n");
		}
		fprintf(Log, "Creating the hands for the player and computer [DONE]\n");
		fprintf(Log, "Creating Turn, Amounts, CurrentCard, ChangeColor, ChosenColor, and TurnNum variables [DONE]\n");
		fclose(Log);
		// there's zero inspiration from c:\windows\logs\dism\dism.log. idk what you're talking about.
		// i also find it funny how the logs for making variables etc kinda hardcoded for now. 
		// this is because there's much larger issues than the log if the program doesn't make them.
		// maybe one day i'll do it properly.
	}
	int Test;
	Test = 0;

	for(int i = 0; i < 7; i++) {
		Computer.Cards[i][0] = GiveCard();
		Computer.Amount++;
		Computer.Cards[i][1] = GiveColor(Computer.Cards[i][0]);
		Player.Cards[i][0] = GiveCard();
		Player.Amount++;
		Player.Cards[i][1] = GiveColor(Player.Cards[i][0]);
	}

	Player.Cards[0][0] = 14;
	Player.Cards[0][1] = 0;
	Player.Cards[1][0] = 11;
	Player.Cards[1][1] = 2;
	Player.Cards[2][0] = 10;
	Player.Cards[2][1] = 2;
	Player.Cards[3][0] = 7;
	Player.Cards[3][1] = 2;

	int TurnToPlay; // turn for player
	int IsCardRight = 0; // will be one once we see if the card is correct
	int IsThereACardToPlay = 0;

	int LastFourCards[4][2] = {0}; // last three cards the player chose
	float ResultOfAnalyze = 0;
	int counter = 0; // every three turns, we redo the counter.

	if(LogOrNot == true) {
		FILE *Log = fopen("unoLog.txt", "a");
		fprintf(Log, "Giving a card to all cards [DONE]\nGiving a color to all cards [DONE]\n\n");
		fprintf(Log, "<----- Starting uno game ----->\n");
		Logging(Log, Player, Computer, CurrentCard, Turn, TurnNum, 0, LastFourCards);
		fclose(Log);
	} // oh my days. it was failing because i was using one equal instead of two...

	Turn = 0; // player's turn
	int CardToPlay = ChooseComputerTurn(Computer.Cards, &Computer.Amount, CurrentCard, 1, 0, 0, Computer); // analyze doesn't matter for the first move.
	CurrentCard[0] = Computer.Cards[CardToPlay - 1][0];
	CurrentCard[1] = Computer.Cards[CardToPlay - 1][1];
    ChosenCards[0] = CurrentCard[0];
    ChosenCards[1] = CurrentCard[1];
	Computer.Amount--;
	Computer.Cards[CardToPlay - 1][0] = 0;
	Computer.Cards[CardToPlay - 1][1] = 0;
	FixCardsOld(Computer.Cards, Computer.Amount);
	TurnNum++;

	MiddlePerson(1, &Player, &Computer, CurrentCard, ChosenCards, pTurn, ResultOfAnalyze);

	if(LogOrNot == true) {
		FILE *Log = fopen("unoLog.txt", "a"); // open/close the file every time to fix it?
		Logging(Log, Player, Computer, CurrentCard, Turn, TurnNum, ResultOfAnalyze, LastFourCards);
		fclose(Log);
	}

	while(Player.Amount != 0 && Computer.Amount != 0) {
		if(Turn == 0){
			if(TwoPlayersPlaying == 1){
				printf("Player zero's turn\n");
			}
			// by doing it in a if, if it becomes the user's turn again, it'll make sure that it asks the user
			if(counter == 4){
				counter = 0; // to reset the counter
			}
			/*since we have the functionality for the computer to get a card from the deck, we may as well
		  	hardcode it here because it'll happen every time..?*/
			while(IsThereACardToPlay == 0) {
				for(int i = 0; i < Player.Amount; i++) {
					if(Player.Cards[i][0] == 13 || Player.Cards[i][0] == 14) {
						IsThereACardToPlay = 1;
						break; // no need for new card if you have +4/wild
					}

					if(Player.Cards[i][0] == CurrentCard[0]) {
						IsThereACardToPlay = 1;
						break; // you have a playable card
					}

					if(Player.Cards[i][1] == CurrentCard[1]) {
						IsThereACardToPlay = 1;
						break; // same as above.
					}
				}
			
				if(IsThereACardToPlay == 1){
			    	break; // there's a card you can play, so no need
				}	
			
				//	if you're here, that means you don't have any playable card
				Player.Cards[Player.Amount][0] = GiveCard();
				Player.Cards[Player.Amount][1] = GiveColor(Player.Cards[Player.Amount][0]);
				Player.Amount++;
				printf("Since you didn't have a suitable card to play, you were given this card: ");
				Interpreter(Player, Player.Amount - 1, 0, 0);
				// this will run as many times as it needs to before the player gets a playable card
			}

			Interpreter(Player, 1000, 1, CurrentCard); // moving this here to show cards that have been given.
			printf("\nPlay a card - type the number: ");
		
			while(IsCardRight == 0){
				scanf("%d", &CardToPlay);
				if(CardToPlay == 1002){
					LogErr(2);
				}

				if(CardToPlay == 1003){
					LogErr(3); // look. if you have 1000 cards, just end the game bruh. this will allow me to test errors
				}
				if(CardToPlay == 999){
					// this is just to create a log right here and now.
					if(LogOrNot == true){
						FILE *pLog = fopen("unoLog.txt", "a");
						fprintf(pLog, "<--This is a log made in the middle of a turn. Ignore values like Turn.-->\n");
						Logging(pLog, Player, Computer, CurrentCard, Turn, TurnNum, ResultOfAnalyze, LastFourCards);
						fclose(pLog);
						printf("Log made. Play a card now: "); 
						continue;
					}
				}
				CardToPlay--; // this is because the 1st card the user sees is the 0th card in the array
				ChosenCards[0] = Player.Cards[CardToPlay][0];
				ChosenCards[1] = Player.Cards[CardToPlay][1];
				IsCardRight = CheckIfCardIsRight(CurrentCard, ChosenCards);
				if(IsCardRight == 0) {
					printf("\nYou are unable to play this card based on the current card - try again: ");
				}
			}
        	printf("You've played this card: ");
			Interpreter(Player, CardToPlay, 0, 0); // obviously this and the line above have to be before you remove the card.
			CurrentCard[0] = Player.Cards[CardToPlay][0];
			CurrentCard[1] = Player.Cards[CardToPlay][1];
			LastFourCards[3][0] = Player.Cards[CardToPlay][0];
			LastFourCards[3][1] = Player.Cards[CardToPlay][1];
			FixLastFew(LastFourCards);
			Player.Amount--;
			Player.Cards[CardToPlay][0] = 0;
			Player.Cards[CardToPlay][1] = 0;
			FixCardsOld(Player.Cards, Player.Amount);

			// just before making stuff for the next turn, we're going to check if other cards cna be played w/ this
			if(MultipleCardsPerTurn(ChosenCards, &Player) != 0 || MultipleColorsPerTurn(ChosenCards, &Player) != 0){
				MultipleCards(CurrentCard, &Player, LastFourCards, &Computer, pTurn, ResultOfAnalyze);
			}

			TurnNum++;
			Turn = 1; // now teh computer's turn
			MiddlePerson(0, &Player, &Computer, CurrentCard, ChosenCards, pTurn, ResultOfAnalyze);
			
			counter++;
			if(counter == 3){
				ResultOfAnalyze = AnalyzePlayer(LastFourCards, Computer);
			}
			if(LogOrNot == true) {
				FILE *Log = fopen("unoLog.txt", "a");
				Logging(Log, Player, Computer, CurrentCard, Turn, TurnNum, ResultOfAnalyze, LastFourCards);
				fclose(Log);
			}

        	IsCardRight = 0; // this must be put to zero so that the correct loop will run again.
			IsThereACardToPlay = 0; // so that the loop for giving a card if the player is stuck will run again.

			if(Player.Amount == 0) {
				break; // otherwise the computer would get to play after you've won.
			}
		}

		if(Turn == 1 && TwoPlayersPlaying == 0){
			CardToPlay = ChooseComputerTurn(Computer.Cards, &Computer.Amount, CurrentCard, 0, 0, ResultOfAnalyze, Computer);
			CurrentCard[0] = Computer.Cards[CardToPlay][0];
			CurrentCard[1] = Computer.Cards[CardToPlay][1];
        	ChosenCards[0] = CurrentCard[0];
        	ChosenCards[1] = CurrentCard[1];
			sleep(1);
			printf("The computer played this card: ");
			Interpreter(Computer, CardToPlay, 0, 0);
			Computer.Amount--;
			Computer.Cards[CardToPlay][0] = 0;
			Computer.Cards[CardToPlay][1] = 0;
			FixCardsOld(Computer.Cards, Computer.Amount);
			TurnNum++;
			Turn = 0; // moved above middleperson so it doesn't overwrite any changes done to it from middleperson
			MiddlePerson(1, &Player, &Computer, CurrentCard, ChosenCards, pTurn, ResultOfAnalyze);
			if(MultipleCardsPerTurn(ChosenCards, &Computer) != 0 || MultipleColorsPerTurn(ChosenCards, &Computer) != 0){
				MultipleCardsForComputer(CurrentCard, &Computer, 0, &Player, pTurn, ResultOfAnalyze);
			}
			if(LogOrNot == true) {
				FILE *Log = fopen("unoLog.txt", "a");
				Logging(Log, Player, Computer, CurrentCard, Turn, TurnNum, ResultOfAnalyze, LastFourCards);
				fclose(Log);
			}
		}
	}

	if(Player.Amount == 0) {
		printf("Congratulations! You won!!\n");
	}

	if(Player.Amount == 0 && TwoPlayersPlaying == 1) {
		printf("Player zero has won!!!!\n");
	}

	if(Computer.Amount == 0 && TwoPlayersPlaying == 0) {
		printf("The computer has won. Better luck next time.\n");
	}

	if(Computer.Amount == 0 && TwoPlayersPlaying == 1) {
		printf("Player one has won!!\n");
	}

	return 0;
}

int TestHeuristic(int LastFourCards[4][2], int LastThreeAmounts[3], int CurrentCard[2], Hand Computer, float Analyze, int RequestedReturn, int ColorOrCard, int ValueOfColorOrCard){
	// todo: add bad cards support.
    int Answer; // we'll return this once we know what has the highest?
    Hand CloneOfHand = Computer;
        
    int *HeuristicValues = malloc(CloneOfHand.Amount * sizeof(int));
    if(HeuristicValues == NULL){
        LogErr(2);
    }
    
    for(int i = 0; i < CloneOfHand.Amount; i++){
        // if anyone is reading this, let me announce that i spent 40 minutes debugging an issue w/ this function
        // the issue? this loop had i < CloneOfHand.Amount * sizeof(int)... bruh...
        HeuristicValues[i] = 0;
    }
    
    // here we go again... :(
    
    int NumOfAllColors[5] = {0};
    // first index, aka [0], will go unused
    int NumOfAllCards[15] = {0};
    
    for(int i = 0; i < CloneOfHand.Amount; i++){
        int temp;
        temp = CloneOfHand.Cards[i][1];
        NumOfAllColors[temp]++;
    }
    
    for(int i = 0; i < CloneOfHand.Amount; i++){
        int temp;
        temp = CloneOfHand.Cards[i][0];
        NumOfAllCards[temp]++;
    }
    
    int NumOfPlus4OrWild = 0;
    for(int i = 0; i < CloneOfHand.Amount; i++){
        if(CloneOfHand.Cards[i][0] == 13 || CloneOfHand.Cards[i][0] == 14){
            NumOfPlus4OrWild++;
        }
    }

    for(int i = 0; i < CloneOfHand.Amount; i++){
        if(CloneOfHand.Cards[i][0] != CurrentCard[0] && CloneOfHand.Cards[i][1] != CurrentCard[1] && CloneOfHand.Cards[i][0] != 13 && CloneOfHand.Cards[i][0] != 14){
            HeuristicValues[i] = 119; // 119 = illegal card; is one before 120 just for when we're comparing against max.
            continue; 
            // if there's no card, we'll run this again for the next iteration
        }
        
        if(CurrentCard[0] != CloneOfHand.Cards[i][0]){
            HeuristicValues[i]+=2; // if its legal that we can change card, +1
        }
        
        if(CurrentCard[1] != CloneOfHand.Cards[i][1]){
            HeuristicValues[i]+=2; // if its legal that we can change color, +1
			if(Analyze == 1){
				HeuristicValues[i]++; // if it matches w/ strat, another +1
			}	
        }
        
        int temp;
        temp = CloneOfHand.Cards[i][0];
        if(NumOfAllColors[temp] == 1){
            HeuristicValues[i]+=3; // if its just one card of a color left, we'll do higher thing to get rid of it
        }
        
        temp = CloneOfHand.Cards[i][1];
        if(NumOfAllColors[temp] == 1){
            HeuristicValues[i]+=3; // same as above
        }
        
        if(NumOfAllColors[temp] == 1 && NumOfPlus4OrWild == 0){
            // although you could play a same card of diff color, that's risky as player could intercept and ruin plans, so we do -5
            HeuristicValues[i]-=5;
        }
        
        temp = CloneOfHand.Cards[i][0];
        
        if(NumOfAllCards[temp] == 1 && NumOfPlus4OrWild == 0){
			temp = CloneOfHand.Cards[i][1];
			if(NumOfAllColors[temp] == 0){
				// if there's only one other card, or less, that has the color of the card we're playing, and no plus4/change color, we're going w/ -4
				HeuristicValues[i]-=4;
			}
        }

		if((Analyze == 0.6 || Analyze == 0.5) && (CloneOfHand.Cards[i][0] == 12 || CloneOfHand.Cards[i][0] == 13)){
			bool IsThereASkipOrReverse = false;
			for(int j = 0; j < CloneOfHand.Amount; j++){
				if(CloneOfHand.Cards[j][0] == 11 || CloneOfHand.Cards[j][0] == 10){
					IsThereASkipOrReverse = true;
					if(RequestedReturn == 2){
						return 122; // this means to start a strategy 
					}
					break;
				}
			}

			if(IsThereASkipOrReverse == true){
				HeuristicValues[i]+=5; // +5 if it matches strat
			}

			if(IsThereASkipOrReverse == false){
				HeuristicValues[i]+=3; // +3 if it matches strat but no reverse/skip to go with it.
			}

			if(Analyze == 0.6){
				HeuristicValues[i]+=4; // if it's 0.6, that means that you have 10/more cards - this takes priority.
			}
		}

		// however, if its the only one we can play, do a +10 or smth
		bool IsThereOnePlayableCard = false;
		int NumOfPlayableCards = 0;
		for(int i = 0; i < CloneOfHand.Amount; i++){
			if(CloneOfHand.Cards[i][0] == 13 || CloneOfHand.Cards[i][0] == 14){
				NumOfPlayableCards++; // you can always play this
				continue;
			}

			if(CloneOfHand.Cards[i][0] == CurrentCard[0]){
				NumOfPlayableCards++;
				continue; // just reailzed, putting continue here is probably smart because otherwise if one card had same card/color as currentcard, it'd be counted as two cards.
			}

			if(CloneOfHand.Cards[i][1] == CurrentCard[1]){
				NumOfPlayableCards++;
				continue;
			}
		}

		if(NumOfPlayableCards == 0){
			return 100; // new card needed
		}

		if(NumOfPlayableCards == 1){
			for(int i = 0; i < CloneOfHand.Amount; i++){
				if(HeuristicValues[i] != 119){
					HeuristicValues[i] = 120; // 120 = only card that can be played
				}
			}
		}
    }

	int CardAnswer = 0; // this is the position of Answer 

	if(ColorOrCard == 1){
		Answer = -10;
		// return only card w/ the card stored in ValueOfColorOrCard
		for(int i = 0; i < CloneOfHand.Amount; i++){
			if(HeuristicValues[i] > Answer && ValueOfColorOrCard == CloneOfHand.Cards[i][0]){
				Answer = HeuristicValues[i];
				CardAnswer = i;
			}
		}

		if(Answer == -10){
			return 123; // 123 = none playable.
		}

		if(ValueOfColorOrCard != CloneOfHand.Cards[CardAnswer][0]){
			// error. at some point i do plan on going back and making it so that it'll not give an error and that it'll correct it by either not playing a card, or by playing a correct card
			LogErr(4);
		}
	}

	if(ColorOrCard == 2){
		Answer = -10;
		// return only card w/ the card stored in ValueOfColorOrCard
		for(int i = 0; i < CloneOfHand.Amount; i++){
			if(HeuristicValues[i] > Answer && ValueOfColorOrCard == CloneOfHand.Cards[i][1]){
				Answer = HeuristicValues[i];
				CardAnswer = i;
			}
		}

		if(Answer == -10){
			return 123; // 123 = none playable.
		}

		if(ValueOfColorOrCard != CloneOfHand.Cards[CardAnswer][1]){
			LogErr(4);
		}
	}

	// now that all cards have their value, we check what has the highest.

	if(ColorOrCard == 0){
		Answer = HeuristicValues[0];
		for(int i = 0; i < CloneOfHand.Amount; i++){
			if(HeuristicValues[i] > Answer){
				Answer = HeuristicValues[i];
				CardAnswer = i;
			}
		}
	}

    free(HeuristicValues);

	if(RequestedReturn == 1){
		return CardAnswer;
	}

	if(RequestedReturn == 0){
		return Answer;
	}
	
	return 10000; // this means strat isn't possible.
}

void LogErr(int err){
	// should've called it smth fancy like KeBugCheckEx-similar-ish smh
	if(LogOrNot == false){
		printf("Something inside this app has blown up and it needs to exit to prevent unexpected behaviour. Dumping a few things here:\n");
		if(err == 2){
			printf("Calling function: TestHeuristic\n"); // im fine with this being hardcoded rn
			printf("Error code: 2\nMALLOC FAILED.\n\n");
		}

		if(err == 3){
			printf("Calling function: GiveCard\n");
			printf("Error code: 3\nFAILED TO GIVE A VAILD CARD.\n\n");
		}

		if(err == 4){
			printf("Calling function: TestHeuristic\n");
			printf("Error code: 4\nCARD GIVEN HAS WRONG COLOR/CARD.\n\n");
		}

		// add other errors here.

		printf("App is exiting with code 1.\n");
		exit(1);
	}

    FILE *pLog = fopen("unoLog.txt", "a");
    if(pLog == NULL){
        printf("Something has blown up and the application needs to close to prevent unexpected behaviour.\n");
		perror("Opening the file has also gone disastrously wrong with error");

		if(err == 2){
			printf("The error was that malloc failed called from TestHeuristic (error code 2).\n");
		}

		if(err == 3){
			printf("The error was giving a card failed from GiveCard (error code 3).\n");
		}

		if(err == 4){
			printf("The error was that the card given was incorrect and would've caused an illegal move TestHeuristic (error code 4).\n");
		}

		printf("The app is exiting w/ error code 1.\n");
        exit(1);
    }
    
	printf("Something has blown up and the application needs to close to prevent unexpected behaviour. A log entry has been added.\n");
    
	fprintf(pLog, "\n\n<--FATAL ERROR-->\n");
	if(err == 2){
		fprintf(pLog, "ERROR 2: MALLOC FAILED.\n");
	}

	if(err == 3){
		fprintf(pLog, "ERROR 3: FAILED TO GIVE A CARD.\n");
	}

	if(err == 4){
		fprintf(pLog, "ERROR 4: CARD GIVEN HAS WRONG COLOR/CARD FROM TESTHEURISTIC.\n");
	}

	// add other errors here.

	fprintf(pLog, "Exiting with error code 1.\n");
    fclose(pLog);
	exit(1);
}

void MultipleCardsForComputer(int CurrentCard[2], Hand *Computer, int LastFourCards[4][2], Hand *User, int *pTurn, float Analyze){
	Hand Temp = *Computer;

	int OriginalCards[2];
	OriginalCards[0] = CurrentCard[0];
	OriginalCards[1] = CurrentCard[1];

	int CanMultipleCardsPlay = MultipleCardsPerTurn(OriginalCards, &Temp);
	int CanMultipleColorsPlay = MultipleColorsPerTurn(OriginalCards, &Temp);

	int ColorOrCard;

	if(CanMultipleCardsPlay != 0 && CanMultipleColorsPlay == 0){
		ColorOrCard = 0;
	}

	if(CanMultipleCardsPlay == 0 && CanMultipleColorsPlay != 0){
		ColorOrCard = 1;
	}

	if(CanMultipleCardsPlay != 0 && CanMultipleColorsPlay != 0){
		ColorOrCard = rand() % (1 - 0 + 1) + 0; 
	}

	int CardToPlay;
	int CanCardsBePlayed = 1;
	
	while(CanCardsBePlayed == 1){
		if(ColorOrCard == 0){
			int CardToPlay = TestHeuristic(0, 0, CurrentCard, Temp, Analyze, 1, 1, OriginalCards[0]);
		}

		if(ColorOrCard == 1){
			int CardToPlay = TestHeuristic(0, 0, CurrentCard, Temp, Analyze, 1, 2, OriginalCards[1]);
		}

		if(CardToPlay == 123){
			break;
		}
		CurrentCard[0] = Temp.Cards[CardToPlay][0];
		CurrentCard[1] = Temp.Cards[CardToPlay][1];
		printf("The computer played this card: ");
		Interpreter(Temp, CardToPlay, 0, 0);
		Temp.Amount--;
		Temp.Cards[CardToPlay][0] = 0;
		Temp.Cards[CardToPlay][1] = 0;
		FixCardsOld(Temp.Cards, Temp.Amount);
		MiddlePerson(1, User, &Temp, CurrentCard, CurrentCard, pTurn, Analyze);

		if(ColorOrCard == 0){
			if(MultipleCardsPerTurn(OriginalCards, &Temp) == 0){
				CanCardsBePlayed = 0;
				break; // just in case
			}
		}

		if(ColorOrCard == 1){
			if(MultipleColorsPerTurn(OriginalCards, &Temp) == 0){
				CanCardsBePlayed = 0;
				break; // just in case
			}
		}
	}

	*Computer = Temp;
}

void MultipleCards(int CurrentCard[2], Hand *User, int LastFourCards[4][2], Hand *Computer, int *pTurn, float Analyze){
	Hand Temp = *User;
	int ChosenCardToPlay = 0;
	int ChosenCardsToPlay[2] = {0};
	int CanCardsBePlayed = 1;

	printf("You can play more than one card here - would you like to? Type 1 for yes, type 0 for no: ");
	int Option = ValidateFTInput();
	if(Option == 0){
		return; // didn't know you could call return from void()
	}

	int OriginalCards[2];
	OriginalCards[0] = CurrentCard[0];
	OriginalCards[1] = CurrentCard[1]; // not doing anything fancy because it's just two indexes
	int ColorOrCard = 2; // basically, if the user chose to keep color the same, this'll be 1, otherwise 0. if 2, both are, it'll be decided again next time.

	if(Option == 1){
		while(CanCardsBePlayed == 1){
			Interpreter(Temp, 1000, 1, CurrentCard);
			AfterInterpreter:
			printf("\nPlay a card - type the number: ");
			scanf("%d", &ChosenCardToPlay);
			ChosenCardToPlay--;
			ChosenCardsToPlay[0] = Temp.Cards[ChosenCardToPlay][0];
			ChosenCardsToPlay[1] = Temp.Cards[ChosenCardToPlay][1];
			if(OriginalCards[0] == CurrentCard[0] && OriginalCards[1] != CurrentCard[1] && ColorOrCard == 2){
				ColorOrCard = 0;
			}

			if(OriginalCards[0] != CurrentCard[0] && OriginalCards[1] == CurrentCard[1] && ColorOrCard == 2){
				ColorOrCard = 1;
			}


				if(ColorOrCard == 0 && ChosenCardsToPlay[0] != OriginalCards[0]){
					printf("Redo. You chose to keep the card the same first time around, do the same here\n");
					goto AfterInterpreter;
				}

				if(ColorOrCard == 1 && ChosenCardsToPlay[1] != OriginalCards[1]){
					printf("Redo. You chose to keep the card the same first time around, do the same here\n");
					goto AfterInterpreter;
				}

				if(ChosenCardsToPlay[0] != CurrentCard[0] && ChosenCardsToPlay[1] != CurrentCard[1]){
					printf("Try choosing a card again. Remember, additional cards must have the same color or card as the original card\n");
					goto AfterInterpreter;
				}
				// card is fine. 
				printf("You've played this card: ");
				Interpreter(Temp, ChosenCardToPlay, 0, 0); // obviously this and the line above have to be before you remove the card.
				CurrentCard[0] = Temp.Cards[ChosenCardToPlay][0];
				CurrentCard[1] = Temp.Cards[ChosenCardToPlay][1];
				LastFourCards[3][0] = Temp.Cards[ChosenCardToPlay][0];
				LastFourCards[3][1] = Temp.Cards[ChosenCardToPlay][1];
				FixLastFew(LastFourCards);
				Temp.Amount--;
				Temp.Cards[ChosenCardToPlay][0] = 0;
				Temp.Cards[ChosenCardToPlay][1] = 0;
				FixCardsOld(Temp.Cards, Temp.Amount);
				MiddlePerson(0, &Temp, Computer, CurrentCard, CurrentCard, pTurn, Analyze);
				*User = Temp;
				if(ColorOrCard == 2){
					if(OriginalCards[0] == CurrentCard[0] && OriginalCards[1] != CurrentCard[1]){
						ColorOrCard = 0;
					}

					if(OriginalCards[0] != CurrentCard[0] && OriginalCards[1] == CurrentCard[1]){
						ColorOrCard = 1;
					}
			}
			if(ColorOrCard == 0){
				sleep(1);
				if(MultipleCardsPerTurn(OriginalCards, &Temp) == 0){
					printf("No more cards can be played.\n");
					return;
				}
			}

			if(ColorOrCard == 1){
				if(MultipleColorsPerTurn(OriginalCards, &Temp) == 0){
					sleep(1);
					printf("No more cards can be played.\n");
					return;
				}
			}

			if(ColorOrCard == 2){
				if(MultipleCardsPerTurn(OriginalCards, &Temp) == 0 && MultipleColorsPerTurn(OriginalCards, &Temp) == 0){
					sleep(1);
					printf("No more cards can be played.\n");
					return;
				}
			}

			if(MultipleCardsPerTurn(OriginalCards, &Temp) != 0 || MultipleColorsPerTurn(OriginalCards, &Temp) != 0){
				sleep(1);
				printf("You can play more than one card here again - would you like to? Type 1 for yes, type 0 for no: ");
				Option = ValidateFTInput();
				if(Option == 0){
					return;
				}
			}
		}
	}

}

int ValidateFTInput(){
	// this is only for 0/1, doing this because i had this tooo many times
	int temp;
	do{
		scanf("%d", &temp);
		printf("\n");

		if(temp != 1 && temp != 0){
			printf("You did not write a valid input - try again: ");
		}
	} while(temp != 0 && temp != 1);

	return temp;
}

int MultipleCardsPerTurn(int ChosenCards[2], Hand *User){
	if(ChosenCards[1] == 0){
		return 0; // you can't play two plus4/changecolor at once.
	}
	int Counter = 0; // number of cards that can be played based on what the player wanted to play.

	for(int i = 0; i < User->Amount; i++){
		if(User->Cards[i][0] == ChosenCards[0]){
			Counter++;
		}
	}

	return Counter; 
}

// whether or not a separate function is needed for colors, idk, but we're going w/ it.

int MultipleColorsPerTurn(int ChosenCards[2], Hand *User){
	if(ChosenCards[1] == 0){
		return 0; // you can't play two plus4/changecolor at once.
	}
	int Counter = 0; 

	for(int i = 0; i < User->Amount; i++){
		if(User->Cards[i][1] == ChosenCards[1]){
			Counter++;
		}
	}

	return Counter;
}


void FixLastFew(int LastFewCards[4][2]){
	LastFewCards[0][0] = LastFewCards[1][0];
	LastFewCards[0][1] = LastFewCards[1][1];
	LastFewCards[1][0] = LastFewCards[2][0];
	LastFewCards[1][1] = LastFewCards[2][1];
	LastFewCards[2][0] = LastFewCards[3][0];;
	LastFewCards[2][1] = LastFewCards[3][1];; 
}

float AnalyzePlayer(int LastFewCards[4][2], Hand ComputerStuff){
	// the major issue i'm seeing here is that this function doesn't include context. the player could've used all, say, plus2s because that's all theyu had that they could play
	// changing this to be a float - some real scary stuff here :fearful:

	float WhatToReturn = 0; // so. if this is a 0.5, the player did two plus's in a row. if it's a 1, they did three plus's in a row.
	bool IsStrategyPossible = false;

	if((LastFewCards[0][0] == 13 || LastFewCards[0][0] == 12 || LastFewCards[1][0] == 13 || LastFewCards[1][0] == 12) && (LastFewCards[1][0] == 13 || LastFewCards[1][0] == 12 || LastFewCards[2][0] == 13 || LastFewCards[2][0] == 12)){ // modify this, we want if 2/3 of the last 3 cards were plus2/plus4.
		/*for(int i = 0; i < ComputerStuff.Amount; i++){
			// we need to check if we even have a skip/reverse before doing anything else.

			if(ComputerStuff.Cards[i][0] == 11 || ComputerStuff.Cards[i][0] == 12){
				IsStrategyPossible = true;
				break;
			}
		} commenting this out for temp, because we can still play a changecolor/+4 i guess.*/ 
			for(int i = 0; i < ComputerStuff.Amount; i++){
					if(ComputerStuff.Cards[i][0] == 13 || ComputerStuff.Cards[i][0] == 14){
						WhatToReturn = 0.5;
						break;
					}
				}
				if(ComputerStuff.Amount > 10){
					WhatToReturn+=0.1;
				}

		return WhatToReturn;
	} // if a 0.5 is returned, there'll be a 25% chance that the computer will try to first do a skip/reverse, then a plus2/4. this will only work if the computer actually has one.
	// if 0.5, prioritize wild. otherwise prioritize a plus4 to give some cards to the player while we're slowly getting rid of our cards.
	// every three turns we recall this function. once a value is returned, that will stay for 3 turns.

	// the next thing, are they constantly changing color? if so.. then we do the same. also, we'll do this. if there's a +4/changecolor AND a color with only one card... that'll be the comobo
	// but won't that make us stuck? lol? maybe do if we have one spare.
	for(int i = 0; i < 3; i++){
		if(LastFewCards[0][1] != LastFewCards[1][1]){
			// again, this may be out of the user's control, but oh well.
			if(LastFewCards[1][1] != LastFewCards[2][1]){
				WhatToReturn+=1; // if WhatToReturn = 1, 50% chance that we will change the color
			}
		}
	}

	return WhatToReturn;
	// future ideas go here.

}

void Logging(FILE* path, Hand PlayerCards, Hand ComputerCards, int CurrentCard[2], int Turn, int TurnNum, float Analyze, int Last[3][2]){
	fprintf(path, "Turn number is %d\n", TurnNum);
	fprintf(path, "PlayerCards dump: ");
	for(int i = 0; i < PlayerCards.Amount; i++) {
		fprintf(path, "%d %d   ", PlayerCards.Cards[i][0], PlayerCards.Cards[i][1]); // good enough (not)
	}
	fprintf(path, "\nComCards dump: ");
	for(int i = 0; i < ComputerCards.Amount; i++) {
		fprintf(path, "%d %d   ", ComputerCards.Cards[i][0], ComputerCards.Cards[i][1]);
	}
	fprintf(path, "\nAmounts: %d %d", PlayerCards.Amount, ComputerCards.Amount);

	fprintf(path, "\nCurrentCard: %d %d", CurrentCard[0], CurrentCard[1]);
	fprintf(path, "\nTurn: %d", Turn);
	fprintf(path, "\nTurnNum: %d\n", TurnNum);
	fprintf(path, "LastFewCards: ");
	for(int i = 0; i < 3; i++) {
		fprintf(path, "%d %d   ", Last[i][0], Last[i][1]);
	}
	fprintf(path, "\nResultOfAnalyze: %.2f\n", Analyze);

	fprintf(path, "\n\n"); // just to make the ends a bit clearer.

}

void MiddlePerson(int Cop, Hand *Player, Hand *Computer, int CurrentCard[2], int ChosenCards[2], int *Turn, float Analyze){
	if(Cop == 0) {
		// this is it's the player's turn we're processing. we've already checked that the card is right prior to calling this func

		switch(ChosenCards[0]) {
		case 10:
			(*Turn) = ProcessSkip(0, (*Turn));
			break;

		case 11:
			(*Turn) = ProcessReverse(0, (*Turn));
			break;

		case 12:
			ProcessPlusTwo(0, Computer);
			// note to self. here, pass the Computer instead of &Computer - &Computer is a pointer to *Computer, since we're anyways passing a pointer to here
			// doing &Computer would make it a pointer to a pointer, we don't want that.
			break;

		case 13:
			ProcessPlusFour(0, CurrentCard, Player, Computer, Analyze, Turn);
			break;

		case 14:
			ProcessChangeColor(0, CurrentCard, Player, Analyze, Computer, Turn);
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
				ProcessPlusTwo(1, Player);
				break;

			case 13:
				ProcessPlusFour(1, CurrentCard, Player, Computer, Analyze, Turn);
				break;

			case 14:
				ProcessChangeColor(1, CurrentCard, Computer, Analyze, Player, Turn);
				break;

			default:
				break; // wonder if this is even needed
			}
		}
}

int FixCards(Hand MainHand){
	// the dreaded rewrite is here. this function is broken since i've moved to structs, and since i have no clue about what could possibly be wrong since the function seems alien despite me writing it, i'm going to rewrite it.
	// here's the thing: we don't ever remove more than one card at once - for now. even in change color etc, we remove the card first then call fixcards, then deal with the card played afterward
	// so it doesn't need to be so complex. i'm also unsure why we're even returning anything...? its not like hte number in Amount will be inaccurate. 
	// i also noticed a flaw: it checks values *against* Amount. but there's always cards ahead if you're bringing them back. this can be fixed by either doing Amount + 10 and it should be fine.. or.. remove from Amount after calling this functino, which we can go with.

	for(int i = 0; i < MainHand.Amount; i++){
		// actually, this is delayed. i could just bring the old function and make everything work around it. i feel like this is one of the worst things you could ever do in practise.. but oh well.
	}
}

int FixCardsOld(int Cards[100][2], int Amount) {
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

void ProcessPlusFour(int WhoMad, int CurrentCard[2], Hand *User0, Hand *User1, float Analyze, int *Turn){
	Hand TempUser0 = *User0;
	Hand TempUser1 = *User1;

	// this will sure be fun. i dont want this to be linked to main change color, so i'll have my own stuff here
	// funny to read this comment while performing minor rewrites, very funny to see i did just use the change color lol.

	if(WhoMad == 0) {
		printf("Four cards have been given to the computer\n");
		for(int i = 0; i < 4; i++) {
			TempUser1.Cards[TempUser1.Amount][0] = GiveCard();
			TempUser1.Cards[TempUser1.Amount][1] = GiveColor(TempUser1.Cards[TempUser1.Amount][0]);
			TempUser1.Amount++;
		}

		ProcessChangeColor(0, CurrentCard, &TempUser0, Analyze, &TempUser1, Turn);
		//passing othercards here as its the user's cards that matter here.

		*User0 = TempUser0;
		*User1 = TempUser1;
		// i don't think we have to do more here
	}


	if(WhoMad == 1) {
		printf("You have been given four cards. They are:\n");

		for(int i = 0; i < 4; i++) {
			TempUser0.Cards[TempUser0.Amount][0] = GiveCard();
			TempUser0.Cards[TempUser0.Amount][1] = GiveColor(TempUser0.Cards[TempUser0.Amount][0]);
			TempUser0.Amount++;

			Interpreter(TempUser0, TempUser0.Amount -1, 0, 0); 
		}
		
		// one thing i'm noticing, i want to come back to this. really i should set CurrentCard[0] and [1] to 99 or something to let the computer play *any* card.
		int ComputerCardChosen = ChooseComputerTurn(TempUser1.Cards, &TempUser1.Amount, CurrentCard, 0, 1, Analyze, *User1);
		CurrentCard[0] = TempUser1.Cards[ComputerCardChosen][0];
		CurrentCard[1] = TempUser1.Cards[ComputerCardChosen][1];

		printf("The computer played this card: ");
		Interpreter(TempUser1, ComputerCardChosen, 0, 0); // we'll just pass the card before removing it from the cmoputer *shrug*


		TempUser1.Amount--;
		TempUser1.Cards[ComputerCardChosen][0] = 0;
		TempUser1.Cards[ComputerCardChosen][1] = 0;  // that's it no?
		// aha, found the issue - FixCards needs to be called here
		FixCardsOld(TempUser1.Cards, TempUser1.Amount);
		MiddlePerson(1, &TempUser0, &TempUser1, CurrentCard, CurrentCard, Turn, Analyze);
		*User1 = TempUser1;
		*User0 = TempUser0; 
	}
}

void ProcessPlusTwo(int WhoMad, Hand *User){
	Hand TempUser = *User;
	if(WhoMad == 1) {
		printf("You have been given two cards as the computer played a Plus 2. They are:\n");
		for(int i = 0; i < 2; i++) {
			TempUser.Cards[TempUser.Amount][0] = GiveCard(); 
			TempUser.Cards[TempUser.Amount][1] = GiveColor(TempUser.Cards[TempUser.Amount][0]);
			TempUser.Amount++;
			Interpreter(TempUser, TempUser.Amount -1, 0, 0);
		}
		*User = TempUser; 
	}

	if(WhoMad == 0) {
		printf("The computer has been given two cards!!\n"); 
		for(int i = 0; i < 2; i++) {
			TempUser.Cards[TempUser.Amount][0] = GiveCard();
			TempUser.Cards[TempUser.Amount][1] = GiveColor(TempUser.Cards[TempUser.Amount][0]);
			TempUser.Amount++; 
			// the funny thing is, now that it's all in strcuts, really the for loop could be in common and the printf/interpreter in the whomad, but oh well. maybe later despite how easy it would be
		}
	}
	*User = TempUser;
}

void ProcessChangeColor(int WhoMad, int CurrentCard[2], Hand *User, float Analyze, Hand *OtherUser, int *Turn){
	Hand TempUser = *User;
	int ColorToChangeTo;
	int CardToChoose;
	//FILE *log = fopen("log.txt", "a");
	//fprintf(log, "%d %d %d %d %d %d %d", WhoMad, CurrentCard[0], CurrentCard[1], Cards[0][0], Cards[0][1], Amounts[0], Amounts[1]);
    
	if(WhoMad == 1) {// oh my days all along hte issue was the 1 and 0 was swapped...
		//we'll basically find a suitable card, play that, change the color to it. easy
		int ComputerCardChange = ChooseComputerTurn(TempUser.Cards, &TempUser.Amount, CurrentCard, 0, 1, Analyze, *User); //remember to remove change color from deck.
		InterpretColor(TempUser.Cards[ComputerCardChange][1]);
		printf("The computer has played the following card by changing the color: ");
		Interpreter(TempUser, ComputerCardChange, 0, 0);
		//IS THIS A BUG THAT WE'RE DOING IT TO COMPUTERCHARDCHANGE INSTEAD OF CARDTOCHOOSE [fixed]
		CurrentCard[0] = TempUser.Cards[ComputerCardChange][0];
		CurrentCard[1] = TempUser.Cards[ComputerCardChange][1];
		TempUser.Amount--;
		TempUser.Cards[ComputerCardChange][0] = 0;
		TempUser.Cards[ComputerCardChange][1] = 0;
		FixCardsOld(TempUser.Cards, TempUser.Amount);
		MiddlePerson(1, OtherUser, &TempUser, CurrentCard, CurrentCard, Turn, Analyze);
		*User = TempUser;
	} // much easier here because the computer won't put a wrong card, lol...
    
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

			for(int i = 0; i < TempUser.Amount; i++) {
				if(TempUser.Cards[i][1] == ColorToChangeTo) {
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
		Interpreter(TempUser, 1000, 0, 0);

		while(1) {

			scanf("%d", &CardToChoose);
			CardToChoose--; //since 1st card for user is 0th card in array

			if(TempUser.Cards[CardToChoose][1] != ColorToChangeTo) {
				printf("You chose an invalid card based on the color you chose - please choose again\n");
			}

			else {
				break;
			}
		}

			// now that a card has been chosen, we must change the current card, amount of cards, num. of cards

			CurrentCard[0] = TempUser.Cards[CardToChoose][0];
			CurrentCard[1] = TempUser.Cards[CardToChoose][1];
			TempUser.Amount--;
			TempUser.Cards[CardToChoose][0] = 0;
			TempUser.Cards[CardToChoose][1] = 0; // ig that's it - change color isn't being removed here as it may be needed
			FixCardsOld(TempUser.Cards, TempUser.Amount);
			MiddlePerson(0, &TempUser, OtherUser, CurrentCard, CurrentCard, Turn, Analyze);
			*User = TempUser;
		
	}
}

int ChooseComputerTurn(int Cards[100][2], int *Amounts, int CurrentCard[2], int IsItTheStart, int ChangeColor, float Analyze, Hand Computer) {
	// it really is begging for a rewrite - i'm passing Hand Computer just because TestHeuristic wants it, even though this function still uses hte old method from before structs

	int TempAmount = *Amounts;
	if(TempAmount == 2){
		for(int i = 0; i < 2; i++){
			if(Cards[i][0] == 13){
				return i;
			}
			if(Cards[i][0] == 14){
				return i; 
			}
		}
	}

	if(StrategyInProgress == true){
		// so we've already done the skip/reverse, now time to do the +4 or smth
		StrategyInProgress = false; // first thing is to set it to false, so the next time we call this it doesn't just do this.
		if(Analyze == 0.5){
		    for(int i = 0; i < TempAmount; i++){
		        if(Cards[i][0] == 14){
		            return i;
		        }
		    }
		    
		    for(int i = 0; i < TempAmount; i++){
		        if(Cards[i][0] == 13){
		            return i;
		        }
		    }
	}
	
	    if(Analyze == 0.6){
		    for(int i = 0; i < TempAmount; i++){
		        if(Cards[i][0] == 13){
		            return i;
		        }
		    }
		    
		    for(int i = 0; i < TempAmount; i++){
		        if(Cards[i][0] == 14){
		            return i;
		        }
		    }
	}
	}
	// this function ain't getting a rewrite to use structs. i don't know if it'll lead to a bug.. i hope not... but i want the function fully wokring first.
	if(IsItTheStart == 1) {
		// technically i can hardcode this as 7 because we know that there'll only be 7 cards at the start
		return rand() % (7 - 1 + 1) + 1;
	}

	if(ChangeColor == 1){
	    int NumOfColorOfCard[4] = {0}; // thsi is the number of cards that have a color
	    for(int i = 0; i < TempAmount; i++){
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
            for(int i = 0; i < TempAmount; i++){
                if(Cards[i][1] == test){
                    return i; // a card was found.
                }
            }

			// if we're here, this basically means that the only card left was the plus4/changecolor.. really, shouldn't be like this and i should make a functionl for it but uhh
	    }
	    
	}

	int BestCardToPlay = TestHeuristic(0, 0, CurrentCard, Computer, Analyze, 1, 0, 0);
	int BestCardToPlayValue = TestHeuristic(0, 0, CurrentCard, Computer, Analyze, 0, 0, 0);
	int IsStrategyDoable = TestHeuristic(0, 0, CurrentCard, Computer, Analyze, 2, 0, 0);
	// the 0 0 will go once i start doing BadCards, not rn.

	if(BestCardToPlayValue == 100 && BestCardToPlay == 100){
		// oops... how to get a new card in the middle of nowhere?
		// apparently you could do this, i thought this was asm-only lol

		goto OldMethod;
	}

	if(BestCardToPlayValue == 120){
		return BestCardToPlay; // only playable card.
	}

	if(BestCardToPlayValue != 120 && BestCardToPlayValue != 119 && BestCardToPlayValue != 100){
		// just in case
		if(IsStrategyDoable == 122){
			for(int i = 0; i < TempAmount; i++){
				if(CurrentCard[1] == Cards[i][1] && (Cards[i][0] == 10 || Cards[i][0] == 11)){
					// return a skip/reverse if its the same color
					StrategyInProgress = true;
					return i;
				}
			}
		}
		return BestCardToPlay;
	}

	// what's very funny is that the middle impl lasted for about five seconds. it is gone, honestly, kinda a shame it lasted for so little time.

	OldMethod:
	// below is the previous implementation. this is staying as a fallback if everything above goes wrong and doesn't work..
    // dont use amounts[0] lol
	while(1) {
		for(int i = 0; i < TempAmount; i++) {
			//randomly realized that with this method, if there's a plus 2 and you put a plus 2, it'd be two to you and two to PC rather than both to you.. hmm.. FIX THIS
			if(CurrentCard[0] == 12 && Cards[i][0] == 12) {
				return i;
			}
			// i also just realized, these will all have to be in separate for functions because otherwise it'd always do the first possible thing rather than what you want, hm
			// i wonder if this could be done from switch..? or nested for?
		}

		for(int i = 0; i < TempAmount; i++) {
			if(CurrentCard[0] == 12 && Cards[i][0] == 13) {
				return i; // if the user put a +2, we're going with a +4.. uh...
			}
		}

		for(int i = 0; i < TempAmount; i++) {
			if(CurrentCard[0] == 10 && Cards[i][0] == 10 || CurrentCard[0] == 11 && Cards[i][0] == 1) {
				return i; // if the user put a plus/skip... ig we put that too??
			}
		}

		// the issue i'm realizing here is that this will be super easy to predict....

		for(int i = 0; i < TempAmount; i++) {
			if(CurrentCard[0] == Cards[i][0] || CurrentCard[1] == Cards[i][1]) {
				return i;
			}
		}

		// if we're here, that means that the computer had no cards it could do - this means we keep on giving it new cards until we satisfy this.
		// but that means we shoulddo the whole thing in a loop...

		printf("The computer took a new card as it had no possible cards\n");
		Cards[TempAmount][0] = GiveCard();
		Cards[TempAmount][1] = GiveColor(Cards[TempAmount][0]); // i think this is fine.
		TempAmount++; // i just reailzed we're supposed to do this. bruh. i think it's been a month diff in between writing this and the rest...
		// okay wait i just realized. we're using tempamount everywhere but its not being updated because we don't update *Amounts before returning the val
		// but it shouldn't be too much fo an issue; i think this is the only place where we modify it.
		*Amounts = TempAmount;
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
	// MAJOR REWORK UNDER PROGRESS - CARDS GIVEN NOW ARE ACCURATE CHANCES.
	int Card = rand() % (26 - 0 + 1) + 0; // 10 = skip, 11 = reverse, 12 = plus2, 13 = plus4, 14=changecolor
	int WhatToReturn = 1000; 
	if(Card == 0){
		WhatToReturn = 0;
	}

	if(Card == 1 || Card == 2){
		// honestly, i could do this faster... but i'd take slower but actually work.
		WhatToReturn = 1;
	}

	if(Card == 3 || Card == 4){
		WhatToReturn = 2;
	}

	if(Card == 5 || Card == 6){
		WhatToReturn = 3;
	}

	if(Card == 7 || Card == 8){
		WhatToReturn = 4;
	}

	if(Card == 9 || Card == 10){
		WhatToReturn = 5;
	}

	if(Card == 11 || Card == 12){
		WhatToReturn = 6;
	}

	if(Card == 13 || Card == 14){
		WhatToReturn = 7;
	}

	if(Card == 15 || Card == 16){
		WhatToReturn = 8;
	}

	if(Card == 17 || Card == 18){
		WhatToReturn = 9;
	}

	if(Card == 19 || Card == 20){
		WhatToReturn = 10; //skip
	}

	if(Card == 21 || Card == 22){
		WhatToReturn = 11; // reverse
	}

	if(Card == 23 || Card == 24){
		WhatToReturn = 12; // +2
	}

	if(Card == 25){
		WhatToReturn = 13; // +4
	}

	if(Card == 26){
		WhatToReturn = 14;
	}

	if(WhatToReturn == 1000){
		// failed to give a card.
		LogErr(3);
	}

	return WhatToReturn;
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
		printf("Something went wrong. If a log file was generated, please send it on my GitHub.\n");
		printf("Exiting the app now.\n");
	}
}

void Interpreter(Hand User, int Card, int ShowCard, int CurrentCard[2]) {
	// i just love how this has stayed the same right from the start... so much stuff changed.. went from separate variables for colors and cards to using structs and yet this is still like this
	// i am thinking of using an array of strings and strcpy'ing it somewhere, maybe.
	if(Card == 1000) {
		printf("You have these cards:\n");
		for(int i = 0; i < User.Amount; i++) {
			printf("%d. ", i+1); // first time modifying this in years lol.
			switch(User.Cards[i][1]) {
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

			switch(User.Cards[i][0]) {
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
		switch(User.Cards[Card][1]) {
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

		switch(User.Cards[Card][0]) {
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
