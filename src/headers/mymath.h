#ifndef CHESS_MYMATH_H
#define CHESS_MYMATH_H

extern double sqrt (double __x);
extern double __sqrt (double __x);

__attribute__ ((__simd__ ("notinbranch")))
extern double pow (double __x, double __y) __THROW;
extern double __pow (double __x, double __y) __THROW;

extern double floor (double __x) __attribute__ ((__const__));
extern double __floor (double __x) __attribute__ ((__const__));

extern double ceil (double __x) __attribute__ ((__const__));
extern double __ceil (double __x) __attribute__ ((__const__));

extern double fabs (double __x) __attribute__ ((__const__));
extern double __fabs (double __x) __attribute__ ((__const__));

extern double fmod (double __x, double __y);
extern double __fmod (double __x, double __y);

#endif //CHESS_MYMATH_H