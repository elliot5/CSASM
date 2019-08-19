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

/** @file csasm.h
 *  @brief Handles loading, tokenization, output and processing of CSASM
 *
 *  @author Elliot Bewey (elliot5)
 */

#ifndef CSASM_H
#define CSASM_H

#ifndef CSASM_MEM_SIZE
	/** @brief The allocated size to the csasm memory object */
	#define CSASM_MEM_SIZE 255
#endif

#ifdef CSASM_OVERRIDE_DERRNO
	#define CSASM_FAILURE 1
	#define CSASM_SUCCESS 0
#else
	#define CSASM_FAILURE EXIT_FAILURE
	#define CSASM_SUCCESS EXIT_SUCCESS
#endif

/** @brief When defined, the values used in CSASM will be of type 'long long' instead of 'long' */
#define CSASM_DLONG_NUMBERS

#ifdef CSASM_DLONG_NUMBERS
	typedef unsigned long long indexnum_t ;
	typedef long long valuenum_t;
#else
	typedef unsigned long indexnum_t ;
	typedef long valuenum_t;
#endif

/**
 * \brief Contains line data used for labels
 */
typedef struct lined
{
	/** The label name */
	char* label;
	/** The line number */
	indexnum_t line;
} lined_t;

/**
 * \brief Stores a line data pointer along with its length
 */
typedef struct linedarr
{
	/** The line data pointer */
	lined_t* lined;
	/** The number of line data items contained in the line data pointer */
	long long length;
} linedarr_t;

/**
 * \brief Stores the needed data/parameters for use in token processing
 */
typedef struct csparams
{
	unsigned long long line;
	long long acc;
	long long memory[CSASM_MEM_SIZE];
	linedarr_t lined;
} csparams_t;

/**
 * \brief Stores token data for use in tokens
 */
typedef struct tknd
{
	char* operand;
	char* src;
} tknd_t;

/**
 * \brief Stores the definition for token data
 */
typedef struct deftkn
{
	const unsigned int icode;
	const char* scode;
	int (*def_func)(tknd_t, csparams_t*);
} deftkn_t;

/**
 * \brief Base token struct, stores token data and opcode
 */
typedef struct tkn
{
	unsigned int opcode;
	tknd_t data;
} tkn_t;

/**
 * \brief Token array struct storing length and token pointer
 */
typedef struct tknarr
{
	tkn_t* tokens;
	long length;
	long buffered_length;
} tknarr_t;

extern int csasm_add(tknd_t data, csparams_t* params);
extern int csasm_out(tknd_t data, csparams_t* params);
extern int csasm_mov(tknd_t data, csparams_t* params);
extern int csasm_ldr(tknd_t data, csparams_t* params);
extern int csasm_set(tknd_t data, csparams_t* params);
extern int csasm_lbl(tknd_t data, csparams_t* params);
extern int csasm_jmp(tknd_t data, csparams_t* params);
extern int csasm_inp(tknd_t data, csparams_t* params);

extern csparams_t gen_params();
extern int cerr_print(void);

extern int add_label(long line, char* label, linedarr_t* linearr);
extern int get_label_line(char* label, linedarr_t* linearr);

/**
 * \brief Reads the contents of a file into a char*
 * 
 * @param dir The directory of the file to be read
 * 
 * \returns The contents of the file
 */
extern char* open_file(const char* dir);

/**
 * \brief Returns the opcode value from given string
 * 
 * This function takes opcodes such as "add" and returns
 * the relevent integer that represents the opcode
 * 
 * @param str The string representation of the opcode
 * 
 * \returns The integer representation of the opcode
 */
extern int str_opcode(const char* str);

/**
 * \brief Returns the opcode value from given string
 * 
 * This function takes opcodes such as "add" and returns
 * the relevent integer that represents the opcode
 * 
 * @param str The string representation of the opcode
 * 
 * \returns The integer representation of the opcode
 */
extern void print_token(const tkn_t token);


extern tkn_t tokenize(char* str);

extern tknarr_t tokenize_lines(char* str);


extern int process_tokens(tknarr_t arr, csparams_t* params);

#endif
