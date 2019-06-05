/*
Program to read through Python programs and print out method, class, and decorator definitions.
Written by Bailie Livingston May 24th, 2019
*/
#include <stdio.h>

//Macros
#define FALSE 0
#define MAXLENGTH 10000
#define TRUE 1

//Functions
int bgetline(char s[], int putNl);

//Main takes a input and output filename as arguments
int main(int argc, char *argv[]){
    int classCount = 0;
    int decoratorCount = 0;
    int i;
    int methodCount = 0;
    int nl;
    char inputChar;
    char inputFileName[MAXLENGTH];
    char *outputFileName = "findMethodsOutput.txt";
    char fileline[MAXLENGTH];
    FILE *input;
    FILE *output;

    if ((argc == 2) && argv[1][1] == 'h')
    {
        printf("Usage: ./findMethods inputFilename outputFilename\nIf you do not supply an output filename, the output will go to standard output.\nReads through a Python program and prints the definitions of classes, decorators, and methods.\n");
    }

    //One or more arguments (besides program name) means input file
    if (argc > 1)
    {
        input = fopen(argv[1], "r");
        if (!input)
        {
            fprintf(stderr, "Error! File not found. Exiting.\n");
            return -1;
        }
    }
    //Two or more arguments besides program name means output file
    if (argc > 2)
    {
        output = fopen(argv[2], "w");
        outputFileName = argv[2];
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
            output = stdout;
        }
        else
        {
            printf("Please enter the output file's name: \n");
            i = bgetline(outputFileName, FALSE);
            output = fopen(outputFileName, "w");
        }
    }
    //One argument besides program name means they must not want to save to a file
    //This goes with the second if statement
    else
    {
        output = stdout;
    }



    //Loop through file and look for method definitions
    for (int iter = 0; iter < MAXLENGTH && (fgets(fileline, MAXLENGTH, input) != NULL); iter++){
        //Look at actual line of file
        for (int j = 0; j < MAXLENGTH && (fileline[j] != '\0'); j++){
            //Skip whitespace
            if (fileline[j] == 32 || fileline[j] == '\t')
            {
                continue;
            }
            //Look for '@' (only decorators start with that)
            else if (fileline[j] == '@')
            {
                fprintf(output, "%s", fileline);
                decoratorCount++;
            }
            //Look for the word "def" (Python methods declared with def methodName)
            else if (fileline[j] == 'd' && fileline[j + 1] == 'e' && fileline[j + 2] == 'f' && fileline[j + 3] == 32)
            {
                //Print any line with a method declaration
                fprintf(output, "%s", fileline);
                methodCount++;
            }
            //Look for the word "class"
            else if (fileline[j] == 'c' && fileline[j + 1] == 'l' && fileline[j + 2] == 'a' && fileline[j + 3] == 's' && fileline[j + 4] == 's')
            {
                fprintf(output, "%s", fileline);
                classCount++;
            }
            //Method/class definitions will always be the first item on that line
            else
                break;
        }
    }
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
