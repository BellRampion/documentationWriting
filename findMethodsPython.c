/*
Program to read through Python programs and print out method, class, and decorator definitions.
Written by Bailie Livingston May and July 2019
*/
#include <stdio.h>
#include <string.h>

//Macros
#define FALSE 0
#define MAXLENGTH 10000
#define TRUE 1

//Functions
int bgetline(char s[], int putNl);
int getFirstWord(char src[], char substr[], int max);

//Main takes a input and output filename as arguments
int main(int argc, char *argv[]){
    int classCount = 0;
    int decoratorCount = 0;
    int i;
    int methodCount = 0;
    int nl;
    int wordLen = 0;
    char firstWord[MAXLENGTH];
    char inputChar;
    char inputFileName[MAXLENGTH];
    char outputFileName[MAXLENGTH];
    char fileline[MAXLENGTH];
    FILE *input;
    FILE *output;

    if ((argc == 2) && argv[1][1] == 'h')
    {
        printf("Usage: ./findMethods inputFilename outputFilename\nIf you do not supply an output filename, the output will go to standard output.\nReads through a Python program and prints the definitions of classes, decorators, and methods.\n");
    }

    /***Set up files***/
    //**==
    //One or more arguments (besides program name) means input file
    if (argc > 1)
    {
        input = fopen(argv[1], "r");
        strncpy(inputFileName, argv[1], MAXLENGTH);
        if (!input)
        {
            fprintf(stderr, "Error! File not found. Exiting.\n");
            return -1;
        }
    }
    //Two or more arguments besides program name means output file
    if (argc > 2)
    {
        output = fopen(argv[2], "a");
        strncpy(outputFileName, argv[2], MAXLENGTH);
    }
    //No arguments, just the program name
    else if (argc == 1)
    {
        //Get input filename
        printf("Please enter the hard path of the file to read through (e.g. /home/myUsername/python/myPythonFile.py): \n");
        i = bgetline(inputFileName, FALSE);
        //Open file
        input = fopen(inputFileName, "r");

        //Ask if they want it saved to a file
        printf("Save output to file? (y/n) [y]: \n");
        inputChar = getchar();
        nl = (inputChar == '\n') ? 0 : getchar();

        //Open the output file or have output point to stdout
        if (inputChar == 'n')
        {
            input = stdout;
        }
        else
        {
            printf("Please enter the output file's name: \n");
            i = bgetline(outputFileName, FALSE);
            output = fopen(outputFileName, "a");
        }
    }
    //One argument besides program name means they must not want to save to a file
    //This goes with the second if statement
    else
    {
        output = stdout;
    }

    //==** End setting up files

    /*Print input filename to file*/
    fprintf(output, "From %s\n", inputFileName);

    /***Loop through file and look for definitions***/
    //**==
    for (int iter = 0; iter < MAXLENGTH && (fgets(fileline, MAXLENGTH, input) != NULL); iter++){
        //Get first word of file and add a space on the end (avoids false matches)
        wordLen = getFirstWord(fileline, firstWord, MAXLENGTH);
        if (wordLen < MAXLENGTH - 2)
        {
            firstWord[wordLen] = 32;
            firstWord[wordLen + 1] = '\0';
        }

        //Look for '@' (only decorators start with that)
        if (strchr(firstWord, '@') != NULL)
        {
            fprintf(output, "%s", fileline);
            decoratorCount++;
        }
        //Look for the word "def" (Python methods declared with def methodName)
        else if (strcmp(firstWord, "def ") == 0)
        {
            //Print any line with a method declaration
            fprintf(output, "%s", fileline);
            methodCount++;
        }
        //Look for the word "class"
        else if (strcmp(firstWord, "class ") == 0)
        {
            fprintf(output, "%s", fileline);
            classCount++;
        }
        else ;
    }
    //==** End looping through file
    //Print total number of methods and classes to file
    fprintf(output, "Total methods: %i\n", methodCount);
    fprintf(output, "Total classes: %i\n", classCount);
    fprintf(output, "Total decorators: %i\n", decoratorCount);

    fclose(input);
    fclose(output);
    //Print closing statement
    printf("Finished! Output is in %s.\n", outputFileName);

    return 0;
}

//Gets a line of input from console
int bgetline(char s[], int putNl){
	int i;
	int c;

	for (i=0; i<MAXLENGTH-1 && (c=getchar()) !='$' && c!='\n'; ++i){
		s[i] = c;
    }
	if ((c == '\n') && (putNl == 1))
	{
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	return i;
}

//Gets first word of a string and returns the word's length
int getFirstWord(char src[], char substr[], int max){
    int i, j;

    //Skip leading whitespace
    for (i = 0; i < max && (src[i]) != '\0'; i++){
        if (src[i] != 32 && src[i] != '\t')
            break;
    }


    for (i = i, j = 0; i < max && (src[i]) != '\0' && j < max; i++, j++){
        //If it's a space, dot, or EOL character, leave
        if (src[i] == 32 || src[i] == '.' || src[i] == '\n' || src[i] == '\r')
        {
            break;
        }
        else
        {
            //Copy it to the substring array
            substr[j] = src[i];
        }
    }
    substr[j] = '\0';
    return j;
}
