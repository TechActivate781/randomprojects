# Introduction
This is a simple game of UNO. If you've never heard of it before, you can see some information about the game [here](https://en.wikipedia.org/wiki/Uno_(card_game)). This project is a CLI implementation of the game.

> [!WARNING]
> This app currently contains a bug where the computer gets stuck playing a turn. Currently, the cause is unknown. It is being investigated.

# Breakdown of functions excluding main()
This section talks (is that the right word?) about the features in the app. Every function will be documented here. The main function is documented at the bottom of this document.

> [!NOTE]
> The documentation frequently talks about certain numbers as card. In the app, the number 10 is a Skip Turn card, while 11 is a Reverse, 12 is a Draw 2, 13 is a Draw 4, and 14 is a Wild (a.k.a Change Color). The app stores all cards in a 2D array called ```Cards``` (which itself is in a struct); the first array stores the card, while the second stores the color. The color is a number from 0-4. Zero is only used in Plus4/Change Color, while numbers 1, 2, 3, and 4 are red, yellow, green, and blue, respectively. 

## Logging
To help in debugging (and partially because it's cool), this function logs the events of the game to a file, ```newUno.txt```. This is only done <ins>after getting permission from the user</ins> (the user has an option to deny making a log). When a log is generated, it looks something like this:
```
UNO v0.9 Release; Developed by techactivate781~ :3. System date and time: Fri Oct 16 18:46:15 2026
Current Operating System: Win64 
Please do not cheat by reading the values of the computer.
<----- Starting uno session; preparing game ----->
Making the move to using structs [DONE]
[NOTE] Computer vs Computer mode chosen
[NOTE] This game mode is still in alpha. Expect bugs. Treat all player values as those of the computer.
Creating the hands for the player and computer [DONE]
Creating Turn, Amounts, CurrentCard, ChangeColor, ChosenColor, and TurnNum variables [DONE]
Giving a card to all cards [DONE]
Giving a color to all cards [DONE]

<----- Starting uno game ----->
Turn number is 0
PlayerCards dump: 12 2   2 1   6 4   0 1   0 1   7 1   14 0   
ComCards dump: 12 3   1 3   8 4   12 2   9 1   2 1   1 1   
Amounts: 7 7
CurrentCard: 0 0
Turn: 0
TurnNum: 0
LastFewCards: 0 0   0 0   0 0   
ResultOfAnalyze: 0.00
```

The initial section contains the version number, system date/time, and the detected operating system (this may fail and not have an output if not run on Linux, Windows, or FreeBSD). Keep in mind that the initial part of the long, where the game is being prepared, is (as of version 0.9.1) **hardcoded**. This means that if one of the operations listed fail, the log will still believe that they were successful. Do not rely on these for early debugging.

The later part of the log, where values are dumped, is shown below.

### Second section
Once the game is started, a typical log at a specific turn will contain various information about the turn. A dump of various variables are made; ```PlayerCards``` are simply what cards the player possesses. Each card has two values - the card number and the card color. The number is **always** the first detail shown. In the above log, the first card in ```Player.Card``` was "12 2". The number 12, in this case, simply means a +2. The number 2 is the color, which means it is a yellow card. More information about cards/colors is provided at the top.

Additionally, values called "```Amounts```" are shown. This is the number of cards that are present in the array (as in the array storing the cards) for either the player or the computer. It is important to note that the first index is actually something like ```Player.Cards[0][0]```, so to access the 7th card's color, you would need something like ```Player.Cards[6][1]```.

```CurrentCard``` simply holds the current card in the above format. ```Turn``` is the turn number of the game (the number of turns that have elapsed since the start), while ```TurnNum``` decides who's turn it is - the player's or the computer's (or otherwise depending on the chosen game mode). ```LastFewCards``` contains the last few cards played by the player for analyzing them (more information below), while ```ResultOfAnalyze``` is the result of this.

A log can also be made on-demand by typing 999 when asked to play a card _in the main screen_. This does not work when playing more than one card at a time, or in a Plus 4 or Change Color.

## LogErr
In the event of a serious error, the function ```LogErr``` will be asked to exit the app after informing the user about the error. If the user chose to log at the start of the game, an attempt will be made to write to said log; however, if unable to, details will be dumped on screen. A log entry will be made if possible, and if the user chose not to make a log, the following information will be dumped to the screen:
* The calling function
* The error code
* The reason for said error

Currently, as of version 0.9.1, only 3 errors are implemented:
- Error 2 is given if dynamic memory allocation failed from ```TestHeuristic``` when checking what card the computer should play
- Error 3 is given if a card number was not given correctly, called from ```GiveCard```
- Error 4 is given if the card the computer chose to play was not a legal card. A plan has been made in future versions to phase this out and to instead try to give a card again

The application exits with error code 1 after either writing to the log or the terminal.

## ValidateFTInput
This function allows values of 1 or 0 to be easily accepted from the user, which is then returned by the function. Values that are not 0 or 1 are rejected until a value that is either a 0 or 1 is entered.

## GiveCard
This function gives a card. The giving of the card is pseudo-random; the time is used as a seed. Cards are given based on the probability of the card being given in a real UNO deck. Cards 1-13 have the same chance of appearing, which is 1/13 each. Plus 4, Change Color, and the card of number "0" all have a 1/26 chance of being given.

## GiveColor
This function simply gives a color to the card. If the card of 13 or 14 is there, it will return 0. Otherwise, a pseudo-random number from 1-4 is returned.

## Interpreter
This isn't really what an interpreter is, I think. However, this function simply allows the user to see all of his/her/their cards. It tells the user what cards they have. The ```ShowCard``` variable that is passed to it is simply a 1/0 to show what the current card is after showing the user their cards.

## InterpretColor
A simple function to show what the color has been changed to, used in ```ProcessPlusFour``` and ```ProcessChangeColor```.

## ProcessSkip
> [!NOTE]
> Many functions take an argument called "```WhoMad```". When this value is 0, the computer is the one playing said card. If it is one, then the computer is playing it.

This function simply informs the user if they skipped a turn/if their turn was skipped. It then returns the opposite of the Turn; the turn is flipped.

## ProcessReverse
Same as ```ProcessSkip```, except it references it as a reverse being played instead of a skip turn.

## ProcessPlusTwo
This function gives two cards to either the computer or player, depending on the value of ```WhoMad```. If the player received the cards, the user is informed about which cards they have received. 

## ProcessPlusFour
If ```WhoMad``` is 0 (the player played the card), they are informed that the computer has been given four cards and are able to play a card of a new color. When ```WhoMad``` is 1 (the computer played the card), the computer plays a card. The player is then informed about what four cards they were given and what card the computer played.

## ProcessChangeColor
When ```WhoMad``` is 1, the computer simply plays a card. The player is informed about which color the computer changed it to, and what card it played. When ```WhoMad``` is 0, the player is asked to change the color. If the player has no cards of that color, it will not allow the player to change the color to that color. Once an applicable card has been chosen, it will ask the user to play a card from that color.

## MiddlePerson
This function accepts just about every variable. When the player/computer play a card, it will then call the appropriate function for the card if it is special.

## FixCards
This function is incomplete.

## FixCardsOld
This is the previous implementation of ```FixCards``` and thus accepts ```Cards[100][2]``` and an int called Amount. This is needed because if the card stored in the index of the array is played, it will become empty. This function moves all cards back by the amount needed to make sure the app runs correctly. 

## MultipleCardsPerTurn
This function simply accepts CurrentCard and a Hand. It then returns a non-zero value (the value is the number of cards that can be played at once that have the same card number) or a value of 0 if no other cards can be played at that moment that have the same card.

## MultipleColorsPerTurn
This function is similar to the above function, except it checks if more than one card can be played that has the same color.

## CheckIfCardIsRight
When a player plays a card, this function checks if the card is legal or not by checking it against ```CurrentCard```. For a card to be accepted, it needs to be either a Plus 4/Change Color, or have the card or color in common with ```CurrentCard```.

## MultipleCards
This function allows the user to play more than one card at a single turn if applicable. When (that is, if they choose to) the player chooses a card to play, it checks whether or not the cards are similar in terms of card or color. This is then used to make sure that the player does not first play a card which has the same color as the original card, followed by a card which has the same card but different color as the original card.

The user is given a choice to play more cards at the turn if they have the available cards to do so.

## MultipleCardsForComputer
This function allows the computer to play more than one card at a single turn. 

## ChooseComputerTurn
### Previous implementation
> [!IMPORTANT]
> This implementation is not used by default and is instead kept as a fallback in case the new implementation fails, or if the computer does not have a card to play.

The previous implementation, written under ```OldMethod``` in the code, was very simple. It first puts a Draw 2 card if the user played that, then a Draw 4 if a Draw 2 is unavailable. If the user played a Skip Turn or Reverse, it will attempt to play a skip turn or reverse. Finally, if none of the above works, it will play a card which has the same card/color as the current card. These are done in order - if a Plus 2 or Skip Turn is available, the former will be chosen.

If no available cards are playable, it will give a card, then repeat the process.

### Current Implementation
> [!NOTE]
> This part relies heavily on the function TestHeuristic. It is advisable to read that part of the documentation to understand how this works.

If the computer only has two cards, and one of them are a Plus 4 or Change Color, the computer will play that. After that, it will check if a strategy is in progress by using ```bool StrategyInProgress```. If it is in progress, it will attempt to play a Plus 4 first, followed by a Plus 2 if unavailable. 

An int called ```IsItTheStart``` is passed to the function. If this is 1, it is assumed that the game has just begun, and since the computer plays first, **any** card is played. 

An int called ```ChangeColor``` is also passed if ChooseComputerTurn is called from ChangeColor; if this is 1, the computer will randomly find a card to play (based on any colors that the computer has), and that card will be returned.

Various values from TestHeuristic are returned. If a strategy is possible to be started, it is started by playing either a reverse or a skip turn (a strategy is counted as a two-turn move where the computer first plays a Skip Turn/Reverse followed by a Plus 2, Plus 4, or Change Color). Otherwise, the best card to play (which is obtained from the TestHeuristic function) is played.

If no cards can be played, the computer will ```goto``` to the previous implementation.

## AnalyzePlayer
This function makes an attempt into analyzing the player's moves. It is passed ```int LastFourCards[4][2]```, an array that contains the last three cards the player played. If it is noticed that the player is frequently playing cards that are either a Plus 2 or Plus 4, the computer will make an attempt to play a similar card. If the computer has more than 10 cards, it will prioritize a Plus 4 over a Change Color. Additionally, the probability that the color is changed is increased if the player is often changing cards.

It is worth noting that this function does not include context - for instance, it is possible that the player was switching cards often because they were the only playable legal cards.

This function can return a few values:
- If the player is frequently playing Plus 2/Plus 4 values, and we have a Plus 4/Change color, we have the value of 0.5
- This can be increased by 0.1 to 0.6 <ins>if the computer has more than ten cards</ins> (to get rid of the cards)
- If the player changed the current color often, a 1 is returned

## FixLastFew
This function simply keeps the last three cards the player played. It gets rid of the fourth latest in favor of the latest and moves every card behind by one in the array of LastFewCards (often also called LastFourCards in the program; although it stores 3 cards in practice. It is called LastFourCards as the latest card is temporarily stored in the 4th element/3rd index prior to being moved back).

## TestHeuristic
The idea of this function is that every card is given a value/weighting of how much it will affect the current game. The function starts by dynamically assigning memory to an array (which, if failed, will call ```LogErr```). Once all values are set to zero, it quickly checks the number of cards that are of a certain card or color. This is stored in two arrays that are declared as:

```
int NumOfAllColors[5] = {0};
int NumOfAllCards[15] = {0};
```

For either array, the format is the same; NumOfAllCards[12] would contain the number of Plus 2 cards that the computer has. A separate array stores the total number of 13/14 cards that the computer has. 

Finally, the computer assigns a weight to each available card. The rules are as follows:
- If the card can't be legally played, a value of 119 is assigned
- If the color can be changed legally, +2 to the current value
- If the card can be changed legally, +2 to the current value
- If changing the color will allow an alignment to the strategy, another +1
- If the card is the last of its card, +2
- If the card is the last of its color, +3
- If there is only one card of the color we are changing the color to and there aren't any Plus 4/Change color, -5 (assuming its the color being changed)
- If there is only one card of the card we are changing the card to and there aren't any Plus 4/Change Color, -3 (assuming its the card being changed)
- If the value returned from ```AnalyzePlayer``` is 0.5 or 0.6, and we have a Reverse/Skip Turn is present, +5
- If the value returned from ```AnalyzePlayer``` is 0.5 or 0.6, but there aren't any Reverse/Skip Turn present, +3
- If the value returned from ```AnalyzePlayer``` is 0.6, +4
- If only one card can be played, that card has the value of 120

### Values returned
The function can return many different values. A value of 100 returned means that a new card is needed, while a value of 122 means to start a strategy (this is returned if the ```int ColorOrCard``` argument passed to this function when called is 2). A value of 123 is returned if no playable cards are present (is this needed -- isn't 100 for that?) and a value of 10,000 is returned if starting a strategy is not possible.

When passing values to this function, the value passed for ```RequestedReturn``` chooses _how_ the value is returned. If it is a 1, then the <ins>card with the highest value is returned</ins> (internally called ```CardAnswer```). If it is a 0, then the <ins>value itself is returned</ins>. This can also be set to 2; when done, the function will only check whether or not a strategy is possible. If so, a value of 122 is returned. If a strategy can't be started, it returns a value of 10,000. 

The ```ChooseComputerTurn``` function then uses this value as part of deciding what card to play.

# The main function
When the application is started, it aims to detect what operating system the application is being run on. This is included in the log (provided that the user agrees to create one). The user is then given an option to create a log file. If they decide to, an option is presented to either overwrite an existing file or append to it. The file is opened at the start of the program to test if opening the file is successful; if not, it will give an error.

Options are then presented to either start a game where two players play against each other (which, as of writing, is not implemented), or a game mode where two computers play against each either (declining both of these options will result in the typical game of a computer against a player).

For readability purposes, the terminal is then cleared (at least, on Windows/Linux hosts). The game then starts, with the computer playing the first turn (provided that it is a computer v player game). Once the game is finished, the winner is announced.

