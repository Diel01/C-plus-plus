#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();   

    // Get the words in the file HiddenWordList and put them in an array
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);

    Isograms = GetValidWords(Words);

    SetupGame();
}   

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{  
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();     
    }
    else
    {         
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{    
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; 
    Lives = HiddenWord.Len()*2;
    bGameOver = false;

    // Welcoming the player
    PrintLine(TEXT("Welcome to BullCowGame.")); 
    PrintLine(FString::Printf(TEXT("Guess the %i letter word!"), HiddenWord.Len() ));    
    //PrintLine(FString::Printf(TEXT("The HiddenWord is: %s."),  *HiddenWord)); //Debug Line
    PrintLine(FString::Printf(TEXT("You have %i attempts!"), HiddenWord.Len() ));    
    PrintLine(TEXT("Press TAB to access the terminal."));
    PrintLine(TEXT("Type your guess. \nPress ENTER to continue..."));
}   

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress ENTER to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& GuessInput)
{
    if (GuessInput == HiddenWord)    
        {
            ClearScreen();   
            PrintLine(TEXT("YOU HAVE WON!"));
            PrintLine(TEXT("\nThe hidden words was: %s"), *HiddenWord );
            EndGame();
            return;
        }    

    // Check right numbers of characters
    if (GuessInput.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Try guessing again!"));       
        PrintLine(FString::Printf(TEXT("The hidden word is %i letter long."), HiddenWord.Len() ));       
        return;             
    }   

    // Check if isogram
    if (!IsIsogram(GuessInput))
    {
         PrintLine(TEXT("No repeating letters, guess again!"));
         return;
    }
    
    // Remove life
    --Lives;
    PrintLine(TEXT("You lost a life. %u remaining"), Lives );  

    if (Lives == 0)
        {  
            // Game over screen      
            ClearScreen();    
            PrintLine(TEXT("GAME OVER\n"));   
            PrintLine(TEXT("You have no lives left."));
            PrintLine(TEXT("The hidden words was: %s"), *HiddenWord );
            EndGame(); 
            return;
        }

    FBullCowCount Score = GetBullCows(GuessInput);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
} 

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    // Loop through all characters
    for(int Index = 0; Index < Word.Len(); Index++)
    {
        // Compare the current character with others front of him
        for(int Current = Index+1; Current < Word.Len(); Current++)
        {
            if (Word[Current] == Word[Index])
            {
                return false;
            } 
        }                          
    }     
    return true;  
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordsList) const
{ 
    TArray<FString> ValidWords;

    for (FString Word : WordsList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 6)
        {
            if (IsIsogram(Word))
            {
                ValidWords.Emplace(Word);
            }            
        }
    } 
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 IndexCharacter = 0; IndexCharacter < Guess.Len(); IndexCharacter++)
        {
            if (Guess[GuessIndex] == HiddenWord[IndexCharacter])
            {
                Count.Cows++;
                break;
            } 
        }        
    }   
    return Count;
}