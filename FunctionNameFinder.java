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
Goes through a Xojo program, finds the function/subroutine names from the declarations, and saves them to a file. Also saves the declaration lines.
Written by Bailie June 2019
*/

import java.io.*;
import java.util.*;

public class FunctionNameFinder {
    //Takes one argument
    public static void main(String[] args) throws IOException {
        int startIndex = 0;
        int endIndex = 0;
        //Java doesn't pass the program name as the first arg
        File inputFileName = new File(args[0]);
        Scanner fileInput;
        //==Replace with correct extension==
        String fileLine = "";
        //==Replace the keywords as necessary to adapt this program for various languages==
        //Sub (subroutine) is the same as a function but without a return value
        //Need spaces so that the program doesn't pick up these as parts of other words
        String[] keywords = {
            " Sub ",
            " Function "
        };
        PrintWriter namesOutput = new PrintWriter("names.txt");
        PrintWriter declOutput = new PrintWriter("decls.txt");

        try {
            //If the file doesn't exist, throw a fit
            if (inputFileName.exists() == false){
                throw new IOException("File not found. Exiting.");
            }

            //Create Scanner
            fileInput = new Scanner(inputFileName);

            //Take out function names from each fileline and write them to a file
            while (fileInput.hasNext()){
                fileLine = fileInput.nextLine();
                //If it starts with End, then skip - no way it's a declaration
                if (fileLine.trim().startsWith("End"))
                {
                    continue;
                }
                //See if a keyword matches
                for (int i = 0; i < keywords.length; i++){
                    startIndex = fileLine.indexOf(keywords[i]);

                    //If the keyword is in the line,
                    if (startIndex != -1)
                    {
                        //Print declaration and name (not including keyword or parameters) to files
                        declOutput.printf("%s\n", fileLine);
                        endIndex = fileLine.indexOf('(');
                        if (endIndex != -1)
                        {
                            //Print a space before the name to prevent matching part of a word later
                            namesOutput.printf(" %s\n", fileLine.substring(startIndex + keywords[i].length(), endIndex));
                        }
                    }

                } //End of checking keywords

            } //End of looking for function names

            System.out.printf("Finished - declarations are in %s and names are in %s.\n", "decls.txt", "names.txt");

            //Close files
            namesOutput.close();
            declOutput.close();

        } //End of try block
        catch (IOException err){
            System.out.println(err);
        }

    } //End of main()

} //End of class
