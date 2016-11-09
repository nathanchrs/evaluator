# evaluator
Parser ekspresi aritmatika sederhana berjenis recursive-descent untuk tugas 2 mata kuliah IF2220 Teori Bahasa Formal dan Otomata.
By nathanchrs & leecopper15.

## Struktur direktori

- `src`: kode program
- `bin`: hasil kompilasi
- `doc`: dokumentasi dan laporan

## Cara kompilasi & menjalankan program

Pre-requisite: GCC, GNU Make

1. Masuk ke direktori utama program.
2. Jalankan perintah `make`.
3. Masuk ke direktori `bin`.
4. Jalankan program `evaluator` untuk memulai program.

## TODO

- Kasus 1+-5 masih diperbolehkan, cek apakah seharusnya boleh atau tidak
- Angka dengan awalan 0 masih diperbolehkan (ketika di-parse, awalan 0 hilang), cek apakah diperbolehkan