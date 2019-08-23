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

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include "cmocka.h"
#include "csasm/csasm.h"
#include "csasm/csasmtkns.h"

void test_csasm_add(void ** state)
{
    // Set accumulator to 7
    csparams_t params = {
        .acc = 7
    };

    // Set memory address 0x13 (19) to 29
    params.memory[19] = 29;

    // Set operand to memory address 0x13 (19)
    tknd_t data = {
        .operand = "0x13"
    };
    
    // Call add function
    csasm_add(data, &params);

    // 29 + 7 = 36 in accumulator
    assert_int_equal(params.acc, 36);
}

void test_csasm_mov(void ** state)
{
    // Set accumulator to 254
    csparams_t params = {
        .acc = 254
    };

    // Set memory address 0xFA (250) to 40
    params.memory[250] = 40;

    // Set operand to memory address 0x13 (19)
    tknd_t data = {
        .operand = "0xFA"
    };
    
    // Call add function
    csasm_mov(data, &params);

    // Check 0xFA was changed from 40 to 254
    assert_int_equal(params.memory[250], 254);
}

void test_csasm_program_add()
{
    size_t tknarr_length = -1;
	tkn_t* tknarr;

    char* code = malloc(42);
    strcpy(code, "\nset 5\nmov 0x2\nset 7\nadd 0x2\nmov 0x3");

	int tstatus = tokenize_lines(code,
        CSASM_TKNS, CSASM_TKNS_LENGTH, &tknarr_length, &tknarr);
    assert_int_equal(tstatus, 0);
    assert_true(tknarr_length >= 0);
    
    csparams_t params = gen_params();
    int pstatus = process_tokens(&params, tknarr, tknarr_length, CSASM_TKNS, CSASM_TKNS_LENGTH);
    free_tokens(tknarr, tknarr_length);

    assert_int_equal(pstatus, 0);
    assert_int_equal(params.memory[3], 12);
    free(code);
}

int main(void)
{
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(test_csasm_add),
        cmocka_unit_test(test_csasm_mov),
        cmocka_unit_test(test_csasm_program_add)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}