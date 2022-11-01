#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
	ARGTYPE_INT8,
	ARGTYPE_INT16,
	ARGTYPE_INT32,
	ARGTYPE_INT64,
	ARGTYPE_UINT8,
	ARGTYPE_UINT16,
	ARGTYPE_UINT32,
	ARGTYPE_UINT64,
	ARGTYPE_FLOAT,
	ARGTYPE_DOUBLE,
	ARGTYPE_CHAR,
	ARGTYPE_STRING,
	ARGTYPE_BOOL,
} ARGTYPE_t;


/*	Define your arguments here
*/
extern char* wordlist_file;
extern uint32_t num_words;
extern bool duplicates_allowed;
extern bool verbose;
extern uint32_t repetitions;

#define	ARGUMENT_TABLE \
	{ &wordlist_file, ARGTYPE_STRING, "wordlist_file", "Wordlist file name", true }

#define	OPTION_TABLE \
	{ { &num_words, ARGTYPE_UINT32, "num_words", "Number of random words to select", false }, 'c', "count" }, \
	{ { &duplicates_allowed, ARGTYPE_BOOL, "duplicates_allowed", "Allow words to be used more than once", false }, 'd', "duplicates" }, \
	{ { &verbose, ARGTYPE_BOOL, "verbose", "Print extra information", false }, 'v', "verbose" }, \
	{ { &repetitions, ARGTYPE_UINT32, "repetitions", "Number of passwords to generate", false }, 'r', "repetitions" }


extern bool cmdargs_parse(int argc, char *argv[]);
extern void cmdargs_print_help(char *app_name);
