/* chbs.c
* 
* Generate passwords using the method outlined in XKCD 936, i.e. a
* random selection of words from a dictionary.
* 
* Author: Kuro68k
* Licence: GPLv3
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "csprng/csprng.h"
#include "cmdargs.h"

char* wordlist_file;
uint32_t num_words = 4;
bool duplicates_allowed = false;
bool verbose = false;
uint32_t repetitions = 1;

void clean_string(char* s)
{
	while ( (*s > 32) &&			// ASCII space and non-printables
			(*s != 127) &&			// ASCII DEL
			((uint8_t)(*s) < 255))	// ASCII non printable
	{
		s++;
	}
	*s = '\0';
}

int main(int argc, char* argv[])
{
	CSPRNG rng = csprng_create();
	if (!rng)
	{
		fprintf(stderr, "Failed to initialize the OS CSPRNG.\n");
		return 1;
	}

	if (!cmdargs_parse(argc, argv))
	{
		cmdargs_print_help("chbs");
		return 1;
	}
	if ((num_words == 0) || (repetitions == 0))
	{
		fprintf(stderr, "Number of words and number of repetitions cannot be zero.\n");
		return 1;
	}

	if (verbose)
	{
		printf("Wordlist file:   \"%s\"\n", wordlist_file);
		printf("Number of words: %u\n", num_words);
		printf("Duplicates:      %s\n", duplicates_allowed ? "Allowed" : "Not allowed");
	}

	FILE* fp = fopen(wordlist_file, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "Unable to open \"%s\"\n", wordlist_file);
		return 1;
	}

	// read the number of lines in the file
	uint32_t num_lines = 0;

	char temp[256];
	while (!feof(fp))
	{
		num_lines++;
		fgets(temp, sizeof(temp), fp);
	}
	if (num_lines == 0)
	{
		fclose(fp);
		fprintf(stderr, "Wordlist has zero lines.\n");
		return 1;
	}
	if (num_lines < 1000)
	{
		printf("WARNING: Wordlist is very short (%u lines).\n", num_lines);
		printf("At least 1000 lines is recommended.\n");
	}
	
	if (verbose)
		printf("Number of lines: %u\n", num_lines);
	num_lines -= 1;	// line count starts from zero

	// generate a mask to make selecting a random word more efficient
	uint32_t rng_mask = 1;
	while ((rng_mask != UINT32_MAX) && (num_lines > rng_mask))
	{
		rng_mask <<= 1;
		rng_mask |= 1;
	}
	if (verbose)
		printf("Mask:            %u\n", rng_mask);

	// generate password(s)
	for (uint32_t reps = 0; reps < repetitions; reps++)
	{
		// select random lines
		uint32_t random_lines[num_words];
		for (uint32_t i = 0; i < num_words;)
		{
			uint32_t r;
			do
			{
				csprng_get(rng, &r, sizeof(r));
				r &= rng_mask;
			} while (r > num_lines);
			random_lines[i] = r;

			if (!duplicates_allowed && (i > 0))
			{
				bool duplicate = false;
				for (uint32_t j = 0; j < i - 1; j++)
				{
					if (random_lines[j] == random_lines[i])
					{
						duplicate = true;
						break;
					}
				}
				if (!duplicate)
					i++;
			}
			else
				i++;
		}

		if (verbose)
		{
			for (uint32_t i = 0; i < num_words; i++)
				printf("Word %u from line %u\n", i + 1, random_lines[i]);
		}

		// print the randomly selected words
		fseek(fp, 0, SEEK_SET);
		uint32_t line_no = 0;
		while (!feof(fp))
		{
			fgets(temp, sizeof(temp), fp);
			for (uint32_t i = 0; i < num_words; i++)
			{
				if (random_lines[i] == line_no)
				{
					clean_string(temp);
					printf("%s ", temp);
				}
			}
			line_no++;
		}
		printf("\n");
	}

	return 0;
}
