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

/** @file csretcode.h
 *  @brief Stores the needed
 *  @author Elliot Bewey (elliot5)
 */

#ifndef CRETCODE_H
#define CRETCODE_H

#ifndef EXIT_FAILURE
	#define _CSASM_FAILURE 1
	#define _CSASM_SUCCESS 0
#else
	#define _CSASM_FAILURE EXIT_FAILURE
	#define _CSASM_SUCCESS EXIT_SUCCESS
#endif

typedef enum CS_RETCODE
{
    CSASM_SUCCESS = _CSASM_SUCCESS,
    CSASM_FAILURE = _CSASM_FAILURE,
    CERR_INVALID_ARGS = 801,
    CERR_DIR_READ_FAIL = 802,
    CERR_TKN_MALLOC_FAIL = 805,
    CERR_UNKNOWN_OPCODE = 810,
    CERR_TOKENIZE_OPCODE_FAIL = 811
} CS_RETCODE_T;

#endif