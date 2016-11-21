#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <stdio.h>

/* Operasi yang mengakibatkan string membesar secara otomatis akan mengalokasikan string
   dalam blok-blok dengan kapasitas ini. */
#define STRING_ALLOC_BLOCK_SIZE 16

/* Baca file dari posisi saat ini hingga terbaca sebuah karakter delimiter.
   String yang dihasilkan dialokasikan secara dinamis. Mengembalikan NULL jika alokasi gagal. */
char* StringUtils_scan(FILE *fin, const char delim);

/* Dealokasi string */
void StringUtils_deallocate(char *str);

/* Perbandingan string yang tidak memperhatikan huruf besar/kecil,
   mengembalikan nilai < 0 jika karakter pertama yang tidak cocok di str1 < str2,
   0 jika kedua string sama, atau > 0 jika tidak keduanya. */
int StringUtils_strcmpi(const char *str1, const char *str2);

#endif