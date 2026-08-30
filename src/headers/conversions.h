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
char Convert(char Subject, int Bindings, const Bind* Bounded);
char TwoWayConversion(char Subject, char CharacterBind1, char NumeralBind1, char CharacterBind2, char NumeralBind2);

#endif // WITSPG_CONVERSIONS_H
