#include "headers/run.h"

int main() {
    Set Element = Hold();

    _Bool ClientPlayingStarter = 0;
    signed char EngineLevel = 1;

    BotMemory Engine = (BotMemory) {
        .DoO = 1
    };

    printf("Begin Play\n");

    while (1) {
        // Input Handled
        char Override = 0;

        if (ClientPlayingStarter != Element.Full) {
            const char Potential = GameBot(EngineLevel, Element.Home.Line, &Engine);
            Override = Potential;
        }

        const IO Action = HandleInput(Element.Full, Element.Home, 0, Override);

        if (Action.Return == 1) break;

        // Game Updated
        AddSpot(&Element.Home, Action.Play);

        QuickAdd(&Element.Player1, Element.Home, Element.Player1Settings.Length);
        QuickAdd(&Element.Player2, Element.Home, Element.Player2Settings.Length);

        // Game check
        const GameConclude Result = GameEnding(Element.Player1, Element.Player2, Element.Player1Settings, Element.Player2Settings, ClientPlayingStarter);

        if (Result.End == 1) {
            OutputResult(Element.Home, Result, Element.Player1Settings, Element.Player2Settings, Element.Full, ClientPlayingStarter);
            break;
        };

        // Continue
        SwapState(&Element.Full);
    }

    Release(Element.Home.Line, &Element.Player1, &Element.Player2);

    return 0;
}