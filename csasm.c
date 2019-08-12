#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "csasm.h"

#define MEM_SIZE 255

long csasm_mem[MEM_SIZE];
long csasm_acc = 0;

int csasm_add(tknd_t data)
{
	long addr = strtol(data.operand, NULL, 0);
	csasm_acc += csasm_mem[addr];
	return 0;
}

int csasm_out(tknd_t data)
{
	printf("[asm_out] %ld\n", csasm_acc);
	return 0;
}

int csasm_mov(tknd_t data)
{
	long addr = strtol(data.operand, NULL, 0);
	csasm_mem[addr] = csasm_acc;
	return 0;
}

int csasm_ldr(tknd_t data)
{
	long addr = strtol(data.operand, NULL, 0); // 0 = hex
	csasm_acc = csasm_mem[addr];
	return 0;
}

int csasm_set(tknd_t data)
{
	long val = strtol(data.operand, NULL, 10); // 10 for base ten
	csasm_acc = val;
	return 0;
}

const deftkn_t CSASM_TKNS[] = {
	{0x0, "add", csasm_add},
	{0x1, "out", csasm_out},
	{0x2, "mov", csasm_mov},
	{0x3, "set", csasm_set},
	{0x4, "ldr", csasm_ldr}
};

const int TKNS_LEN = sizeof(CSASM_TKNS) / sizeof(CSASM_TKNS[0]);

char* open_file(const char* dir)
{
	const char* mode = "r";
	FILE * file_ptr;
	char* buffer;
	long length;
	file_ptr = fopen(dir, mode);
	if(file_ptr)
	{
		fseek(file_ptr, 0, SEEK_END);
		length = ftell(file_ptr) + 1;
		fseek(file_ptr, 0, SEEK_SET);
		buffer = malloc(length);
		if(buffer == NULL)
		{
			perror("Failed to malloc file buffer");
			exit(-1);
		}
		fread(buffer, 1, length, file_ptr);
		fclose(file_ptr);
	}
	buffer[length-1] = '\0';
	return buffer;
}

int str_opcode(const char* str)
{
	for(int i = 0; i < TKNS_LEN; i++)
	{
		deftkn_t curr_tkn = CSASM_TKNS[i];
		if(strcmp(curr_tkn.scode, str) == 0)
		{
			return curr_tkn.icode;
		}
	}
	return -1;
}

void print_token(const tkn_t token)
{
	printf("%u : %s\r\n", token.opcode, token.data.operand);
}

tkn_t tokenize(char* str)
{
	char* opcode = strtok_r(str, " ", &str);
	if(opcode == NULL)
	{
		perror("Tokenize opcode fail");
		exit(-1);
	}
	int i_opcode = str_opcode(opcode);
	if(i_opcode < 0)
	{
		perror("Unknown opcode");
		exit(-2);
	}
	char* operand = strtok_r(NULL, " ", &str);

	tknd_t token_data = {
		.operand = operand
	};

	tkn_t token = {
		.opcode = i_opcode,
		.data = token_data
	};

	return token;
}

tknarr_t tokenize_lines(char* str)
{
	long buffered_length = 16;
	long index = 0;
	char* tok_ptr = NULL;
	tok_ptr = strtok_r(str, "\n", &str);
	tknarr_t tokens = {
	       .tokens = malloc(sizeof(tkn_t) * buffered_length),
	       .buffered_length = buffered_length
	};
	while(tok_ptr != NULL)
	{
		if(index >= buffered_length)
		{
			buffered_length *= 2;
			tokens.tokens = realloc(&tokens, sizeof(tkn_t) * buffered_length);
			tokens.buffered_length = buffered_length;
		}
		long length = strlen(tok_ptr) + 1;
		char* buffer = malloc(length);
		if(buffer == NULL)
		{
			perror("Failed to malloc tkn buffer");
			exit(-1);
		}
		buffer[length-1] = '\0';
		snprintf(buffer, length, "%s", tok_ptr);
		
		tkn_t token = tokenize(buffer);
		tokens.tokens[index] = token;
		tokens.length = index + 1;
		tokens.tokens[index].data.src = buffer;	

		tok_ptr = strtok_r(NULL, "\n", &str);

		index = index + 1;
	}
	return tokens;
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "Program requires file directory input\n");
		return (-5);
	}
	char* directory = argv[1];
	char* file_contents = open_file(directory);
	if(file_contents == NULL)
	{
		fprintf(stderr, "Failed to read directory %s (does the file exist?)\n", directory);
		return (-4);
	}
	
	tknarr_t token_array = tokenize_lines(file_contents);
	
	tkn_t* tokens = token_array.tokens;
	size_t length = token_array.length;

	for(size_t i = 0; i < length; i++)
	{
		tkn_t token = tokens[i];
		CSASM_TKNS[token.opcode].def_func(token.data);
	}
	for(size_t i = 0; i < length; i++)
	{
		free(tokens[i].data.src);
	}
	free(tokens);
	free(file_contents);
	return 0;
}
