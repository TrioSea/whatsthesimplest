#ifndef WITSPG_CONVERSIONS_H
#define WITSPG_CONVERSIONS_H

typedef struct {
    char Expectation;
    _Bool Return;
} Product;

typedef struct {
    char Character;
    char Numeral;
} Bind;

Product ConvertParent(char Subject, char ConnectLetter, char ConnectNumber);
_Bool Convert(char Subject);

#endif //WITSPG_CONVERSIONS_H
