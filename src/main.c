#include "libs/run.h"

int main() {
    Set Element = Hold();

    while (1) {
        const IO Action = HandleInput(Element.Full, Element.Home, 0);

        if (Action.Return == 1) break;

        AddSpot(&Element.Home, Action.Play);

        QuickAdd(&Element.Player1, Element.Home, Element.Player1Settings.Length);
        QuickAdd(&Element.Player2, Element.Home, Element.Player2Settings.Length);

        const GameConclude Result = GameEnding(Element.Player1, Element.Player2, Element.Player1Settings, Element.Player2Settings, Element.Full);
        OutputResult(Result, Element.Player1Settings, Element.Player2Settings, Element.Full);

        if (Result.End == 1) break;

        SwapState(&Element.Full);
    }

    Release(Element.Home.Line, &Element.Player1, &Element.Player2);

    return 0;
}