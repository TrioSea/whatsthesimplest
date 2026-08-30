#include "headers/run.h"

int main() {
    Set Element = Hold();

    const _Bool ClientPlayingStarter = 0;

    while (1) {
        // Input Handled
        char Override = 0;
        if (ClientPlayingStarter != Element.Full) Override = 'X';

        const IO Action = HandleInput(Element.Full, Element.Home, 0, Override);

        if (Action.Return == 1) break;

        // Game Updated
        AddSpot(&Element.Home, Action.Play);

        QuickAdd(&Element.Player1, Element.Home, Element.Player1Settings.Length);
        QuickAdd(&Element.Player2, Element.Home, Element.Player2Settings.Length);

        // Game check
        const GameConclude Result = GameEnding(Element.Player1, Element.Player2, Element.Player1Settings, Element.Player2Settings, Element.Full);
        OutputResult(Result, Element.Player1Settings, Element.Player2Settings, Element.Full);

        if (Result.End == 1) break;

        // Continue
        SwapState(&Element.Full);
    }

    Release(Element.Home.Line, &Element.Player1, &Element.Player2);

    return 0;
}