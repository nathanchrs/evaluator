#include "stringutils.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* Read string from the input file from the current position until the delimiter character.
   The resulting string is dynamically allocated. Returns null on allocation failure. */
char* StringUtils_scan(FILE *fin, const char delim) {
	char inp;
	size_t len = 0;
	size_t capacity = STRING_ALLOC_BLOCK_SIZE;
	char *str = malloc(sizeof(char)*capacity);
	if (!str) return str;

	while (1) {
		inp = fgetc(fin);
		if (inp == EOF || inp == delim) break;
		str[len] = inp;
		len++;

		// Resize if actual length + 1 exceeds capacity
		if (len == capacity) {
			str = realloc(str, sizeof(char) * (capacity + STRING_ALLOC_BLOCK_SIZE));
			if (!str) return str;
			capacity += STRING_ALLOC_BLOCK_SIZE;
		}
	}

	// Set ending null character
	str[len] = 0;

	// Resize to exact string length, then return
	return realloc(str, sizeof(char) * (len + 1));
}

/* Returns a pointer to a clone of string str
   The resulting string is dynamically allocated. Returns null on allocation failure. */
char* StringUtils_clone(const char *str) {
	char *res = malloc(sizeof(char) * (strlen(str) + 1));
	if (res) strcpy(res, str);
	return res;
}

/* Creates a new string containing the concatenation of str1 and str2. Returns null on allocation failure. */
char* StringUtils_concat(const char *str1, const char *str2) {
	size_t slen1 = strlen(str1);
	size_t slen2 = strlen(str2);
	char *res = malloc(sizeof(char) * (slen1 + slen2 + 1));
	if (res) {
		strcpy(res, str2);
		strcpy(res + slen1, str2);
	}
	return res;
}

void StringUtils_deallocate(char *str) {
	free(str);
}

/* Case-insensitive string comparison,
   returns <0 if first character that does not match in str1 < str2, 0 if both strings are equal, or >0 otherwise */
int StringUtils_strcmpi(const char *str1, const char *str2) {
	const char *p1 = str1;
	const char *p2 = str2;
	char c1, c2;
	do {
		c1 = (char) tolower((int) *p1);
		c2 = (char) tolower((int) *p2);
		p1++;
		p2++;
	} while (c1 == c2 && c1 != 0);
	return c1-c2;
}

/* Checks whether all characters in a string (not including ending null byte) satisfies a condition.
   checker is a pointer to a function which accepts an integer representing a character,
   and outputs a non-zero integer if the character satisfies the condition,
   or 0 otherwise (e.q. ctype's isdigit, isspace, islower functions. */
unsigned char StringUtils_check(const char *str, int (*checker)(int)) {
	const char *p = str;
	while ((*p) != 0) {
		if (!(*checker)((int) *p)) return 1;
		p++;
	}
	return 0;
}