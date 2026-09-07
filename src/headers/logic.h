#ifndef MATH_H_
#define MATH_H_

// handle with _Bool and say == 1 or == 0 for true/false checks

extern _Bool Invert(_Bool N);

extern _Bool Inequality(const _Bool* X, _Bool Check);
extern _Bool Equality(const _Bool* X, _Bool Check);

extern _Bool InvertedAND(_Bool A, _Bool B);
extern _Bool AND(_Bool A, _Bool B);
extern _Bool InvertedInclusiveOR(_Bool A, _Bool B);
extern _Bool InclusiveOR(_Bool A, _Bool B);
extern _Bool InvertedExclusiveOR(_Bool A, _Bool B);
extern _Bool ExclusiveOR(_Bool A, _Bool B);

extern unsigned char AbsoluteValue_hhu(signed char X);
extern signed char SignedSwap_hh(unsigned char X);
extern signed char ClampValue_hhd(signed char X, signed char Minimum, signed char Maximum);

#endif // MATH_H_