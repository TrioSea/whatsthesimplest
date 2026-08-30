#include "strings.h"
#include "conversions.h"

Product ConvertParent(const char Subject, const char ConnectLetter, const char ConnectNumber) {
    Product End = { 0 };

    if (Subject == ConnectLetter) End = (Product) {.Expectation = ConnectNumber, .Return = 1};
    if (Subject == ConnectNumber) End = (Product) {.Expectation = ConnectLetter, .Return = 1};

    return End;
}

char Convert(const char Subject, const int Bindings, const Bind* Bounded) {
    int BindIndex = 0;

    while (BindIndex < Bindings) {
        const Bind Connect = Bounded[BindIndex];
        const Product End = ConvertParent(Subject, Connect.Character, Connect.Numeral);
        if (End.Return == 1) return End.Expectation;

        BindIndex++;
    }

    return 0;
}

char TwoWayConversion(const char Subject, const char CharacterBind1, const char NumeralBind1, const char CharacterBind2, const char NumeralBind2) {
    const int Bindings = 2;
    Bind* Bounded = calloc(Bindings, sizeof(Bind));

    Bounded[0] = (Bind) {
        .Character = CharacterBind1,
        .Numeral = NumeralBind1
    };

    Bounded[1] = (Bind) {
        .Character = CharacterBind2,
        .Numeral = NumeralBind2
    };

    const char Given = Convert(Subject, Bindings, Bounded);
    free(Bounded);

    return Given;
}