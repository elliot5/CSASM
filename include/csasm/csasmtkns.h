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

/** @file csasmtkns.h
 *  @brief Stores the default tokens with function pointers for CSASM
 *  @author Elliot Bewey (elliot5)
 */

#ifndef CSASMTKNS_H
#define CSASMTKNS_H

#include "csasm.h"

const deftkn_t CSASM_TKNS[] = {
    {0x0, "add", csasm_add},
    {0x1, "out", csasm_out},
    {0x2, "mov", csasm_mov},
    {0x3, "set", csasm_set},
    {0x4, "ldr", csasm_ldr},
    {0x5, "lbl", csasm_empty},
    {0x6, "jmp", csasm_jmp},
    {0x7, "inp", csasm_inp}
};

const int CSASM_TKNS_LENGTH = sizeof(CSASM_TKNS) / sizeof(CSASM_TKNS[0]);

#endif