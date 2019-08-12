#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Token Data
typedef struct tknd
{
	const char* operand;
} tknd_t;


// Definition Token
typedef struct deftkn
{
	const unsigned int icode;
	const char* scode;
	int (*def_func)(tknd_t);
} deftkn_t;

// Token
typedef struct tkn
{
	const unsigned int opcode;
	const tknd_t data;
} tkn_t;

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
	printf("%ld\n", csasm_acc);
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

	tknd_t token_data = {
		.operand = strtok_r(NULL, " ", &str)
	};
	
	tkn_t token = {
		.opcode = i_opcode,
		.data = token_data
	};

	return token;
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

		tkn_t token = tokenize(buffer);
		CSASM_TKNS[token.opcode].def_func(token.data);

		tok_ptr = strtok_r(NULL, "\n", &str);
		free(buffer);
	}
}

int main()
{
	tokenize_lines(open_file("file.csasm"));
	
	return 0;
}
