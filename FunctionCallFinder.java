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
Program to go through a .xojo_code file and find all of the function calls made in that file.
*/

import java.io.*;
import java.util.*;


public class FunctionCallFinder {

    public static void main(String[] args) throws IOException {
        int startIndex = 0;
        int endIndex = 0;
        int lineNumber = 0;
        int keyFileLength = 0;
        int skipLine = 0; //Bool for telling next loop to skip this line
        int lineContinues = 0; //For if the line takes more than one literal line
        int hadKeyword = 0; //If the starting of a continuing line had a keyword,
        //Java doesn't pass the program name as the first arg
        File inputFileName = new File(args[0]);
        File keywordFileName = new File("functNames.txt");
        Scanner codeFile;
        Scanner getFileLength;
        Scanner keywordFile = new Scanner(keywordFileName);
        String[] avoidKeywords = {
            " Sub ",
            " Function "
        };
        String fileLine = "";
        //Will be filled from file and set to length of file
        String[] keywords;
        PrintWriter callsOutput = new PrintWriter("callsOutput.txt");

        try {
            //If the file doesn't exist, throw a fit
            if (inputFileName.exists() == false){
                throw new IOException("File not found. Exiting.");
            }

            //Fill array with keywords

            //Open a scanner to read through the keywords file to get the length
            getFileLength = new Scanner(keywordFileName);
            //Count length of file
            while (getFileLength.hasNext()){
                keyFileLength++;
                fileLine = getFileLength.nextLine();
            }
            //Close scanner
            getFileLength.close();

            //Initialize array for keywords
            keywords = new String[keyFileLength];
            //Load keywords
            for (int i = 0; i < keyFileLength && keywordFile.hasNext(); i++){
                keywords[i] = keywordFile.next();
            }

            //Create Scanner
            codeFile = new Scanner(inputFileName);

            //Print header for file
            callsOutput.printf("==From %s:\n", args[0]);

            //Get function calls and write them to a file
            while (codeFile.hasNext()){
                //Line count starts with one, so advance right away
                lineNumber++;
                fileLine = codeFile.nextLine();
                //If it starts with End, then skip - no way it's a declaration
                if (fileLine.startsWith("End"))
                {
                    continue;
                }
                //Skip words tagged to avoid
                for (int i = 0; i < avoidKeywords.length; i++){
                    if (fileLine.indexOf(avoidKeywords[i]) != -1)
                    {
                        skipLine = 1;
                    }
                }
                //Check to see line needs skipping
                if (skipLine == 1)
                {
                    skipLine = 0;
                    continue;
                }
                //See if a keyword matches
                for (int i = 0; i < keywords.length; i++){
                    startIndex = fileLine.indexOf(keywords[i]);
                    //If the keyword is in the line,
                    if (startIndex != -1)
                    {
                        //If the line has the "continues on next line" marker, print line, set bool, and leave this loop
                        if (fileLine.indexOf('_') != -1)
                        {
                            callsOutput.printf("Line %d: %s", lineNumber, fileLine.substring(startIndex).trim());
                            //System.out.printf("Line %d: %s\n", lineNumber, fileLine.trim());
                            lineContinues = 1;
                            hadKeyword = 1;
                            break;
                        }
                        else
                        {
                            lineContinues = 0;
                        }

                        //Find the closing parenthesis - there will be one since the line is not continuing
                        endIndex = fileLine.lastIndexOf(')');
                        if (endIndex != -1)
                        {
                            //If the ')' isn't after the function call (actually happened!), then just go to the end of the string
                            if (endIndex < startIndex)
                            {
                                callsOutput.printf("Line %d: %s\n", lineNumber, fileLine.substring(startIndex).trim());
                                //Don't keep checking
                                break;
                            }
                            else
                            {
                                //Print call to file - endIndex won't actually include the ')', so add one to include it
                                callsOutput.printf("Line %d: %s\n", lineNumber, fileLine.substring(startIndex, endIndex + 1).trim());
                                break;
                            }
                        }
                    } //End of if startIndex != -1

                } //End of checking keywords

                //Print all continuing lines of a function call
                if (fileLine.indexOf('_') != -1 && lineContinues == 1 && hadKeyword == 1)
                {
                    while (codeFile.hasNext() && fileLine.indexOf('_') != -1){
                        lineNumber++;
                        fileLine = codeFile.nextLine();
                        callsOutput.printf(" %s", fileLine.trim());
                    }

                    //Print newline and reset vars
                    callsOutput.printf("\n");
                    lineContinues = 0;
                    hadKeyword = 0;
                }

            } //End of looking for function calls

            System.out.printf("Finished processing file %s - output is in %s.\n", args[0], "callsOutput.txt");

            //Close files
            callsOutput.close();

        } //End of try block
        catch (IOException err){
            System.out.println(err);
        }
        catch (StringIndexOutOfBoundsException err){
            System.out.printf("Code has misinterpreted the file again. Check at line %s\n", err.getMessage());
        }

    } //End of main()

} //End of class
