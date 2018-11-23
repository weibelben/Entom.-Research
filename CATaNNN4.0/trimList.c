#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * This main method reads through each of the listed input files and prints the
 * name of the species, then all of the reads of that species separated by a 
 * single space.
 */
int main(int argc, char *argv[])
{
	FILE *file;
	FILE *newFile;
	char *line = malloc(100 * sizeof(char));
	char *write_line = malloc(100 * sizeof(char));
	char *int_string = malloc(10 * sizeof(char));
	int i = 0;
	int len = 0;
	
	////////////////////////////////////////////////////////////////////////////
	// Ensured correct input format
	if (argc < 3)
        {
                printf("Usage: ./trimList <list file> <new list file>\n");
                exit(1);
        }

	file = fopen(argv[1], "r");
	if (file == NULL) {
		printf("Cannot open %s for reading.\n", argv[i]);
	        exit(1);
	}

	newFile = fopen(argv[2], "w");
	if (file == NULL) {
		printf("Cannot open %s for reading.\n", argv[i]);
	        exit(1);
	}
	
	// iterate through every line, printing the reads only
	while (NULL != fgets(line, 100, file))
	{
		while(line[i] != 32) 
		{
			write_line[i] = line[i];
			i++; // ascii for space
		}
		fprintf(newFile, "%s ", write_line);
		
		for (int j = 0; j < strlen(line) - i; j++)
			int_string[j] = line[j + i];
		
		len = atoi(int_string);
		len = (len - (len%3));
		fprintf(newFile, "%d\n", len);
		
		i = 0;		
		int_string[0] = 0;
		free(int_string);
		free(line);
		free(write_line);
		int_string = malloc(10 * sizeof(char));
		line = malloc(100 * sizeof(char));
		write_line = malloc(100 * sizeof(char));
	}

	fclose(file);
	fclose(newFile);
}
