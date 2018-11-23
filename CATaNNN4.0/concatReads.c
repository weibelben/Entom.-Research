#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void printHeader(FILE *newFile, char* fileName, int argc);
/**
 * This main method reads through each of the listed input files and prints the
 * name of the species, then all of the reads of that species separated by a 
 * single space.
 */
int main(int argc, char *argv[])
{
	FILE *file;
	FILE *newFile;
	int j = 0;
	char *next_line = malloc(2000 * sizeof(char));;

	////////////////////////////////////////////////////////////////////////////
	// Ensured correct input format
	if (argc < 2)
        {
                printf("Usage: ./concatReads <input filename>* <output filename>\n");
                exit(1);
        }

	newFile = fopen(argv[argc - 1], "w");
        if (newFile == NULL)
        {
                printf("Cannot open file for writing.\n");
                exit(1);
        }
	////////////////////////////////////////////////////////////////////////////

	// Jump through the first file to create the header
	printHeader(newFile, argv[1], argc);	

	// iterates through each input file
	for (int i = 1; i < argc - 2; i++)
	{
		file = fopen(argv[i], "r");
	        if (file == NULL) {
	                printf("Cannot open %s for reading.\n", argv[i]);
	                exit(1);
	        }
			
		// remove the .<file type> from the end of file name
		j = 0;
		while (argv[i][j] != 46) j++; // ascii for '.'
		argv[i][j] = 0;	
		fprintf(newFile, "%s\n", argv[i]);
		
		// iterate through every line, printing the reads only
		while (NULL != fgets(next_line, 2000, file))
		{
			next_line[strlen(next_line) - 1] = 0;
			if (next_line[0] == 'C' || next_line[0] == 'G' || next_line[0] == '?' ||
				 next_line[0] == 'T' || next_line[0] == 'A') // ascii for '>'
				fprintf(newFile, "%s ", next_line);
			
			free(next_line);
			next_line = malloc(2000 * sizeof(char));
		}
		
		printf("\t%s\t written to %s\n", argv[i], argv[argc - 1]);
		fprintf(newFile, "\n");
		fclose(file);	
	}
	
	fclose(newFile);
}

/**
 * printHeader reads through the first input file to parse some information
 * and write 2 header lines to the top of the output file
 * It prints:
 * <number of species> <number of total genes> G
 * G <number of reads> <read length>*
 */
void printHeader(FILE *newFile, char* fileName, int argc)
{	
	// open the first input file
	FILE *file = fopen(fileName, "r");
	if (file == NULL) {
		printf("Could not open %s for reading\n", fileName);
		exit(1);
	}

	// allocate memory for vars
	int *lengths = malloc(200 * sizeof(int));
	int total_length = 0;
	int lengths_index = 0;
	int num_reads = 0;
	char line[2000];	

	// continue while there is more info in the file
	while (fgets(line, 2000, file) != NULL)
	{
		if (line[0] != 62) // '>'
		{
			// increment counts accordingly
			lengths[lengths_index] = strlen(line);
			lengths_index++;
			num_reads++;
			total_length += strlen(line);
		}
	}	

	// print information to file in specified format
	fprintf(newFile, "%d %d G\nG %d ", (argc - 2), total_length, num_reads); 	
	
	for (int i = 0; i < num_reads; i++)
		fprintf(newFile, "%d ", lengths[i]);
	fprintf(newFile, "\n");

	if (fclose(file) != 0)
		printf("Could not close %s", fileName);
}
