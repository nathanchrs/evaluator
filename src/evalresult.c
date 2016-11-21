
#include "evalresult.h"

inline unsigned char isError(EvalResult res) {
	return res.resultType == SYNTAX_ERROR || res.resultType == DIVISION_ERROR;
}

inline EvalResult integralResult(long long value) {
	EvalResult res;
	res.resultType = INTEGRAL;
	res.integralValue = value;
	return res;
}

inline EvalResult fractionalResult(double value) {
	EvalResult res;
	res.resultType = FRACTIONAL;
	res.fractionalValue = value;
	return res;
}

inline EvalResult syntaxErrorResult(size_t pos) {
	EvalResult res;
	res.resultType = SYNTAX_ERROR;
	res.errorPos = pos;
	return res;
}

inline EvalResult divisionErrorResult() {
	EvalResult res;
	res.resultType = DIVISION_ERROR;
	return res;
}

EvalResult add(EvalResult op1, EvalResult op2) {
	if (isError(op1)) return op1;
	if (isError(op2)) return op2;

	if (op1.resultType == INTEGRAL && op2.resultType == INTEGRAL) {
		return integralResult(op1.integralValue + op2.integralValue);
	} else if (op1.resultType == FRACTIONAL && op2.resultType == INTEGRAL) {
		return fractionalResult(op1.fractionalValue + (double) op2.integralValue);
	} else if (op1.resultType == INTEGRAL && op2.resultType == FRACTIONAL) {
		return fractionalResult((double) op1.integralValue + op2.fractionalValue);
	} else {
		return fractionalResult(op1.fractionalValue - op2.fractionalValue);
	}
}

EvalResult subtract(EvalResult op1, EvalResult op2) {
	if (isError(op1)) return op1;
	if (isError(op2)) return op2;

	if (op1.resultType == INTEGRAL && op2.resultType == INTEGRAL) {
		return integralResult(op1.integralValue - op2.integralValue);
	} else if (op1.resultType == FRACTIONAL && op2.resultType == INTEGRAL) {
		return fractionalResult(op1.fractionalValue - (double) op2.integralValue);
	} else if (op1.resultType == INTEGRAL && op2.resultType == FRACTIONAL) {
		return fractionalResult((double) op1.integralValue - op2.fractionalValue);
	} else {
		return fractionalResult(op1.fractionalValue - op2.fractionalValue);
	}
}

EvalResult multiply(EvalResult op1, EvalResult op2) {
	if (isError(op1)) return op1;
	if (isError(op2)) return op2;

	if (op1.resultType == INTEGRAL && op2.resultType == INTEGRAL) {
		return integralResult(op1.integralValue * op2.integralValue);
	} else if (op1.resultType == FRACTIONAL && op2.resultType == INTEGRAL) {
		return fractionalResult(op1.fractionalValue * (double) op2.integralValue);
	} else if (op1.resultType == INTEGRAL && op2.resultType == FRACTIONAL) {
		return fractionalResult((double) op1.integralValue * op2.fractionalValue);
	} else {
		return fractionalResult(op1.fractionalValue * op2.fractionalValue);
	}
}

EvalResult divide(EvalResult op1, EvalResult op2) {
	if (isError(op1)) return op1;
	if (isError(op2)) return op2;
	if (op2.resultType == INTEGRAL && op2.integralValue == 0) return divisionErrorResult();
	if (op2.resultType == FRACTIONAL && (op2.fractionalValue == 0.0 || op2.fractionalValue == -0.0)) return divisionErrorResult();

	if (op1.resultType == INTEGRAL && op2.resultType == INTEGRAL) {
		if (op1.integralValue % op2.integralValue == 0) {
			return integralResult(op1.integralValue / op2.integralValue);
		} else {
			return fractionalResult((double) op1.integralValue / (double) op2.integralValue);
		}
	} else if (op1.resultType == FRACTIONAL && op2.resultType == INTEGRAL) {
		return fractionalResult(op1.fractionalValue / (double) op2.integralValue);
	} else if (op1.resultType == INTEGRAL && op2.resultType == FRACTIONAL) {
		return fractionalResult((double) op1.integralValue / op2.fractionalValue);
	} else {
		return fractionalResult(op1.fractionalValue / op2.fractionalValue);
	}
}
