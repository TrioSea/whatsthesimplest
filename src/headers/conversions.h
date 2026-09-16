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

typedef struct {
    int Bindings;
    Bind* Bounded;
} Table;

Product ConvertParent(char Subject, char ConnectLetter, char ConnectNumber);
char Convert(char Subject, Table Table);

#endif // WITSPG_CONVERSIONS_H
