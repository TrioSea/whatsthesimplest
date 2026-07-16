#include "conversions.h"

Product ConvertParent(const char Subject, const char ConnectLetter, const char ConnectNumber) {
    Product End = { 0 };

    if (Subject == ConnectLetter) End = (Product) {.Expectation = ConnectNumber, .Return = 1};
    if (Subject == ConnectNumber) End = (Product) {.Expectation = ConnectNumber, .Return = 1};

    return End;
}

_Bool Convert(const char Subject) {
    Product End;
    Bind Connect;

    Connect = (Bind) {.Character = '0', .Numeral = 0};
    End = ConvertParent(Subject, Connect.Character, Connect.Numeral);
    if (End.Return == 1) return End.Expectation;

    Connect = (Bind) {.Character = 'X', .Numeral = 0};
    End = ConvertParent(Subject, Connect.Character, Connect.Numeral);
    if (End.Return == 1) return End.Expectation;

    return 0;
}