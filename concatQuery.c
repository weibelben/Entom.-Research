/**
 * This program concatinates matched queries of subject reads adding N's where
 * there is no match
 * @author Ben Weibel weibelben@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int findFrontIndex(char* line); 
int findEndIndex(char* line);
int findReadLength(char* line);
char* findHeader(char* line);
char* splitHeader(char* header);
/**
 * The main method calls upon the 4 wethods above, reads and writes from and 2 files
 * precified by its argv[] indexes. 
 * @param argc is the number of args, should always be 3
 * @param argv is the array of string args, inputfile and output file
 * @author Ben Weibel weibelben@gmail.com
*/
int main(int argc, char *argv[])
{
	//////////////////////////////////////////////////////////////////////////////
	// ensure args are correct and files are not null
	if (argc != 3)
	{
		printf("Usage: ./concatQuery <input filename> <output filename>\n");
                exit(1);
	}	
	
	FILE *file = fopen(argv[1], "r");
        if (file == NULL)
        {
                printf("Cannot open file for reading.\n");
                exit(1);
        }
	
	FILE *newFile = fopen(argv[2], "w");
        if (newFile == NULL)
        {
                printf("Cannot open file for writing.\n");
                exit(1);
        }
	//////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////
	// declare necesary vars, read first line of input file and write header
	char first_line[300];
	char next_line[300];
	char *header = malloc(300 * sizeof(char));
	char *prev_header;
	char *print_string = malloc(300 * sizeof(char));
	int first_index;
	int last_index;
	int length;
	int query_length;
	int max_index;
	int num_matches = 0;	
	double percent_match;
	fgets(first_line, 300, file);

	// set header to the characters after the () and before '+' in the first line
	header = findHeader(first_line);
	print_string = splitHeader(header);

	fprintf(newFile, ">%s", print_string);
	for (int i = 0; i < strlen(header) - strlen(print_string); i++)
		header[i] = header[i + strlen(print_string) + 1];
	header[strlen(header) - strlen(print_string)] = 0;
	prev_header = header; 
	print_string = NULL;

	//////////////////////////////////////////////////////////////////////////////
	// Create the char array to be written to be written to the new file
	first_index = findFrontIndex(first_line);
	last_index = findEndIndex(first_line);
	max_index = findReadLength(first_line);
	query_length = last_index - first_index + 1;
	char first_query[query_length];
	char *read = malloc(max_index * sizeof(char));
	fgets(first_query, abs(query_length) * 2, file);
	
	if (query_length > 0)
        	for (int j = first_index; j < first_index + query_length; j++)
                	read[j - 1] = first_query[j - first_index];
	else
        	for (int j = last_index; j < first_index; j++)
                	read[j - 1] = first_query[j - last_index];
	
	
	///////////////////////////////////////////////////////////////////////////////
	// continue while the file has more content
	while (NULL != fgets(next_line, 300, file))
	{
		header = malloc(300 * sizeof(char));
		header = findHeader(next_line);
        	print_string = splitHeader(header);
		length = strlen(header) - strlen(print_string);
        	for (int i = 0; i < length; i++)
        	        header[i] = header[i + strlen(print_string) + 1];
	        header[strlen(header) - strlen(print_string)] = 0;

		// prints headers
		printf("\t%s\t%s\n", prev_header, header); 


		if (strcmp(prev_header, header) != 0)
		{
			// fill this read w 'N's and write it to file
			for (int i = 0; i < max_index; i++)
			{ // when not ATGC
				if (read[i] != 65 && read[i] != 67 
				&& read[i] != 71 && read[i] != 84)
                        		read[i] = 'N';
        			else num_matches++;
			}
			
			fprintf(newFile, "%s", header);
			// compute percent match and write to file
			percent_match = 100 * ((double)num_matches / (double)max_index);
        		fprintf(newFile, "%d of %d genes match: %f%s\n", num_matches, 
				max_index, percent_match, "%");
        		fprintf(newFile, "%s\n>", read);

			// set up next read
			for (int i = 0; i < max_index; i++) read[i] = 0; // null out read
			num_matches = 0;
			max_index = findReadLength(next_line);
			read = malloc(max_index + sizeof(char));

		}

		// write the matches into the correct indeces in our read
		// insert the current query into the current read
		first_index = findFrontIndex(next_line);
		last_index = findEndIndex(next_line);
		query_length = last_index - first_index + 1;
		char next_query[query_length];

		fgets(next_query, abs(query_length) * 2, file);

		if (query_length > 0) 
			for (int j = 0; j < query_length; j++)
				read[j + first_index - 1] = next_query[j];
		else
			for (int j = 0; j < abs(query_length); j++)
                        	read[last_index + j - 1] = next_query[j];
		
		fprintf(newFile, "%s ", print_string);
		prev_header = malloc(strlen(header));
		prev_header = header;
	}

	//////////////////////////////////////////////////////////////////////////////
	// fill characters that are not A,C,G,T with 'N'
	for (int i = 0; i < max_index; i++)
	{
		if (read[i] != 65 && read[i] != 67 && read[i] != 71 && read[i] != 84)
			read[i] = 'N';
		else num_matches++;
	}
	
	// compute percent match and write read to file
	percent_match = 100 * ((double)num_matches / (double)max_index);
	fprintf(newFile, "%s", header);
	fprintf(newFile, " %d of %d genes match: %f%s\n", num_matches, max_index, 
		percent_match, "%");
	fprintf(newFile, "%s\n", read);
	printf("Wrote Query matches from %s to %s\n", argv[1], argv[2]);
	
	// close both files
	free(read);
	free(header);
	free(print_string);
	fclose(file);
	fclose(newFile);
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
	
	while (*(line + i) != 41) i++; // move 1 set of ()
        while (*(line + i) != 40) i++; // 40 = ascii for '('
        while (*(line + i + num_digits) != 45) num_digits++; // 45 = '-'
        num_digits -= 3; // remove whitespace

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

	while (*(line + i) != 41) i++; // move 1 set of ()
	while (*(line + i) != 45) i++; // 45 = '-'
        while (*(line + i + num_digits) != 41) num_digits++; // 41 = ')'
        num_digits -= 3; // remove whitespace

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

	while (*(line + i) != 41) i++; // move 1 set of ()
	while (*(line + i) != 58) i++; // 58 - ascii for ':'
        while (*(line + i + num_digits) != 40) num_digits++;
        num_digits -= 3; // remove whitespace

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
	int i = 0;
	int num_digits = 0;
	int print_digit_counter = 0;
	char *head = malloc(300 * sizeof(char));

	// read scaffold
	while (*(line + i) != 46) i++; // ascii for "."
	i += 3;
	while (*(line + i + num_digits) != 91) num_digits++; // "["
	num_digits -= 2;
	for (int k = 0; k < num_digits; k++)
		head[k] = *(line + i + k);
	i += num_digits;
	print_digit_counter = num_digits;

	// read indexes and name of read
	while (*(line + i) != 40) i++; // "("
	num_digits = 0;
	while (*(line + i + num_digits) != 43) num_digits++; // "+"
	num_digits--;
	for (int j = 0; j < num_digits; j++)
		head[print_digit_counter + j] = *(line + i + j);

	return head;
}

/**
 * splitHeader removed the header name so the scaffold can be written to the 
 * new file along with the present read indexes
 */
char* splitHeader(char *header)
{
	int j = 0;

	while (*(header + j) != 41) j++; // ascii for ")"
	j++;

	char *print_string = malloc(j * sizeof(char)); 
	for (int i = 0; i < j; i++)
		print_string[i] = header[i];
	print_string[j] = 0;

	return print_string;
}
