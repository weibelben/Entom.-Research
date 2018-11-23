/**
 * This program aligns the specified reads of all .out files in directory
 * @author Ben Weibel weibelben@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* findName(char* header);
int findChar(int index, char *line, char ch);

int main(int argc, char *argv[])
{
	char header[300];
	char *read_name = NULL;
	char *species = NULL;
	char read[2000];
	int first_read = 1;
	
        if (argc < 3) {
                printf("Usage: ./alignReads <input filename>* <read name>\n");
                exit(1);
        }
	
	// create a .phy file with the read name specification
	char filename[strlen((argv[argc - 1] + 4)) * sizeof(char)];
	int j;
	for (j = 0; j < strlen(argv[argc-1]); j++)
		filename[j] = argv[argc-1][j];
	filename[j] = '.';
	filename[j+1] = 'p';
	filename[j+2] = 'h';
	filename[j+3] = 'y';
	filename[j+4] = 0;

	FILE *newFile = fopen(filename, "w");
        if (newFile == NULL) {
                printf("Cannot open %s for writing.\n", filename);
                exit(1);
        }
	
	fprintf(newFile, "%s ", argv[argc-1]);	
	// iterate through the .out files that were inputed
	for (int i = 1; i < argc - 2; i++)
	{
		FILE *file = fopen(argv[i], "r");
	        if (file == NULL) {
        	        printf("Cannot open %s for writing.\n", argv[i]);
                	exit(1);
        	}
		
		species = argv[i];
		species[findChar(0, species, '.')] = 0;
		printf("\t%s\n", species);

		// iterate through the lines of the file
		while (NULL != fgets(header, 300, file))		
		{
			if (header[0] == 62) // ascii for '>'
				read_name = findName(header);
			
			// compare header with read name input
			if (strcmp(read_name, argv[argc-1]) == 0)
			{
				if (first_read == 1)
				{
					// print length of read if it is the first file
					fgets(read, 2000, file);
					fprintf(newFile, "%d s\n\n", strlen(read));
					first_read = 0;
				}
				else 
				{
					// simply print the species and read
					fprintf(newFile, "%s\n          ", species);
					fgets(read, 2000, file);
					fprintf(newFile, "%s\n", read);
				}
			}
		}
		fclose(file);
	}
	
	fclose(newFile);
}

/*
 * findName returns the name of the read given a .out format header 
 */
char* findName(char* header)
{
	char *name = malloc(20 * sizeof(char));
	int i = 0;
	
	while (header[i] != 32) i++; // ascii for space
	for (int j = 0; j < i - 1; j++)
		name[j] = header[j+1];
	name[i-1] = 0;
	return name;
}	

/*
 * findChar counts the index until a specified character is found
 */
int findChar(int index, char *line, char ch)
{
        while (line[index] != ch)
        {
                if (line[index] == 10) // ascii for newline
                {
                        printf("\tYou're doing great! One issue:\n");
                        printf("***ERROR*** File format missmatch, could not find (%c)\n", ch);
                        exit(1);
                }
                index++;
        }
        return index;
}

