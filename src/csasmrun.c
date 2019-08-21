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
#include "csretcode.h"
#include "csasm.h"

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "Program requires file directory input\n");
		return exit_status(CERR_INVALID_ARGS);
	}

	char* directory = argv[1];
	char* file_contents = open_file(directory);

	if(file_contents == NULL)
	{
		fprintf(stderr, "Failed to read directory %s (does the file exist?)\n", directory);
		free(file_contents);
		return exit_status(CERR_DIR_READ_FAIL);
	}

	const deftkn_t csasm_tkns[] = {
		{0x0, "add", csasm_add},
		{0x1, "out", csasm_out},
		{0x2, "mov", csasm_mov},
		{0x3, "set", csasm_set},
		{0x4, "ldr", csasm_ldr},
		{0x5, "lbl", csasm_empty},
		{0x6, "jmp", csasm_jmp},
		{0x7, "inp", csasm_inp}
	};
	const int csasm_tkns_length = sizeof(csasm_tkns) / sizeof(csasm_tkns[0]);
	
	size_t tknarr_length = -1;
	tkn_t* tknarr;
	int tstatus = tokenize_lines(file_contents, csasm_tkns, csasm_tkns_length, &tknarr_length, &tknarr);
	if(tstatus != 0)
	{
		return exit_status(tstatus);
	}
	if(tknarr_length >= 0)
	{
		csparams_t params = gen_params();
		int pstatus = process_tokens(&params, tknarr, tknarr_length, csasm_tkns, csasm_tkns_length);
		free_tokens(tknarr, tknarr_length);
		free(file_contents);
		return exit_status(pstatus);
	} else {
		fprintf(stderr, "Tokenization failed: returned value less than 0");	
		free_tokens(tknarr, tknarr_length);
		free(file_contents);
		return exit_status(CSASM_FAILURE);
	}
	return exit_status(CSASM_SUCCESS);
}