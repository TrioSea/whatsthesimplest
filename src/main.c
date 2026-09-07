#include "headers/run.h"

int main() {
    Set Element = Hold();

    Element.Home.BotLevel = 3;
    _Bool ClientPlayingStarter = 1;

    printf("Begin Play\n");

    _Bool quit = 0;

    while (!quit) {
        // Input Handled
        char Override = 0;

        if (ClientPlayingStarter != Element.Full) {
            const char Potential = GameBot(Element, 1, 0);
            Override = Potential;
        }

        const IO Action = HandleInput(Element.Full, ClientPlayingStarter, Element, 0, Override);

        if (Action.Return != 1) {
            UpdateGame(&Element, Action.Play);

            // Game check
            const GameConclude Result = GameEnding(Element.Player1, Element.Player2, Element.Player1Settings, Element.Player2Settings, ClientPlayingStarter);

            if (Result.End != 1) {
                // Continue
                SwapState(&Element.Full);

                continue;
            }

            OutputResult(Element.Home, Result, Element.Player1Settings, Element.Player2Settings, Element.Full, ClientPlayingStarter);
        }

        printf("\nEnd Sequence: \n    ");
        PrintLine(Element.Home);
        printf("\n");

        quit = 1;
    }

    Release(Element.Home.Line, &Element.Player1, &Element.Player2);

    return 0;
}