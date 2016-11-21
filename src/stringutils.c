#include "stringutils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Baca file dari posisi saat ini hingga terbaca sebuah karakter delimiter.
   String yang dihasilkan dialokasikan secara dinamis. Mengembalikan NULL jika alokasi gagal. */
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

		// Perbesar kapasitas string jika ukuran akan melebihi kapasitas
		if (len == capacity) {
			str = realloc(str, sizeof(char) * (capacity + STRING_ALLOC_BLOCK_SIZE));
			if (!str) return str;
			capacity += STRING_ALLOC_BLOCK_SIZE;
		}
	}

	// Set karakter kosong di akhir
	str[len] = 0;

	// Ubah kapasitas string menjadi sesuai dengan ukurannya
	return realloc(str, sizeof(char) * (len + 1));
}

/* Dealokasi string */
void StringUtils_deallocate(char *str) {
	free(str);
}

/* Perbandingan string yang tidak memperhatikan huruf besar/kecil,
   mengembalikan nilai < 0 jika karakter pertama yang tidak cocok di str1 < str2,
   0 jika kedua string sama, atau > 0 jika tidak keduanya. */
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
