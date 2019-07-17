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

#include <stdio.h>
#include <string.h>

/*Reads through a file and prints out the keywords when it finds them. Written to read through a logfile and print out method names, but could be easily adapted by changing the list of keywords.
Written by Bailie Livingston May 2019*/

//Macros
#define FALSE 0
#define MAXLENGTH 10000
#define NUMKEYWORDS 96
#define TRUE 1


//Functions
int bgetline(char s[], int putNl);

int main(int argc, char *argv[]){
    struct item {
        char searchTerm[MAXLENGTH];
        int count;
    };

    char fileName[MAXLENGTH];
    char outputFileName[MAXLENGTH] = "parseLogsOutput.txt"; //Default never used; not sure why this is here
    char inputFileName[MAXLENGTH];
    char fileline[MAXLENGTH];
    char *searchTerm;
    char inputChar;
    char *subStr;
    //Words to look for in log. Alphabetical
    char keywords[NUMKEYWORDS][30] = {
        "==", //Comments
        "Age",
        "ASD",
        "Attempting",
        "BDM",
        "Charts",
        "ChgDr",
        "ClHn",
        "Connection",
        "Creating",
        "Decrypt",
        "Deleting",
        "Deletion",
        "DSF",
        "EMAIL",
        "Exception:",
        "Fetching",
        "FlHn",
        "GA",
        "GET",
        "GINME",
        "GME",
        "INME",
        "LLE",
        "LLWS",
        "MBF",
        "POST",
        "Parsing",
        "RESULT",
        "Received",
        "RegForm",
        "S3FH",
        "SMD",
        "SMSDr",
        "Saving",
        "Send",
        "Sent",
        "Session",
        "Storing",
        "SubDr",
        "Team",
        "TkBs",
        "TkV1",
        "Token",
        "Trying",
        "TW",
        "ZMD",
        "_get_token_impl",
        "acknowledge",
        "age_ranges",
        "can_add_group",
        "can_change_group_members_role",
        "can_search_users",
        "conf",
        "contact",
        "create",
        "decrypt",
        "document_add",
        "document_delete",
        "document_folder",
        "document_move",
        "edit",
        "email_group",
        "is_existing_user", //Purposefully out of place. Needs to be hit before existing_user, or will never be found
        "existing_user",
        "faq",
        "format_str_url",
        "group_text_create",
        "group_types",
        "handle_error",
        "how_it_works",
        "index",
        "inject_common_variables",
        "invite",
        "link_types",
        "load_user",
        "login",
        "logout",
        "meeting_create",
        "meeting_delete",
        "meeting_time_types",
        "msg",
        "org:",
        "org_by_vanity_name",
        "page_not_found",
        "prices",
        "privacy_setting_choices",
        "received",
        "redirect_dest",
        "register_extensions",
        "result",
        "show",
        "sms_driver",
        "teardown_request",
        "update",
        "view_all"
    };

    struct item instances[NUMKEYWORDS];

    int i, j, inner;
    int nl;
    int subIndex = 0;
    int linesNoKeyword = 0;

    FILE *input;
    FILE *output;

    /*===Set up variables===*/
    for (int iter = 0; iter < NUMKEYWORDS && keywords[iter] != NULL; iter++){
        strcpy(instances[iter].searchTerm, keywords[iter]);
    }

    /*===Parse Arguments===*/

    if ((argc == 2) && (argv[1][0] == 'h'))
    {
        printf("Usage: ./parseLogs inputFilename outputFilename\nIf you do not supply an output filename, the output will go to standard output.\nReads through a logfile for Lifeloop and prints out keywords.\n");
        return 0;
    }

    //One or more arguments (besides program name) means input file
    if (argc > 1)
    {
        input = fopen(argv[1], "r");
        if (!input)
        {
            fprintf(stderr, "Error! File %s not found. Exiting.\n", argv[1]);
            return -1;
        }
    }
    //Two or more arguments besides program name means output file
    if (argc > 2)
    {
        output = fopen(argv[2], "a");
        fprintf(output, "==New runthrough==\n");
        strcpy(outputFileName, argv[2]);
    }
    //No arguments, just the program name
    else if (argc == 1)
    {
        //Get input filename
        printf("Please enter the hard path of the file to read through (e.g. /home/myUsername/python/myPythonFile.py): \n");
        i = bgetline(inputFileName, FALSE);
        //Open file
        input = fopen(inputFileName, "r");

        if (!input)
        {
            fprintf(stderr, "Error: incorrect filename given. Exiting.\n");
            return -1;
        }

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
            printf("Got filename!\n");
            output = fopen(outputFileName, "a");
            fprintf(output, "==New runthrough==\n");
        }
    }
    //One argument besides program name means they must not want to save to a file
    //This goes with the second if statement
    else
    {
        output = stdout;
    }


    /*===Parse File===*/

    //char *strstr(const char *haystack, const char *needle) finds first occurrence of "needle" in string haystack

    //Loop through file and look at each line to find the substring
    for (int iter = 0; iter < MAXLENGTH && (fgets(fileline, MAXLENGTH, input) != NULL); iter++){
        //Skip empty lines
        if (fileline[0] == '\n')
        {
            continue;
        }
        for (inner = 0; inner < NUMKEYWORDS && (instances[inner].searchTerm != NULL); inner++){
            subStr = strstr(fileline, instances[inner].searchTerm);
            //If there was a match,
            if (subStr != NULL)
            {
                //Print rest of line
                fprintf(output, "%s", subStr);
                /*if (subStr[strlen(fileline)] != '\n')
                {
                    fprintf(output, "\n");
                }*/
                instances[inner].count++;
                break;
            }
        }
        if (subStr == NULL)
        {
            linesNoKeyword++;
        }
    }

    //Print number of occurrences of each keyword
    for (int iter = 0; iter < NUMKEYWORDS && (instances[inner].searchTerm != NULL); iter++){
        if (instances[iter].count > 0)
        {
            fprintf(output, "%s: %i\n", instances[iter].searchTerm, instances[iter].count);
        }
    }
    fprintf(output, "Lines without keywords: %i\n", linesNoKeyword);

    //Close files
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
