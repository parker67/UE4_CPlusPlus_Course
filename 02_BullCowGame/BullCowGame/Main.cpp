/* This is the console execuatable that makes use of the bull cow class
This acts as the view in a MVC pattern, and is responsible for all 
user interaction. 
For game logic see the FBullCowGame class.
*/


#include <iostream>;
#include <string>;
#include "FBullCowGame.h"

using FText = std::string;
using int32 = int;

void PrintIntro();
void PlayGame();
FText GetValidGuess();
bool AskToPlayAgain();
void PrintGameSummary();
FBullCowGame BCGame; //create an instance of a new game
//Main Entry for application

int main()
{
	bool bWantsToPlayAgain = false;
	do {
		PrintIntro();
		PlayGame();
		PrintGameSummary();
		bWantsToPlayAgain = AskToPlayAgain(); 
	} while (bWantsToPlayAgain);
	return 0;
}

void PlayGame()
{
	BCGame.Reset();
	int32 MaxTries = BCGame.GetMaxTries();

	//Loop asking for guesses while the game
	//is NOT won and there are still tries remaining
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries)
	{
		//getting guess
		FText Guess = GetValidGuess(); // TODO make loop checking validation
		//TODO submit valid guess to game
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		//echo guess back
		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << "Cows = " << BullCowCount.Cows << "\n\n";
	}
	// TODO add game summary
	return;
}

//loop continually until the user gives a valid guess
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {
		//Get a guess from player

		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries() << ". ";
		std::cout << "Enter your Guess: ";
		getline(std::cin, Guess);

		//check guess validity
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter all lowercase characters.\n\n";
			break;
		default:
			//assuming guess is valid as no errors
			break;
		}
	} while (Status != EGuessStatus::OK); //keep looping until no errors
	return Guess;
}

bool AskToPlayAgain()
{
	std::cout << "Do you want to play again with the same word? [Y/N] ";
	FText Response = "";
	getline(std::cin, Response);

	//return bool true if first letter is Y
	return (Response[0] == 'y') || (Response[0] == 'Y');
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "Congratuations you won! You guessed the word correctly!\n";
	}
	else {
		std::cout << "Better luck next time.\n";
	}
	return;
}

void PrintIntro()
{
	//print intro
	std::cout << "\n\nWelcome to Bulls and Cows.\n";
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength() << " letter isogram I'm thinking of?\n";
	std::cout << "An isogram is a word without a repeating letter. \n\n";
	std::cout << "How to Play: \n";
	std::cout << "1. Bulls - This indicates when a letter is in the word and in the right position. \n";
	std::cout << "2. Cows - This indicates when a letter is in the word but NOT in the right position. \n";
	std::cout << "3. Enter your guess and the game will respond with the number of bulls and cows.\n";
	std::cout << "           .............\n";
	std::cout << "       ....             ....\n";
	std::cout << "     ..                     ..\n";
	std::cout << "   ..                         ..\n";
	std::cout << "  .      ___           ___      .\n";
	std::cout << " .      / , \\         / , \\      .\n";
	std::cout << " .      \\___/         \\___/       .\n";
	std::cout << "..                                 .\n";
	std::cout << "..                                 .\n";
	std::cout << "..                O                .\n";
	std::cout << " .    |                     |    .\n";
	std::cout << " .     \\                   /     .\n";
	std::cout << "  .     \\                 /     .\n";
	std::cout << "   ..     \\______________/    ..\n";
	std::cout << "     ..    \\_____\\  \\  \\/   ..\n";
	std::cout << "       ....       |  \\  |....\n";
	std::cout << "           ...... |  |  |\n";
	std::cout << "                   \\___/\n";
	std::cout << std::endl;
}