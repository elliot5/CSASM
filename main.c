#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* open_file(const char* dir)
{
	const char* mode = "r";
	FILE * file_ptr;
	char* buffer = 0;
	long length;
	file_ptr = fopen(dir, mode);
	if(file_ptr)
	{
		fseek(file_ptr, 0, SEEK_END);
		length = ftell(file_ptr);
		fseek(file_ptr, 0, SEEK_SET);
		buffer = malloc(length);
		if(buffer)
		{
			fread(buffer, 1, length, file_ptr);
		}
	}
	fclose(file_ptr);
	return buffer;
}

void tokenize(char* str)
{
	printf("New Token: %s\r\n", str);
}

void tokenize_lines(char* str)
{
	char* tok_ptr = NULL;
	tok_ptr = strtok(str, "\n");
	while(tok_ptr != NULL)
	{
		long length = strlen(tok_ptr);
		char* buffer = malloc(length);
		snprintf(buffer, length, "%s\n", tok_ptr);
		
		tokenize(buffer);
		tok_ptr = strtok(NULL, "\n");
		free(buffer);
	}
}

int main()
{
	tokenize_lines(open_file("file.csasm"));
	
	return 0;
}
