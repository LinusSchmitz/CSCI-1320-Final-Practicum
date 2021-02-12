#include<iostream>
#include<iomanip>
#include<ctime>
#include<string.h>
#include<math.h>
#include<fstream>
#include<cmath>
using namespace std;

double buildPopulation(ofstream &fout, string phrase, int phraseLength, string Words[]);
double calculateFitness( string phrase, string word, int wordLength);
string buildWord( int phraseLength);
char buildLetter();
char buildLetterLong();
double maxValue( double Array[], double maxEle);
int newFitness(double fitnessRating[], double maxNumb);
void raffleAssignment(int * raffle, double fitnessRating[], int raffleLength);
bool buildMatingPool(double fitnessRating[]);
string breed( string Words[], int * raffle, int raffleLength, int phraseLength);
string wordSelect( string Words[], int * raffle, int raffleLength);
string causeMutation(string wordToMutate, int howLong);
int maxFittnes(double newFitnessRating[]);
double avergeFitness( double newFitnessRating[]);
void file(ofstream &fout, int generation, double maxFitness, double avgFitness);

// Varibales which can be changed to see how that will change effientcy 
const int nLength = 200;
const int mutationRate = 25;    // 100/ mutationRate is the percent 
const int matingFactor = 10;
const int expoFactor = 1;
// If you would like to let the program to keep on running without a generation block 
//go to line 148 and comment out the if statement beggining there
const int maxGeneration = 2000;
// If you want to add all characters to the option then go to line 164 and 332 and change the 
//call from buildLetter to buildLetterLong

// If you want to change the breeding method go to line 276, and switch the commented block

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Above this is changeable constants, below is the code which should not be altered
//unless otherwise noted 

//variables which will be declared once and then never changed
string phrase;
int phraseLength;

// Variables which still have to be checked or assigned per function 
string word; 
int * raffle = NULL;            
double maxFitValue;
int raffleLength;

// This is a generation counter to see how long it takes for the word to converge
// It must be declared globally b/c it must be initiallized and then will be incramented in function.   
int generation =0;


//1.4
double newFitnessRating[nLength];
string newWords[nLength] = { };

int main (){
    srand(time(NULL));
    // Takes in a target phrase
    cout << "What is the phrase you want to go for?" << endl;
    getline(cin, phrase); 
    phraseLength = phrase.size();
    cout << "The length of the string is " << phraseLength << " Chars long" << endl;
    
    // Initialize nLength random strings all the same length 
    string Words[nLength];
    // Populate the string 
    for (int i = 0; i < nLength; i++){
        Words[i] = buildWord(phraseLength);
    }

    // The function buildPopulation is the primary caller of all other functions.
    // This operation will be repeated until the fitness value of some word in the function equals 1
    //this means that it has found a match  

    // Creates a new file, can be named all the same or indivdiualized per run.  
        // file pointer 
    ofstream fout; 
    fout.open("genData.csv"); 
    //fout.open(phrase + ".csv");

    do{
    maxFitValue = buildPopulation(fout, phrase, phraseLength, Words);
    } while (maxFitValue != 1);

    return 0;
}

// These first three function are for task 1.1
double buildPopulation (ofstream &fout, string phrase, int phraseLength, string Words[]){

    // This will calculate the fitness rating of each element in the aray 
    double fitnessRating[nLength] = { };
    for (int i = 0; i<nLength; i++ ){
        fitnessRating[i] = calculateFitness(phrase, Words[i], phraseLength );
    }

    // intializeing a counter as well as the random varible which will be used in the mutation function 
    int nMutations = 0;
    const int iLikeSeventeen = 17;  

    // This is if the buildMatingPool fuction can not be properlly run because an array is set to length zero
    // This is an absurd condition so everything gets mutated
    if ( !buildMatingPool( fitnessRating)){
        for (int i= 0; i < nLength; i++){
            newWords[i] = causeMutation( newWords[i], phraseLength);
            nMutations++;
        }
        cout << "Because of an error in the raffle length, the random number generator had to run more often" << endl;
    }
    else{
    // mutatate about four percet of the time
    for (int i= 0; i < nLength; i++){
        newWords[i] = breed(Words, raffle, raffleLength, phraseLength);
        // 1.5
        if ((rand()% mutationRate) == iLikeSeventeen){    //This should be true four percent of the time
            newWords[i] = causeMutation( newWords[i], phraseLength);
            nMutations++;
        }
    }
    }

    //Repopulating the fitness array with the next gen scores 
    for (int i = 0; i<nLength; i++ ){
        newFitnessRating[i] = calculateFitness(phrase, newWords[i], phraseLength );
    } 
    
    // Making sure that all of the variales make sense and then casting anything around as needed
    //to make sure that it is able to output properly 
    int maxFitIndex = maxFittnes(newFitnessRating);
    double maxFitValue = newFitnessRating[maxFitIndex];
    cout << "The maximum fitnessScore for generation " << generation << " is: "<< maxFitValue << " and the word is: " << newWords[maxFitIndex] << endl;
    generation++;

    // Figure out the average fitness 
    double avgFitness = avergeFitness( newFitnessRating);
    
    // Write out data about each generation to a csv file 
    file(fout, generation, maxFitValue, avgFitness); 

    // Copy all the newWords into Words
    for (int i = 0; i < nLength; i++){
        Words[i] = newWords[i];
    }

    /*if (generation >= maxGeneration){
        cout << " The maximum generation of " << maxGeneration << " was reached before the program completed"<< endl;
        return 1;
    }*/

    return maxFitValue;

}

string buildWord( int phraseLength){                        // USed in buildPopulation 
    // This fucntion is used to building or creating the initial generation of random words
    //i don't think that it is called again later in the program as afterwards the only chage 
    //to words is in the mutation function.
    word.clear();
    for ( int i = 0; i < phraseLength; i++){
        word = word + buildLetter();
    }
    return word;
}

char buildLetter(){                                         // Used in buildWord
    // This function creates a single random character within the bounds A-z and space
    // Is called for building the words as well as for mutating indevidual letters 
    int numb = 0;
    char ch = ' ';
        numb = rand()% 53; 
        if ( numb < 26){
            ch = 'A' + numb; 
        }else if (numb < 52){
            numb = numb - 26;
            ch = 'a' + numb;
        }
        // Else if ch is greater than 52 will be initialized to space 
        else{      
            ch = ' ';
        }
        return ch;
        
}

// This is for task 1.2 
 
double calculateFitness( string phrase, string word, int phraseLength){            // Used in buildPopulation
    //This will have to be assigned to a new array which is also nLength in length 
    // This will calculate the correct percentage of each word. Needs to be called for each word
    double good = 0;
    double perGood;
    for (int i = 0; i < phraseLength; i++){
        if ( phrase.at(i) == word.at(i)){
            good = good + 1;
        }
    }
    perGood = good / phraseLength;
    // cout << " The correct percentage of the word is:  " <<  perGood << endl;
    perGood = pow(perGood, expoFactor);
    return perGood;
} 

// This is for task 1.3 

bool buildMatingPool( double fitnessRating[]){
    // find the max value and then devide everything by the max value (normalizing )
        // multiply by ten and round
    // sum up all success rates 
    // max an array for the raffle tickets where the values in that array are the indecies in the (success rate and random string arrays)
    // normalize 
        raffleLength = 0;
        double maxEle = -1;
        double maxNumb = maxValue(fitnessRating, maxEle);
        raffleLength = newFitness(fitnessRating, maxNumb);
        if (raffleLength == 0 ){
            raffle = NULL;
            return false;
        }
        raffle = new int[raffleLength];
        raffleAssignment(raffle, fitnessRating, raffleLength );

        return true;
}
 
double maxValue( double Array[], double maxEle){
    // This finds the max vlaue of the fitness array
    for ( int i = 0; i< nLength; i++){
        if (Array[i]> maxEle) {
            maxEle = Array[i];
        }
    }
    return maxEle;
}

int newFitness( double fitnessRating[], double maxNumb){
    // The devides all the values in the fitness aray by the max value and then automatically multiplies them by 10 and rounds it 
    // Then adds up all the values to determine what the length of the raffle array will be 
    for (int i = 0; i< nLength; i++){
        fitnessRating[i] = fitnessRating[i] / maxNumb;
        fitnessRating[i] = round(fitnessRating[i] * matingFactor);
        raffleLength = raffleLength + fitnessRating[i];
    }
    return raffleLength;
}

void raffleAssignment(int * raffle, double fitnessRating[], int raffleLength){
    // This goes through and populates the raffle array as needed so that it coreseponds with the 
    //correct word's index 
    int indexEnd = 0;
    for (int i = 0; i < nLength; i++){
        int indexNums = fitnessRating[i];
        int indexStart = indexEnd;
        indexEnd = indexEnd + indexNums;
        for ( int j = indexStart; j < indexEnd; j++){
            raffle[j] = i;
        }
    } 
}

//task 1.5 

string wordSelect( string Words[], int * raffle, int raffleLength){
    // This fucntion randomly chooses a word which will be used in the breed function 
    // it is called twice per new word
    string wordOut;
    wordOut.clear();
    int pointer = rand()% raffleLength;
    int value = raffle[pointer];
    wordOut = Words[value];
    return wordOut;
}

string breed( string Words[], int * raffle, int raffleLength, int phraseLength){
    // This is where two words are combined together to (hopfully) create a fitter one
    // which will then continue on to the next generation     
    
    string word1 = wordSelect(Words, raffle, raffleLength);
    string word2 = wordSelect(Words, raffle, raffleLength);
    
    //This is the alternative method for breeding two words together 
    
    /*
    string newWord;
    newWord.clear();
    int midpoint = floor(phraseLength/2);
    for (int i=0; i < midpoint; i++){
        newWord = newWord + word1.at(i);
    }
    for (int i= midpoint;  i < phraseLength; i++ ){
        newWord = newWord + word2.at(i);
    }
    return newWord;

    */
    // This is the second method 
    
    // This restriction on the rand is to make sure that we are getting at least one letter from each word
    int nCharsToMatch = 1 + (rand()% (phraseLength-2));  
    char newWord[phraseLength +1] = {0};            // Ensure room for a terminator 
    int nCharsMatched = 0;
    while( nCharsMatched < nCharsToMatch ){
        int currentLetter = rand()% (phraseLength - 1);     //Because the currentLetter must be an index, then it must range from zero to one less than the length cause length is how many indecies there are
        if ( newWord[currentLetter] == 0 ){
            newWord[currentLetter] = word1.at(currentLetter);
            nCharsMatched++;
        }
    }
    nCharsMatched = 0;
    nCharsToMatch = phraseLength - nCharsToMatch;
    while( nCharsMatched < nCharsToMatch ){
        // now just go fill in the missing letters from the second word
        for ( int i = 0; i < phraseLength; i++){
            if ( newWord[i] == 0 ){
            newWord[i] = word2.at(i);
            nCharsMatched++;
        }
        }
    }
    string returnString(newWord);   
    return returnString;
    
}

string causeMutation(string wordToMutate, int howLong){
    // For the four percent which the word will be mutated this is the mutation function
    //it randomly chooses a letter in the word and then randomly chooses a letter to replace 
    //that with
    int letterToMutate = rand()% howLong;
    char mutatedLetter = buildLetter();
    wordToMutate.at(letterToMutate) = mutatedLetter;
    return wordToMutate;
}

int maxFittnes(double newFitnessRating[]){
    // This function runs through all of the fitness of the generation and checks 
    //to figure out which one is the most fit and then output the index of that one
    //so the word (which is in a global array) can be couted with its fitness score
    double maxFit = 0;
    int maxFitIndex = 0;
    for (int i = 0; i < nLength; i++){
        if (newFitnessRating[i] > maxFit){
            maxFit = newFitnessRating[i];
            maxFitIndex = i;
        }
    }
    return maxFitIndex;
}

double avergeFitness( double newFitnessRating[]){
    //This is used to calculate the average fitness of all of the words per generation
    double fitnessTotal = 0;
    for (int i = 0; i< nLength; i++){
        fitnessTotal = fitnessTotal + newFitnessRating[i];
    }
    double avgFitness = fitnessTotal / nLength;
    return avgFitness;
}

void file(ofstream &fout, int generation, double maxFitness, double avgFitness) 
{   
    // This is used to fill in the csv file
    fout << generation << "," << maxFitness << "," << avgFitness << endl;
}

char buildLetterLong(){                                         // Used in buildWord
    // This function creates a single random character within the bounds A-z and space
    // Is called for building the words as well as for mutating indevidual letters 
    int numb = 0;
    char ch = ' ';
        numb = rand()% 84; 
        if ( numb < 26){
            ch = 'A' + numb; 
        }else if (numb < 52){
            numb = numb - 26;
            ch = 'a' + numb;
        }
        // Else if ch is greater than 52 will be initialized to space 
        else{     
            numb = numb - 52; 
            ch = ' ' + numb;
        }
        return ch;
        
}
