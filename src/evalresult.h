
#ifndef EVALRESULT_H
#define EVALRESULT_H

#include <string.h>

typedef enum {
	INTEGRAL, FRACTIONAL, SYNTAX_ERROR, DIVISION_ERROR
} EvalResultType;

typedef struct {
	EvalResultType resultType;
	double fractionalValue;
	long long integralValue;
	int errorPos;
} EvalResult;


#include "evalresult.h"

inline unsigned char isError(EvalResult res);

inline EvalResult integralResult(long long value);

inline EvalResult fractionalResult(double value);

inline EvalResult syntaxErrorResult(size_t pos);

inline EvalResult divisionErrorResult();

EvalResult add(EvalResult op1, EvalResult op2);

EvalResult subtract(EvalResult op1, EvalResult op2);

EvalResult multiply(EvalResult op1, EvalResult op2);

EvalResult divide(EvalResult op1, EvalResult op2);

#endif