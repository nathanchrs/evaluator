
#include <stdio.h>
#include "stringutils.h"
#include "evaluator.h"


int main() {
	
	char *input;
	input = StringUtils_scan(stdin, '\n');

	EvalResult res = evaluate(input);

	if (res.resultType == INTEGRAL) {
		printf("%lld\n", res.integralValue);
	} else if (res.resultType == FRACTIONAL) {
		printf("%lf\n", res.fractionalValue);
	} else if (res.resultType == SYNTAX_ERROR) {
		printf("Syntax error at position %d.\n", res.errorPos+1);
	} else if (res.resultType == DIVISION_ERROR) {
		printf("Division by zero error.\n", res.errorPos+1);
	}

	StringUtils_deallocate(input);
	return 0;
}
