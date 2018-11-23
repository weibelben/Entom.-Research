/**
 * This program concatinates matched queries of subject reads adding '?'s where
 * there is no match
 * @author Ben Weibel weibelben@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readFile(char *filename);
int findFrontIndex(char* line); 
int findEndIndex(char* line);
int findReadLength(char* line);
char* findHeader(char* line);
char* splitHeader(char* header);
char* splitName(char* header);
int fill(char *read, int num_matches, int max_index);
void insertQuery(char *next_query, char *read, int first_index, int last_index, int max_index);
int findChar(int index, char *line, char ch);


char *read = NULL;
char *return_string = NULL;
char *print_string = NULL;
char *name = NULL;
char *prev_header = NULL;
/**
 * The main method calls upon the readFile method which reads and writes from
 * and to 2 files specified by its argv[] indexes. 
 * @param argc is the number of args, should always be 2
 * @param argv is the array of string args, inputfile and output file
 * @author Ben Weibel weibelben@gmail.com
*/
int main(int argc, char *argv[])
{
	if (argc < 2) {
                printf("Usage: ./concatQuery <input filename>*\n");
                exit(1);
        }

	// iterates through each of the input files
	for (int fileNum = 1; fileNum < argc; fileNum++)
		readFile(argv[fileNum]);
}

/**
 * readFile is the core method of the program which calls other programs to 
 * properly read and write to and from the input files and the respective 
 * files it creates.
 */
void readFile(char *filename)
{
	char first_line[300];
	char next_line[300];
	char *print_string = NULL;
	char *header = NULL;
	char *header_line = NULL;
	char *species = NULL;
	int first_index;
	int last_index;
	int length;
	int query_length;
	int max_index;
	int num_matches;	
	int index;
	double percent_match;
	
//////////////////////////////////////////////////////////////////////////////
// ensure args are correct and files are not null
	// open file, verify it exists
	FILE *file = fopen(filename, "r");
        if (file == NULL) {
                printf("Cannot open %s for reading.\n", filename);
                exit(1);
       	}
	
	// parse the name of the file for the species name
	index = 0;
	species = filename;
	while (species[index] != 46) index++; // ascii for '.'
	species[index] = 0;
	printf("\nSpecies = %s:\n", species);

	// change output file type to .out
	species[index] = 46;
	species[index+1] = 111;
	species[index+2] = 117;
	species[index+3] = 116;
	species[index+4] = 0;

	// open the output file and verify it was created
	FILE *newFile = fopen(species, "w");
        if (newFile == NULL) {
       	        printf("Cannot open file for writing.\n");
               	exit(1);
	}

//////////////////////////////////////////////////////////////////////////////
// initiate necesary vars, read first line of input file and write header
	num_matches = 0;	
	fgets(first_line, 300, file);

	// set header to the characters after the () and before '+'
	header_line = findHeader(first_line);
	print_string = splitHeader(header_line);
	header = splitName(header_line);

	// write the header to the output file
	fprintf(newFile, ">%s %s", header, print_string);
	length = strlen(header) - strlen(print_string);
	for (int i = 0; i < length; i++)
		header[i] = header[i + strlen(print_string) + 1];
	header[length] = 0;
	prev_header = header; 

//////////////////////////////////////////////////////////////////////////////
// Create the char array to be written to be written to the new file
	first_index = findFrontIndex(first_line);
	last_index = findEndIndex(first_line);
	max_index = findReadLength(first_line);
	query_length = last_index - first_index + 1;

	// initialize first_query and read
	char first_query[abs(query_length) * 2];

	if (read != NULL) {
		*(read + max_index) = 0;
		read = NULL;
	}

	read = malloc(max_index * sizeof(char));
	fgets(first_query, abs(query_length) * 2, file);

	// copy the contents of the query to the read, possibly inverted
	insertQuery(first_query, read, first_index, last_index, max_index);
	
///////////////////////////////////////////////////////////////////////////////
// continue while the file has more content
	while (NULL != fgets(next_line, 300, file))
	{
		// locate the name of the read from the header
		header_line = findHeader(next_line);
        	print_string = splitHeader(header_line);
		header = splitName(header_line);

		// print previous header to show that it completed
		printf("\t\t%s\t%s\n", prev_header, header); 
	
		// if this header is differnt from the last
		if (strcmp(prev_header, header) != 0)
		{
			read[max_index] = 0;
			// fills empty indexes of read w 'N's
			num_matches = fill(read, num_matches, max_index);
			
			// compute percent match and write to file
			percent_match = 100 * ((double)num_matches / (double)max_index);
        		fprintf(newFile, "%d of %d genes match: %f%s\n", num_matches, 
			max_index, percent_match, "%");
       			fprintf(newFile, "%s\n>%s", read, header);
			// set up next read

			// reset vars
			num_matches = 0;
			max_index = findReadLength(next_line);
			read = NULL;
			read = malloc(max_index + sizeof(char));
		}

		// write the matches into the correct indeces in our read
		// insert the current query into the current read
		first_index = findFrontIndex(next_line);
		last_index = findEndIndex(next_line);
		query_length = last_index - first_index + 1;
		char next_query[abs(query_length) * 2];
		fgets(next_query, abs(query_length) * 2, file);

		// inserts query in read, possibly inverted
		insertQuery(next_query, read, first_index, last_index, max_index);

		// resets headers and prints scaffold to file
		fprintf(newFile, " %s ", print_string);
		//prev_header = header;
		
		prev_header = malloc(strlen(header) *sizeof(char));
		for (int i = 0 ; i< strlen(header); i++)
			prev_header[i] = header[i];
	} // end while loop

	// fill empty read indexes with 'N'
	num_matches = fill(read, num_matches, max_index);

	// compute percent match and write read to file
	percent_match = 100 * ((double)num_matches / (double)max_index);
	fprintf(newFile, " %d of %d genes match: %f%s\n", num_matches, max_index, 
		percent_match, "%");
	fprintf(newFile, "%s\n", read);
	// close both files

	if (fclose(file) != 0 || fclose(newFile) != 0) {
		printf("Failed to close file!");
		exit(1);
	}
}

/**
 * findFrontIndex reads the provided header line and parses for the numerical
 * value that will represent the beginning index of the query in the read
 * @param line is the header line
 * @return the integer value of the start index
 */
int findFrontIndex(char* line)
{
	int num_digits = 0;
	int i = 0;
	int first_index;
	char first_index_str[10];
	
	i = findChar(i, line, ')');
	i = findChar(i, line, '(');
	num_digits = findChar(i, line, '-') - i - 3;

	// parse integer
        for (int j = 0; j < num_digits; j++)
        	first_index_str[j] = *(line + i + j + 2);
        first_index_str[num_digits] = 0;
	first_index = atoi(first_index_str);
	return first_index;
}

/**
 * findEndIndex reads the provided header line and parses for the numerical
 * value that will represent the end index of the query in the read
 * @param line is the header line
 * @return the integer value of the end index
 */
int findEndIndex(char* line)
{
	int num_digits = 0;
        int i = 0;
        int last_index;
        char last_index_str[10];
	
	i = findChar(i, line, ')');
	i = findChar(i, line, '(');
	i = findChar(i, line, '-');
	num_digits = findChar(i, line, ')') - i - 2;
 
	// parse for an integer
        for (int j = 0; j < num_digits; j++)
        	last_index_str[j] = *(line + i + j + 2);
        last_index_str[num_digits] = 0; // end the string
        last_index = atoi(last_index_str);
	return last_index;
}

/**
 * findReadLength reads the provided header line and parses for the numerical
 * value that will represent the length of the entire read
 * @param line is the header line
 * @return the integer value of the length 
 */
int findReadLength(char* line)
{
	int num_digits = 0;
        int i = 0;
        int max_index;

	i = findChar(i, line, ')');
	i = findChar(i, line, ':');
	num_digits = findChar(i + num_digits, line, '(') - i - 3;

	// parse for an integer
        char max_index_string[num_digits];
        for (int j = 0; j < num_digits; j++)
                max_index_string[j] = *(line + i + j + 1);
        max_index_string[num_digits] = 0;
	max_index = atoi(max_index_string);
	return max_index;
}

/**
 * Given a string, this method iterates through and finds the name or 
 * header of the current read
 * @param line is the entire header line
 * @return a char pointer to the ~9 char header
 */
char* findHeader(char* line) 
{
	int i = 0; // index in string
	int num_digits = 0; // number of chars covered in orig. header
	int print_digit_counter = 0; // number of chars printed
	char head[200] = ""; // name of the current read
	char read_name[50] = "";

	return_string = NULL;
	return_string = malloc(strlen(line) * sizeof(char));

	// read scaffold
	i = findChar(i, line, '.') + 3; // move pointer to after the first name
	num_digits = findChar(i + num_digits, line, '[') - i - 2;

	// write scaffold name to head
	for (int k = 0; k < num_digits; k++)
		head[k] = *(line + i + k);
	head[num_digits] = 0;
	i += num_digits;
	print_digit_counter = num_digits;

	// read indexes and name of read
	i = findChar(i, line, '(');
	num_digits = findChar(i, line, ')') - i + 1;

	for (int j = 0; j < num_digits; j++)
		head[print_digit_counter + j] = *(line + i + j);
	i += num_digits + 1;
	print_digit_counter += num_digits;
	head[print_digit_counter] = 0;

	num_digits = findChar(i, line, ':') - i - 4;
	for (int j = 0; j < num_digits; j++)
		read_name[j] = *(line + i + j);
	read_name[num_digits] = 0;
	read_name[num_digits+1] = 0;

	// place name in the front of the returned char*
	for (int j = 0; j < strlen(read_name); j++)
		return_string[j] = read_name[j];
	return_string[strlen(read_name)] = 33; // '!'

	// concatinate the scaffold data
	for (int j = 0; j < strlen(head); j++)
		return_string[j + 1 + strlen(read_name)] = head[j];
	return_string[1 + strlen(read_name) + strlen(head)] = 0;

	return return_string;
}

/**
 * splitHeader removed the header name so the scaffold can be written to the 
 * new file along with the present read indexes
 */
char* splitHeader(char *header)
{
	int j = findChar(0, header, '!') + 1;
	
	print_string = NULL;
	print_string = malloc(50 * sizeof(char)); 

	for (int i = j; i < strlen(header); i++)
		print_string[i - j] = header[i];
	print_string[strlen(header) - j] = 0;

	return print_string;
}

/**
 * splitName isolates the name of the read
 */
char* splitName(char* header)
{
	name = NULL;
	name = malloc(strlen(header) * sizeof(char));

	int j = findChar(0, header, '!');
	
	for (int i = 0; i < j; i++)
		name[i] = header[i];
	name[j] = 0;
	return name;
}

/**
 * fill completes each read by inserting a '?' to every index where there is no 
 * A G T or C
 */
int fill(char *read, int num_matches, int max_index)
{
	for (int i = 0; i < max_index; i++)
		if (read[i] != 65 && read[i] != 67 && read[i] != 71 && read[i] != 84)
             		read[i] = '?';
		else num_matches++;

	return num_matches;
}

/**
 * insertQuery inserts an existing query into the current read and inverts it
 * if necessary 
 */
void insertQuery(char *next_query, char *read, int first_index, int last_index, int max_index)
{
	int out_of_bounds = 0;
	// first and last indexes must be decremented to fit 0 indexing of arrays
	if (last_index > first_index)
       		for (int j = 0; j < strlen(next_query); j++)
			if (j + first_index < max_index + 2)
       	        		read[j + first_index - 1] = next_query[j];
			else
				out_of_bounds++;	
	else
		for (int j = 0; j < strlen(next_query); j++)
    		{
			if ((first_index - j) < max_index)	
			{	
      	 			if (next_query[j] == 65) // 'A'
					read[first_index - j] = 84; // 'T'
				if (next_query[first_index - j] == 84) // 'T'
					read[first_index - j] = 65; // 'A'
				if (next_query[first_index - j] == 67) // 'C'
					read[first_index -j] = 71; // 'G'
				if (next_query[first_index - j] == 71) // 'G'
					read[first_index - j] = 67; // 'C'
			}
			else 
				out_of_bounds++;
		}
	// counts and tells you how off your data was
	if (out_of_bounds != 0)
		printf("\n\t  (%d)\tquery indexes outide of read.\n\n", out_of_bounds);
}

/**
 * findChar correctly increments a count of how many chars the pointer had to move to 
 * find the specified character. If it is not found, an error message is printed
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

