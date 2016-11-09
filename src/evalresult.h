
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

inline unsigned char isError(const EvalResult res);

inline EvalResult integralResult(const long long value);

inline EvalResult fractionalResult(const double value);

inline EvalResult syntaxErrorResult(const size_t pos);

inline EvalResult divisionErrorResult();

EvalResult add(const EvalResult op1, const EvalResult op2);

EvalResult subtract(const EvalResult op1, const EvalResult op2);

EvalResult multiply(const EvalResult op1, const EvalResult op2);

EvalResult divide(const EvalResult op1, const EvalResult op2);

#endif