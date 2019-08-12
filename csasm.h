

#ifndef CSASM_H
#define CSASM_H

// Token Data
typedef struct tknd
{
	char* operand;
	char* src;
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
	unsigned int opcode;
	tknd_t data;
} tkn_t;

// Token Array
typedef struct tknarr
{
	tkn_t* tokens;
	long length;
	long buffered_length;
} tknarr_t;

extern int csasm_add(tknd_t data);
extern int csasm_out(tknd_t data);
extern int csasm_mov(tknd_t data);
extern int csasm_ldr(tknd_t data);
extern int csasm_set(tknd_t data);

extern char* open_file(const char* dir);
extern int str_opcode(const char* str);
extern void print_token(const tkn_t token);
extern tkn_t tokenize(char* str);
extern tknarr_t tokenize_lines(char* str);

#endif
