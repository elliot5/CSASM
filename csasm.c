#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "csasm.h"

#define GENERIC_ERR -1

long csasm_mem[MEM_SIZE];
long csasm_acc = 0;
linedarr_t csasm_labels;

int cerr_print()
{
	fprintf(stderr, "[asm_err] ");
	return 1;
}

int add_label(long line, char* label)
{
	csasm_labels.lined = realloc(csasm_labels.lined, sizeof(lined_t) * csasm_labels.length + 1);
	lined_t ln_data = {
		.label = label,
		.line = line
	};
	csasm_labels.lined[csasm_labels.length] = ln_data;
	csasm_labels.length += 1;
	return 0;
}

int get_label(char* label)
{
	for(int i = 0; i < csasm_labels.length; i++)
	{
		if(strcmp(csasm_labels.lined[i].label, label) == 0)
		{
			return csasm_labels.lined[i].line;
		}
	}
	return -1;
}

int csasm_add(tknd_t data, csparams_t* params)
{
	if(data.operand == NULL)
	{
		cerr_print();
		fprintf(stderr, "ADD requires operand ADDR\n");
		exit(GENERIC_ERR);
	}
	long addr = strtol(data.operand, NULL, 0);
	params->acc += params->memory[addr];
	return 0;
}

int csasm_out(tknd_t data, csparams_t* params)
{
	printf("[asm_out] %ld\n", params->acc);
	return 0;
}

int csasm_mov(tknd_t data, csparams_t* params)
{
	if(data.operand == NULL)
	{
		cerr_print();
		fprintf(stderr, "MOV requires operand ADDR\n");
		exit(GENERIC_ERR);
	}
	long addr = strtol(data.operand, NULL, 0);
	params->memory[addr] = params->acc;
	return 0;
}

int csasm_ldr(tknd_t data, csparams_t* params)
{
	if(data.operand == NULL)
	{
		cerr_print();
		fprintf(stderr, "LDR requires operand ADDR\n");
		exit(GENERIC_ERR);
	}
	long addr = strtol(data.operand, NULL, 0); // 0 = hex
	params->acc = params->memory[addr];
	return 0;
}

int csasm_set(tknd_t data, csparams_t* params)
{
	if(data.operand == NULL)
	{
		cerr_print();
		fprintf(stderr, "SET requires operand VAL\n");
		exit(GENERIC_ERR);
	}
	long val = strtol(data.operand, NULL, 10); // 10 for base ten
	params->acc= val;
	return 0;
}

int csasm_lbl(tknd_t data, csparams_t* params)
{
	
}

int csasm_jmp(tknd_t data, csparams_t* params)
{
	if(data.operand == NULL)
	{
		cerr_print();
		fprintf(stderr, "JMP requires operand LABEL\n");
		exit(GENERIC_ERR);
	}
	int ret = get_label(data.operand);
	if(ret < 0)
	{
		cerr_print();
		fprintf(stderr, "Failed to JMP to label %s (does it exist?)\n", data.operand);
		exit(GENERIC_ERR);
	}
	params->line = ret;
}

int csasm_inp(tknd_t data, csparams_t* params)
{
	long val = 0;
	scanf("%ld", &val);
	params->acc = val;
}

const deftkn_t CSASM_TKNS[] = {
	{0x0, "add", csasm_add},
	{0x1, "out", csasm_out},
	{0x2, "mov", csasm_mov},
	{0x3, "set", csasm_set},
	{0x4, "ldr", csasm_ldr},
	{0x5, "lbl", csasm_lbl},
	{0x6, "jmp", csasm_jmp},
	{0x7, "inp", csasm_inp}
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
			buffered_length = buffered_length * 2;
			tokens.tokens = realloc(tokens.tokens, sizeof(tkn_t) * buffered_length);
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

int process_tokens(tknarr_t token_array, csparams_t* params)
{
	int lbl_opcode = str_opcode("lbl");
	tkn_t* tokens = token_array.tokens;
	size_t length = token_array.length;

	// Preprocessing
	for(params->line = 0; params->line < length; params->line++)
	{
		tkn_t token = tokens[params->line];
		if(token.opcode == lbl_opcode)
		{
			add_label(params->line, token.data.operand);
		}
	}
	// Runtime
	params->line = 0;
	for(params->line = 0; params->line < length; params->line++)
	{
		tkn_t token = tokens[params->line];
		CSASM_TKNS[token.opcode].def_func(token.data, params);
	}
	// Postprocessing
	for(size_t i = 0; i < length; i++)
	{
		free(tokens[i].data.src);
	}
	free(tokens);
	free(csasm_labels.lined);
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
	csparams_t params;
	process_tokens(token_array, &params);
	free(file_contents);
	return 0;
}
