#include <stdio.h>
#include "stringutils.h"
#include "evaluator.h"


int main() {

	printf("\n");
	printf("EVALUATOR EKSPRESI ARITMATIKA\n");
	printf("=============================\n");
	printf("Tugas 2 IF2220 Teori Bahasa Formal dan Otomata, November 2016\n");
	printf("13515001 - Jonathan Christopher\n");
	printf("13515079 - Nicholas Thie\n");
	printf("Masukkan ekspresi aritmatika atau [exit] untuk keluar.\n\n");
	
	while (1) {
		printf(">> ");

		char *input;
		input = StringUtils_scan(stdin, '\n');

		if (StringUtils_strcmpi(input, "keluar") == 0 || StringUtils_strcmpi(input, "exit") == 0) break;

		EvalResult res = evaluate(input);
		if (res.resultType == INTEGRAL) {
			printf("%lld\n", res.integralValue);
		} else if (res.resultType == FRACTIONAL) {
			printf("%lf\n", res.fractionalValue);
		} else if (res.resultType == SYNTAX_ERROR) {
			printf("Syntax error at position %d.\n", res.errorPos+1);
		} else if (res.resultType == DIVISION_ERROR) {
			printf("Division by zero error.\n");
		}

		StringUtils_deallocate(input);
	}

	printf("\n");
	return 0;
}
