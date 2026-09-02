#include "headers/run.h"

int main() {
    Set Element = Hold();

    _Bool ClientPlayingStarter = 0;
    signed char EngineLevel = 1;

    printf("Begin Play\n");

    _Bool quit = 0;

    while (!quit) {
        // Input Handled
        char Override = 0;

        if (ClientPlayingStarter != Element.Full) {
            const char Potential = GameBot(EngineLevel, Element.Home);
            Override = Potential;
        }

        const IO Action = HandleInput(Element.Full, Element.Home, 0, Override);

        if (Action.Return != 1) {
            UpdateGame(&Element, Action.Play);

            // Game check
            const GameConclude Result = GameEnding(Element.Player1, Element.Player2, Element.Player1Settings, Element.Player2Settings, ClientPlayingStarter);

            if (Result.End == 1) {
                OutputResult(Element.Home, Result, Element.Player1Settings, Element.Player2Settings, Element.Full, ClientPlayingStarter);
                quit = 1;
            }
        } else quit = 1;

        // Continue
        SwapState(&Element.Full);
    }

    Release(Element.Home.Line, &Element.Player1, &Element.Player2);

    return 0;
}