

#ifndef CSASM_H
#define CSASM_H

#define MEM_SIZE 255

// Registers / Memory / Metadata
typedef struct csparams
{
	unsigned long line;
	long acc;
	long memory[MEM_SIZE];
} csparams_t;

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
	int (*def_func)(tknd_t, csparams_t*);
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

// Line Data
typedef struct lined
{
	char* label;
	long line;
} lined_t;

// Line Array Data
typedef struct linedarr
{
	lined_t* lined;
	long length;
} linedarr_t;

extern int csasm_add(tknd_t data, csparams_t* csp);
extern int csasm_out(tknd_t data, csparams_t* csp);
extern int csasm_mov(tknd_t data, csparams_t* csp);
extern int csasm_ldr(tknd_t data, csparams_t* csp);
extern int csasm_set(tknd_t data, csparams_t* csp);
extern int csasm_lbl(tknd_t data, csparams_t* csp);
extern int csasm_jmp(tknd_t data, csparams_t* csp);
extern int csasm_inp(tknd_t data, csparams_t* csp);

extern int cerr_print(void);

extern int add_label(long line, char* label);
extern int get_label(char* label);

extern char* open_file(const char* dir);
extern int str_opcode(const char* str);
extern void print_token(const tkn_t token);
extern tkn_t tokenize(char* str);
extern tknarr_t tokenize_lines(char* str);
extern int process_tokens(tknarr_t);

#endif
