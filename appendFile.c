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

//Program to append one file to another. Like cat, but not restricted to Bash shells

#include <stdio.h>

//MAXLENGTH is 10,000
#define MAXLENGTH 10000

int main(int argc, char *argv[]){
    char srcFileLine[MAXLENGTH];
    FILE *srcFile;
    FILE *destFile;

    if (argc < 3)
    {
        printf("Usage: ./appendFile srcFile destFile\n");
        printf("Appends the contents of srcFile to the end of destFile.\n");
    }

    //Open files
    srcFile = fopen(argv[1], "r");
    destFile = fopen(argv[2], "a");

    //Make sure they exist
    if (srcFile == NULL)
    {
        fprintf(stderr, "Error! File %s not found. Exiting.\n", argv[1]);
        return -1;
    }
    //Shouldn't ever happen, but just in case
    if (destFile == NULL)
    {
        fprintf(stderr, "Error! File %s not found. Exiting.\n", argv[2]);
        return -1;
    }

    //While there's another line, get it and append it to destFile
    while (fgets(srcFileLine, MAXLENGTH, srcFile) != NULL){
        fprintf(destFile, "%s", srcFileLine);
    }

    fprintf(stderr, "Finished! File %s has been appended to %s. The original of %s still exists.\n", argv[1], argv[2], argv[1]);

    fclose(srcFile);
    fclose(destFile);

}//End of main()
