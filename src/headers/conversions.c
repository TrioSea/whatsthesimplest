#include "strings.h"
#include "conversions.h"

Product ConvertParent(const char Subject, const char ConnectLetter, const char ConnectNumber) {
    Product End = { 0 };

    if (Subject == ConnectLetter) End = (Product) {.Expectation = ConnectNumber, .Return = 1};
    if (Subject == ConnectNumber) End = (Product) {.Expectation = ConnectLetter, .Return = 1};

    return End;
}

char Convert(const char Subject, const Table Table) {
    int BindIndex = 0;

    while (BindIndex < Table.Bindings) {
        const Bind Connect = Table.Bounded[BindIndex];
        const Product End = ConvertParent(Subject, Connect.Character, Connect.Numeral);
        if (End.Return == 1) return End.Expectation;

        BindIndex++;
    }

    return 0;
}