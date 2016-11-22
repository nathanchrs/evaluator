#include <string.h>
#include "evaluator.h"

/* Pengaturan kompilasi: menentukan karakter koma (pembatas desimal) */
#define DECIMAL_POINT '.'

/* Pengaturan kompilasi: jika terdefinisi, maka operator '+' uner diperbolehkan */
#define ALLOW_UNARY_POSITIVE_OPERATOR

/* Pengaturan kompilasi: jika terdefinisi, maka operator uner '+' dan/atau '-' yang tepat berurutan diperbolehkan */
#define ALLOW_CONSECUTIVE_UNARY_OPERATORS

EvalResult expression(const char *input, size_t *pos, size_t end);

/* Terminal digit */
EvalResult digit(const char *input, size_t *pos, size_t end) {
	if (*pos <= end && '0' <= input[*pos] && input[*pos] <= '9') {
		return integralResult(input[(*pos)++] - '0');
	} else {
		return syntaxErrorResult(*pos);
	}
}

/* Fractional -> Digit | Digit_Fractional */
EvalResult fractional(const char *input, size_t *pos, size_t end) {
	EvalResult res1 = digit(input, pos, end);
	if (isError(res1)) return res1;

	EvalResult res2 = fractional(input, pos, end);
	if (isError(res2)) {
		return fractionalResult(0.1 * (double) res1.integralValue); // Digit
	}

	return fractionalResult(0.1 * ((double) res1.integralValue + res2.fractionalValue)); // Digit_Fractional
}

/* Integer -> Digit | Digit_Integer */
EvalResult integer(const char *input, size_t *pos, size_t end) {
	size_t initialPos = *pos;

	EvalResult res1 = digit(input, pos, end);
	if (isError(res1)) return res1;

	EvalResult res2 = integer(input, pos, end);
	if (isError(res2)) {
		return integralResult(res1.integralValue); // Digit
	}

	long long exp = 1;
	int i;
	for (i = 0; i < (*pos)-initialPos-1; i++) exp *= 10;

	return integralResult(res1.integralValue * exp + res2.integralValue); // Digit_Integer
}

/* Number -> Integer.Fractional | Integer */
EvalResult number(const char *input, size_t *pos, size_t end) {
	EvalResult res1 = integer(input, pos, end);
	if (isError(res1)) return res1;

	if (*pos <= end && input[*pos] == DECIMAL_POINT) {
		(*pos)++;
		EvalResult res2 = fractional(input, pos, end);
		if (isError(res2)) return res2;
		return fractionalResult((double) res1.integralValue + res2.fractionalValue); // Integer.Fractional
	}
	
	return integralResult(res1.integralValue); // Integer
}

/* factorWithoutUnary -> (Expression) | Number */
EvalResult factorWithoutUnary(const char *input, size_t *pos, size_t end) {
	if (*pos <= end && input[*pos] == '(') {
		(*pos)++;
		EvalResult res = expression(input, pos, end);
		if (*pos <= end && input[*pos] == ')') {
			(*pos)++;
			return res; // (Expression)
		} else {
			return syntaxErrorResult(*pos); // Tidak ada kurung tutup yang cocok
		}
	} else {
		return number(input, pos, end); // number
	}
}

/* factor -> -factor | +factor | factorWithoutUnary */
EvalResult factor(const char *input, size_t *pos, size_t end) {
	if (*pos <= end && input[*pos] == '-') { // Operator uner negatif
		(*pos)++;
		return multiply(factor(input, pos, end), integralResult(-1));
	}

#ifdef ALLOW_UNARY_POSITIVE_OPERATOR
	else if (*pos <= end && input[*pos] == '+') { // Operator uner positif
		(*pos)++;
		return factor(input, pos, end);
	}
#endif

	return factorWithoutUnary(input, pos, end); // FactorWithoutUnary
}

/* Term -> Term*Factor | Term/Factor | Factor
   Dimodifikasi menjadi Factor_((*|/)_Factor)* untuk mencegah rekursi tanpa batas dan mempertahankan asosiativitas-kiri */
EvalResult term(const char *input, size_t *pos, size_t end) {
	EvalResult res1 = factor(input, pos, end);
	if (isError(res1)) return res1;

	EvalResult resAccumulator = res1;
	while(1) {
		if (*pos <= end && input[*pos] == '*') {
			(*pos)++;

			EvalResult res2 = factor(input, pos, end);
			if (isError(res2)) resAccumulator = res2;
			resAccumulator = multiply(resAccumulator, res2);

		} else if (*pos <= end && input[*pos] == '/') {
			(*pos)++;

			EvalResult res2 = factor(input, pos, end);
			if (isError(res2)) resAccumulator = res2;
			resAccumulator = divide(resAccumulator, res2);

		} else {
			break;
		}
	}
	return resAccumulator;
}

/* Expression -> Expression+Term | Expression-Term | Term
   Dimodifikasi menjadi Term_((+|-)_Term)* untuk mencegah rekursi tanpa batas dan mempertahankan asosiativitas-kiri */
EvalResult expression(const char *input, size_t *pos, size_t end) {
	EvalResult res1 = term(input, pos, end);
	if (isError(res1)) return res1;

	EvalResult resAccumulator = res1;
	while(1) {
		if (*pos <= end && input[*pos] == '+') {
			(*pos)++;

			EvalResult res2 = term(input, pos, end);
			if (isError(res2)) resAccumulator = res2;
			resAccumulator = add(resAccumulator, res2);

		} else if (*pos <= end && input[*pos] == '-') {
			(*pos)++;

			EvalResult res2 = term(input, pos, end);
			if (isError(res2)) resAccumulator = res2;
			resAccumulator = subtract(resAccumulator, res2);

		} else {
			break;
		}
	}
	return resAccumulator;
}

/* Fungsi pembungkus untuk mempermudah pemanggilan; menambahkan beberapa pengecekan awal dan akhir */
EvalResult evaluate(const char *input) {
	size_t pos = 0;
	size_t length = strlen(input);

	#ifndef ALLOW_CONSECUTIVE_UNARY_OPERATORS
		// Menggagalkan evaluasi apabila terdapat operator uner '+' dan/atau '-' yang tepat bersebelahan
		if (length > 1) {
			size_t i;
			for (i = 1; i < length; i++) {
				if ((input[i] == '+' || input[i] == '-') && (input[i-1] == '+' || input[i-1] == '-')) {
					return syntaxErrorResult(i);
				}
			}
		}
	#endif

	EvalResult res = expression(input, &pos, length);

	// Menggagalkan evaluasi apabila masih ada karakter yang belum terproses atau berlebih
	if (pos != length) {
		return syntaxErrorResult(pos);
	}
	return res;
}
