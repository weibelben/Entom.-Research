/**
 * This program aligns the specified reads of all .out files in directory
 * @author Ben Weibel weibelben@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const size_t BUFFER_SIZE = 5000;

void findRead(FILE *newFile, char *file_name, char *species);
int findChar(int index, char *line, char ch);
int findLength(char *header);

int main(int argc, char *argv[])
{
	int j = 0;
	int read_length = 0;
	char read_name[50];

        if (argc < 3) {
                printf("Usage: ./alignFromList <input filename>* <list file name>\n");
                exit(1);
        }

	FILE *listFile = fopen(argv[argc - 1], "r");
        if (listFile == NULL) {
                printf("Cannot open %s for reading.\n", argv[argc - 1]);
                exit(1);
        }

	while (NULL != fgets(read_name, 100, listFile)) 
	{

		read_length = findLength(read_name);
		read_name[findChar(0, read_name, 32)] = 0; // ascii for space
		for (int i = 0; i < strlen(read_name); i++)
			read_name[i] = read_name[i+1];
		printf("%s: \n", read_name);

	
		// create a .phy file with the read name specification
		char filename[strlen(read_name) + 4];
		for (j = 0; j < strlen(read_name); j++)
			filename[j] = read_name[j];
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
		
		fprintf(newFile, "%d %d\n\n", argc-2, read_length);
		
		for (int i = 1; i < argc - 1; i++)
			findRead(newFile, argv[i], read_name);

		if (0 != fclose(newFile)) {
			printf("could not close %s\n", filename);
			exit(1);
		}

	} 	
}

/*
 * findRead iterates through the headers of each .out file and prints the correct
 * names and reads to the alignment file
 */
void findRead(FILE *newFile, char *file_name, char *desired_read)
{
	char header[300];
	char read_name[50];
	char read[BUFFER_SIZE];
	char file_name_array[strlen(file_name)];

	FILE *file = fopen(file_name, "r");
        if (file == NULL) {
       	        printf("Cannot open %s for writing.\n", file_name);
               	exit(1);
       	}
	
	for (int i = 0; i < findChar(0, file_name, 46); i++)
		file_name_array[i] = file_name[i];
	file_name_array[findChar(0, file_name, 46)] = 0; 
	printf("\t%s\n", file_name_array);

	// iterate through the lines of the file
	while (NULL != fgets(header, 300, file))		
	{
		if (header[0] == 62) // ascii for '>'
		{
			int index = 0;
			// looks for space of carriage return
			while (header[index] != 32 && header[index] != 13) index++; 
			for (int j = 0; j < index - 1; j++)
				read_name[j] = header[j+1];
			read_name[index-1] = 0;
		}
	
		// compare header with read name input
		if (strcmp(read_name, desired_read) == 0)
		{
			// simply print the species and read
			fgets(read, BUFFER_SIZE, file);
			// make the reads a multiple of 3 characters long
			read[strlen(read) - (strlen(read) % 3)] = 0;
			fprintf(newFile, "%s\n          ", file_name_array);
			fprintf(newFile, "%s\n", read);
			break;
		}
	}
	if (0 != fclose(file)) {
		printf("could not close %s\n", file_name);
		exit(1);
	}
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

/*
 * findLength returns the length of a read based on its definition in the 
 * provided list
 */
int findLength(char *header)
{	
	int index = findChar(0, header, 32);
	char len[strlen(header) - index - 1];
	for (int i = index + 1; i < strlen(header); i++)
		len[i - index - 1] = header[i];
	len[strlen(header) - index - 1] = 0;

	int length = atoi(len);
	return length;
}
