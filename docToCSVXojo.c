/* COPYRIGHT AND LICENSE
Copyright 2019 by Bailie Livingston.
This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/* DESCRIPTION
Program to take an output file listing class, method, window, module, interface, and sub declarations and create a CSV file listing the filepath, class/interface/module/window name, method/sub/function name, parameters, and return type.
Written by Bailie Livingston July 2019
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Macros
#define FALSE 0
#define MAXLENGTH 10000
#define TRUE 1

//Functions
int copySubStr(char src[], char dest[], int srcStart, int srcEnd, int maxLen);
int indexOfStr(char mainStr[], char subStr[]);
int pairMarkers(int openIndex, char closingChar, char src[]);

//Argv[1] is the input file, argv[2] is the output file
int main(int argc, char *argv[]){
    char location[MAXLENGTH];
    char inputline[MAXLENGTH];
    char outputline[MAXLENGTH];
    char *result;
    char tempArray[MAXLENGTH];
    int iter = 0;
    int startIndex = 0;
    int endIndex = 0;
    int tempLength;

    FILE *inputfile;
    FILE *outputfile;
    FILE *discardFile;

    location[0] = '\0';

    if (argc < 3)
    {
        printf("Usage: docToCSV inputfilename outputfilename\nThe outputfile will be appended to.\n");
        return 1;
    }

    //Open files
    inputfile = fopen(argv[1], "r");
    outputfile = fopen(argv[2], "a");
    discardFile = fopen("CSVXojoDiscards.txt", "a");
    if (inputfile == NULL)
    {
        fprintf(stderr, "ERROR: file %s not found. Exiting.\n", argv[1]);
        return -1;
    }
    //Shouldn't happen, but just in case
    if (outputfile == NULL)
    {
        fprintf(stderr, "ERROR: file %s not found. Exiting.\n", argv[2]);
        return -1;
    }

    //Copy input filepath to location array
    strncpy(location, argv[1], MAXLENGTH);

    //Write initial line to discards file
    fprintf(discardFile, "From %s\n", location);

    //Parse fileline
    while (fgets(inputline, MAXLENGTH, inputfile) != NULL){

        //Discard lines starting with "Soft Declare"
        if (strstr(inputline, "Soft Declare") != NULL || strstr(inputline, "soft Declare") != NULL)
        {
            fprintf(discardFile, "%s\n", inputline);
            continue;
        }
        //Check to see if this line has a filename on it
        if ((result = strstr(inputline, "From ")) != NULL)
        {
            //Make sure that there is not a character other than a space right before the keyword
            startIndex = result - inputline;
            if (startIndex == 0 || !isalpha(inputline[startIndex - 1]) || inputline[startIndex - 1] == 32)
            {
                //Get rest of string
                result += strlen("From ");
                strncpy(location, result, MAXLENGTH);
                printf("Location: %s\n", location);
                //Remove the newline or return
                if (location[strlen(location) - 2] == '\r')
                {
                    location[strlen(location) - 2] = '\0';
                }
                else if (location[strlen(location) - 1] == '\n')
                {
                    location[strlen(location) - 1] = '\0';
                }
                else ;
            } //End checking for being part of another word
            else fprintf(discardFile, "%s", inputline);
        }
        //Check to see if this line has a class or module definition
        if ((result = strstr(inputline, "Class ")) != NULL)
        {
            //Make sure that there is not a character other than a space before the keyword
            startIndex = result - inputline;
            if (startIndex == 0 || !isalpha(inputline[startIndex - 1]) || inputline[startIndex - 1] == 32)
            {
                //Get rest of string and add to location array
                result += strlen("Class ");
                strncat(location, "/", MAXLENGTH - strlen(location));
                strncat(location, result, MAXLENGTH - strlen(location));
                //Remove the newline or return
                if (location[strlen(location) - 2] == '\r')
                {
                    location[strlen(location) - 2] = '\0';
                }
                else if (location[strlen(location) - 1] == '\n')
                {
                    location[strlen(location) - 1] = '\0';
                }
                else ;
            } //End checking for being part of another word
            else fprintf(discardFile, "%s", inputline);
        }
        if ((result = strstr(inputline, "Module ")) != NULL)
        {
            //Make sure that there is not a character other than a space before the keyword
            startIndex = result - inputline;
            if (startIndex == 0 || !isalpha(inputline[startIndex - 1]))
            {
                //Get rest of string and add to location array
                result += strlen("Module ");
                strncat(location, "/", MAXLENGTH - strlen(location));
                strncat(location, result, MAXLENGTH - strlen(location));
                printf("Location: %s\n", location);
                //Remove the newline or return
                if (location[strlen(location) - 2] == '\r')
                {
                    location[strlen(location) - 2] = '\0';
                }
                else if (location[strlen(location) - 1] == '\n')
                {
                    location[strlen(location) - 1] = '\0';
                }
                else ;
            } //End checking for being part of another word
            else fprintf(discardFile, "%s", inputline);
        }

        //If not, then get the window name
        if ((result = strstr(inputline, "Begin ")) != NULL)
        {
            //Make sure that there is not a character other than a space before the keyword
            startIndex = result - inputline;
            if (startIndex == 0 || !isalpha(inputline[startIndex - 1]) || inputline[startIndex - 1] == 32)
            {
                //Get the last word of the line
                result += strlen("Begin ");
                while (*result != 32 && *result != '\r' && *result != '\n')
                    result++;
                //Advance one more character
                result++;
                //Concat to end of location array if that doesn't go over max num of chars
                strncat(location, "/", MAXLENGTH - strlen(location));
                strncat(location, result, MAXLENGTH - strlen(location));
                //Remove the newline or return
                if (location[strlen(location) - 2] == '\r')
                {
                    location[strlen(location) - 2] = '\0';
                }
                else if (location[strlen(location) - 1] == '\n')
                {
                    location[strlen(location) - 1] = '\0';
                }
                else ;
            } //End checking for being part of another word
            else fprintf(discardFile, "%s", inputline);
        }
        //Or the interface name
        if ((result = strstr(inputline, "Interface ")) != NULL)
        {
            //Make sure that there is not a character other than a space before the keyword
            startIndex = result - inputline;
            if (startIndex == 0 || !isalpha(inputline[startIndex - 1]) || inputline[startIndex - 1] == 32)
            {
                //Get rest of string and add to location array
                result += strlen("Interface ");
                strncat(location, "/", MAXLENGTH - strlen(location));
                strncat(location, result, MAXLENGTH - strlen(location));
                printf("Location: %s\n", location);
                //Remove the newline or return
                if (location[strlen(location) - 2] == '\r')
                {
                    location[strlen(location) - 2] = '\0';
                }
                else if (location[strlen(location) - 1] == '\n')
                {
                    location[strlen(location) - 1] = '\0';
                }
                else ;
            } //End checking for being part of another word
            else fprintf(discardFile, "%s", inputline);
        }
        //Check if line is a function or sub - order is important here; we need to catch the Sub before WinSubFunction or it will fail
        if ((result = strstr(inputline, "Sub ")) != NULL)
        {
            //Make sure that there is not a letter before the keyword
            startIndex = result - inputline;
            if (startIndex == 0 || !isalpha(inputline[startIndex - 1]) || inputline[startIndex - 1] == 32)
            {
                //Get sub name
                result += strlen("Sub ");
                startIndex = result - inputline;
                endIndex = indexOfStr(inputline, "(") - 1;
                //If there was a match, pull out the name
                if (endIndex != -1)
                {
                    tempLength = copySubStr(inputline, tempArray, startIndex, endIndex, MAXLENGTH - 1);
                    //If there wasn't an error, print location and name to file
                    if (tempLength > 0)
                    {
                        fprintf(outputfile, "%s,%s", location, tempArray);

                        //= Get parameters and print entire list between double quotes =
                        startIndex = indexOfStr(inputline, "(");
                        endIndex = pairMarkers(startIndex, ')', inputline);
                        //If there was a match for the endIndex and the start is smaller than the end,
                        if (endIndex != -1 && endIndex != -2 && startIndex < endIndex)
                        {
                            //= Pull out the parameters and write them to the file =
                            tempLength = copySubStr(inputline, tempArray, startIndex + 1, endIndex - 1, MAXLENGTH - 1);
                            if (tempLength > 0)
                            {
                                fprintf(outputfile, ",'%s'", tempArray);
                            }
                            else fprintf(outputfile, ",");
                        }
                        else
                        {
                            printf("Error code: %i\n", endIndex);
                            fprintf(outputfile, ",");
                        }
                    }
                    //Print the comma that would have been there
                    else fprintf(outputfile, ",");

                    //= Get the return value type =
                    //Skip the space
                    startIndex = endIndex + 1;

                    //Get the first char before the EOL char as the end of the return value type
                    if ((result = strchr(inputline, '\r')) != NULL)
                    {
                        endIndex = result - inputline - 1;
                    }
                    else if ((result = strchr(inputline, '\n')) != NULL)
                    {
                        endIndex = result - inputline - 1;
                    }
                    else endIndex = startIndex;

                    if (inputline[startIndex] != '\r' && inputline[startIndex] != '\n')
                    {
                        tempLength = copySubStr(inputline, tempArray, startIndex + 1, endIndex - 1, MAXLENGTH - 1);
                        if (tempLength > 0)
                        {
                            fprintf(outputfile, ",%s\n", tempArray);
                        }
                        else
                        {
                            printf("Warning: program tried to copy string with boundaries %i and %i. %s\n", startIndex, endIndex, inputline);
                            fprintf(outputfile, ",");
                        }
                    }
                    else fprintf(outputfile, ",\n");
                }
                else
                {
                    fprintf(stderr, "There was an error: A '(' could not be found in %s.\nAborting.\n", result);
                    return -1;
                }
            } //End checking for being part of another word
            else fprintf(discardFile, "%s", inputline);
        } //End checking Sub

        if ((result = strstr(inputline, "Function ")) != NULL)
        {
            //Make sure that there is not a character other than a space before the keyword
            startIndex = result - inputline;
            if (startIndex == 0 || !isalpha(inputline[startIndex - 1]) || inputline[startIndex - 1] == 32)
            {
                //Get function name
                result += strlen("Function ");
                startIndex = result - inputline;
                endIndex = indexOfStr(inputline, "(") - 1;
                //If there was a match, pull out the name
                if (endIndex != -1)
                {
                    tempLength = copySubStr(inputline, tempArray, startIndex, endIndex, MAXLENGTH - 1);

                    //= If there wasn't an error =
                    if (tempLength > 0)
                    {
                        //Print location and name to file
                        fprintf(outputfile, "%s,%s", location, tempArray);
                        //Get parameters and print entire list between double quotes
                        result++;
                        startIndex = indexOfStr(inputline, "(");
                        endIndex = pairMarkers(startIndex, ')', inputline);
                        if (endIndex != -1 && endIndex != -2 && startIndex < endIndex)
                        {
                            tempLength = copySubStr(inputline, tempArray, startIndex + 1, endIndex - 1, MAXLENGTH - 1);
                            if (tempLength > 0)
                            {
                                fprintf(outputfile, ",'%s'", tempArray);
                            }
                            else fprintf(outputfile, ",");
                        }
                        else fprintf(outputfile, ",");
                    }
                    else fprintf(outputfile, ",");

                    //= Get the return value type =

                    startIndex = endIndex;
                    //Get the first char before the EOL char as the end of the return value type
                    if ((result = strchr(inputline, '\r')) != NULL)
                    {
                        endIndex = result - inputline - 1;
                    }
                    else if ((result = strchr(inputline, '\n')) != NULL)
                    {
                        endIndex = result - inputline - 1;
                    }
                    else endIndex = startIndex;

                    //If there is anything else before the EOL, print it to the file in the return value place
                    if (inputline[endIndex] != '\r' && inputline[endIndex] != '\n')
                    {
                        tempLength = copySubStr(inputline, tempArray, startIndex + 1, endIndex, MAXLENGTH - 1);
                        if (tempLength > 0)
                        {
                            fprintf(outputfile, ",%s\n", tempArray);
                        }
                        else
                        {
                            printf("Warning: program tried to copy sub string with boundaries %i and %i. %s\n", startIndex, endIndex, inputline);
                            fprintf(outputfile, ",");
                        }
                    }
                    //Otherwise, include the comma that would be there
                    else fprintf(outputfile, ",\n");
                }
                else
                {
                    fprintf(stderr, "There was an error: A '(' could not be found after %c.\nAborting.\n", *result);
                    return -1;
                }
            } //End checking for being part of another word
            else fprintf(discardFile, "%s", inputline);
        } //End checking function

        //No need to do anything with lines that don't match
    }

    printf("Finished! Output is in %s.\n", argv[2]);

    fclose(inputfile);
    fclose(outputfile);
    fclose(discardFile);
    return 0;

} //End of main()

//Finds and returns the index of the first char of subStr in mainStr, or -1 if not found
int indexOfStr(char *mainStr, char *subStr){
    int index;
    char *result;

    //If the substring is in the main string, find and return the index
    if ((result = strstr(mainStr, subStr)) != NULL)
    {
        index = result - mainStr;
        return index;
    }
    //If no match, return -1
    else return -1;

    //Shouldn't happen, but as a safety net
    return 0;
}

//Copies a substring from src to dest using subStart and subEnd as indexes. Copies at most maxLen chars, not including the NULL pointer
int copySubStr(char src[], char dest[], int srcStart, int srcEnd, int maxLen){
    int iter, i;

    //Make sure start is less than end
    if (srcStart > srcEnd)
        return -1;

    //Make sure end is less than maxLen
    if (srcEnd > maxLen)
        return -2;

    //If srcEnd is -1, go to the end of the array
    if (srcEnd == -1)
        srcEnd = strlen(src);

    //Copy substring bounded by srcStart and srcEnd to dest
    for (iter = srcStart, i = 0; iter <= srcEnd && src[iter] != '\0' && i < maxLen; iter++, i++){
        dest[i] = src[iter];
    }

    dest[i] = '\0';

    //Return length of dest
    return i;
}

//Given an opening marker index and the closing marker char, finds the matching closing marker for the opening marker at the index in src
int pairMarkers(int openIndex, char closingChar, char src[]){

    /*This function works by counting the number of markers found so far. The counter starts at 0. When the program finds another opening marker, it increases the counter by 1. When it finds the next closing marker, it decreases the counter by 1. When the counter reaches zero again and the closing marker is found, it must be the corresponding closing marker for the given opening one.*/

    int i, counter;
    char openingChar;

    counter = 0;

    //Make sure that openIndex is actually within the bounds of src's length
    if (strlen(src) <= openIndex)
        return -2;

    //Look through src for the closing marker, starting at the given index
    i = openIndex;
    openingChar = src[i];
    while (src[i] != '\0'){
        if (src[i] == openingChar)
        {
        //    printf("Char at opening: %c\n", src[i]);
            //printf("Char one before opening: %c\n", src[i - 1]);
            counter++;
            //printf("Counter: %i\n", counter);
        }
        else if (src[i] == closingChar)
        {
            //printf("Char one before closing: %c\n", src[i - 1]);
            counter--;
            if (counter == 0)
            {
                return i;
            }
        }
        else ;
        i++;
    }

    //-1 means closing char not found
    return -1;
}
