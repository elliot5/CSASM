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
	char* opcode = strtok_r(str, " ", &str);
	if(opcode == NULL)
	{
		perror("Tokenize opcode fail");
		exit(-1);
	}
	char* operands[2];
        operands[0] = strtok_r(NULL, " ", &str);
	operands[1] = strtok_r(NULL, " ", &str); //Find way for variable operands
	printf("Opcode(%s), Operand(%s)\r\n", opcode, operand);
}

void tokenize_lines(char* str)
{
	char* tok_ptr = NULL;
	tok_ptr = strtok_r(str, "\n", &str);
	while(tok_ptr != NULL)
	{
		long length = strlen(tok_ptr) + 1;
		char* buffer = malloc(length);
		snprintf(buffer, length, "%s", tok_ptr);
		tokenize(buffer);
		tok_ptr = strtok_r(NULL, "\n", &str);
		free(buffer);
	}
}

int main()
{
	tokenize_lines(open_file("file.csasm"));
	
	return 0;
}
