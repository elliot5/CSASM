/*
MIT License

Copyright (c) 2019 Elliot K Bewey

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "csasm/csasm.h"

CS_RETCODE_T cerr_print(void)
{
	fprintf(stderr, "[asm_err] ");
	return CSASM_SUCCESS;
}

int add_label(const indexnum_t line, char* label, linedarr_t* linearr)
{
	indexnum_t length = linearr->length;
	lined_t* lined = linearr->lined;
	lined = malloc(sizeof(lined_t) * length + 1);
	if(lined == NULL)
	{
		cerr_print();
		fprintf(stderr, "Failed to alloc label space\n");
		return CSASM_FAILURE;
	}
	lined_t ln_data = {
		.label = label,
		.line = line
	};
	linearr->lined[linearr->length] = ln_data;
	linearr->length += 1;
	return CSASM_SUCCESS;
}

indexnum_t get_label_line(const char* label, const linedarr_t* linearr)
{
	for(indexnum_t i = 0; i < linearr->length; i++)
	{
		if(strcmp(linearr->lined[i].label, label) == 0)
		{
			return linearr->lined[i].line;
		}
	}
	return -1;
}

CS_RETCODE_T csasm_add(tknd_t data, csparams_t* params)
{
	if(data.operand == NULL)
	{
		cerr_print();
		fprintf(stderr, "ADD requires operand ADDR\n");
		exit(CSASM_FAILURE);
	}
	long addr = strtol(data.operand, NULL, 0);
	params->acc += params->memory[addr];
	return CSASM_SUCCESS;
}

CS_RETCODE_T csasm_out(tknd_t data, csparams_t* params)
{
	#ifdef CSASM_DLONG_NUMBERS
		printf("[asm_out] %lld\n", params->acc);
	#else
		printf("[asm_out] %ld\n", params->acc);
	#endif
	return CSASM_SUCCESS;
}

CS_RETCODE_T csasm_mov(tknd_t data, csparams_t* params)
{
	if(data.operand == NULL)
	{
		cerr_print();
		fprintf(stderr, "MOV requires operand ADDR\n");
		exit(CSASM_FAILURE);
	}
	long addr = strtol(data.operand, NULL, 0);
	params->memory[addr] = params->acc;
	return CSASM_SUCCESS;
}

CS_RETCODE_T csasm_ldr(tknd_t data, csparams_t* params)
{
	if(data.operand == NULL)
	{
		cerr_print();
		fprintf(stderr, "LDR requires operand ADDR\n");
		exit(CSASM_FAILURE);
	}
	long addr = strtol(data.operand, NULL, 0); // 0 = hex
	params->acc = params->memory[addr];
	return CSASM_SUCCESS;
}

CS_RETCODE_T csasm_set(tknd_t data, csparams_t* params)
{
	if(data.operand == NULL)
	{
		cerr_print();
		fprintf(stderr, "SET requires operand VAL\n");
		exit(CSASM_FAILURE);
	}
	long val = strtol(data.operand, NULL, 10); // 10 for base ten
	params->acc= val;
	return CSASM_SUCCESS;
}

CS_RETCODE_T csasm_lbl(tknd_t data, csparams_t* params)
{
	return CSASM_SUCCESS;
}

CS_RETCODE_T csasm_jmp(tknd_t data, csparams_t* params)
{
	if(data.operand == NULL)
	{
		cerr_print();
		fprintf(stderr, "JMP requires operand LABEL\n");
		exit(CSASM_FAILURE);
	}
	int ret = get_label_line(data.operand, &params->lined);
	if(ret < 0)
	{
		cerr_print();
		fprintf(stderr, "Failed to JMP to label %s (does it exist?)\n", data.operand);
		exit(CSASM_FAILURE);
	}
	params->line = ret;
	return CSASM_SUCCESS;
}

CS_RETCODE_T csasm_inp(tknd_t data, csparams_t* params)
{
	long val = 0;
	scanf("%ld", &val);
	params->acc = val;
	return CSASM_SUCCESS;
}

CS_RETCODE_T csasm_empty(tknd_t data, csparams_t* params)
{
	return CSASM_SUCCESS;
}

char* open_file(const char* dir)
{
	const char* mode = "r";
	FILE * file_ptr;
	char* buffer = NULL;
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
			exit(EXIT_FAILURE);
		}
		fread(buffer, 1, length, file_ptr);
		fclose(file_ptr);
		buffer[length-1] = '\0';
	}
	return buffer;
}

int str_opcode(const char* str, const deftkn_t* tokens, const size_t length)
{
	for(size_t i = 0; i < length; i++)
	{
		deftkn_t curr_tkn = tokens[i];
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

CS_RETCODE_T tokenize(char* str, const deftkn_t* deftkns, const size_t deftkns_length, tkn_t* out)
{
	char* opcode = strtok_r(str, " ", &str);
	if(opcode == NULL)
	{
		cerr_print();
		fprintf(stderr, "Tokenize opcode fail\n");
		return CERR_TOKENIZE_OPCODE_FAIL;
	}
	int i_opcode = str_opcode(opcode, deftkns, deftkns_length);
	if(i_opcode < 0)
	{
		return CERR_UNKNOWN_OPCODE;
	}
	char* operand = strtok_r(NULL, " ", &str);

	tknd_t token_data = {
		.operand = operand
	};

	tkn_t token = {
		.opcode = i_opcode,
		.data = token_data
	};

	*out = token;

	return CSASM_SUCCESS;
}

CS_RETCODE_T tokenize_lines(char* str, const deftkn_t* deftkns,
	const size_t deftkns_length, size_t* tkn_length, tkn_t** out)
{
	long buffered_length = 16;
	long index = 0;
	char* tok_ptr = NULL;
	tok_ptr = strtok_r(str, "\n", &str);

	tkn_t* tokens = malloc(sizeof(tkn_t) * buffered_length);

	while(tok_ptr != NULL)
	{
		if(index >= buffered_length)
		{
			buffered_length = buffered_length * 2;
			tokens = realloc(tokens, sizeof(tkn_t) * buffered_length);
			buffered_length = buffered_length;
		}
		
		long length = strlen(tok_ptr) + 1;
		char* buffer = malloc(length);
		if(buffer == NULL)
		{
			perror("Failed to malloc tkn buffer");
			return CERR_TKN_MALLOC_FAIL;
		}
		buffer[length-1] = '\0';
		snprintf(buffer, length, "%s", tok_ptr);

		tkn_t token;
		CS_RETCODE_T token_status = tokenize(buffer, deftkns, deftkns_length, &token);
		if(token_status != 0)
		{
			if(token_status == CERR_UNKNOWN_OPCODE)
			{
				cerr_print();
				fprintf(stderr, "Unknown opcode \"%s\" on line %lu\n", buffer, index);
			}
			return token_status;
		}
		tokens[index] = token;
		buffered_length = index + 1;
		tokens[index].data.src = buffer;	
		tok_ptr = strtok_r(NULL, "\n", &str);

		index = index + 1;
		*tkn_length = index;
	}

	*out = tokens;

	return CSASM_SUCCESS;
}

csparams_t gen_params(void)
{
	linedarr_t lined_arr = {
		.length = 0,
		.lined = malloc(sizeof(lined_t))
	};
	csparams_t new_params = {
		.lined = lined_arr,
		.line = 0
	};
	return new_params;
}

CS_RETCODE_T free_tokens(tkn_t* tkns, const size_t tkns_length)
{
	for(size_t i = 0; i < tkns_length; i++)
	{
		free(tkns[i].data.src);
	}
	free(tkns);
	return CSASM_SUCCESS;
}

CS_RETCODE_T process_tokens(csparams_t* params, const tkn_t* tkns, const size_t tkns_length,
	const deftkn_t* deftkns, const size_t deftkns_length)
{
	// Temporary
	int lbl_opcode = str_opcode("lbl", deftkns, deftkns_length);

	// Preprocessing
	for(params->line = 0; params->line < tkns_length; params->line++)
	{
		tkn_t token = tkns[params->line];
		if(token.opcode == lbl_opcode)
		{
			int astatus = add_label(params->line, token.data.operand, &params->lined);
			if(astatus != 0)
			{
				return astatus;
			}
		}
	}

	// Runtime
	params->line = 0;
	for(params->line = 0; params->line < tkns_length; params->line++)
	{
		tkn_t token = tkns[params->line];
		deftkns[token.opcode].def_func(token.data, params);
	}
	free(params->lined.lined);
}

CS_RETCODE_T exit_status(const CS_RETCODE_T status) {
	if(status == 0)
	{
		printf("Program exited OK (%d)\n\r", status);
	} else {
		printf("Program exited with error code (%d)\n\r", status);
	}
	return status;
}