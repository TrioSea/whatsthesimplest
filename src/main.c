#include "headers/run.h"

int main() {
    Set Element = Hold();
    MainTables ElementTables = Tables();

    Element.Home.BotLevel = 3;
    _Bool ClientPlayingStarter = (_Bool) 1;

    printf("Begin Play\n");

    _Bool PlayWithBot = (_Bool) 1;
    char AutoPlay = 'O'; // either X, O, or E if not 0

    _Bool quit = (_Bool) 0;

    while (!quit) {
        // Input Handled
        char Override = AutoPlay;

        if (AND(ClientPlayingStarter != Element.Full, PlayWithBot)) {
            const char Potential = GameBot(Element, 1, 0, ElementTables.Table4);
            Override = Potential;
        }

        const IO Action = HandleInput(Element, ClientPlayingStarter, 0, Override, 0, ElementTables.Table2, ElementTables.Table4);

        if (Action.Return != 1) {
            UpdateGame(&Element, Action.Play, ElementTables.Table4);

            // Game check
            const GameConclude Result = GameEnding(Element.Player1, Element.Player2, Element.Player1Settings, Element.Player2Settings, ClientPlayingStarter);

            if (Result.End != 1) {
                // Continue
                SwapState(&Element.Full);

                continue;
            }

            OutputResult(Result, Element.Player1Settings, Element.Player2Settings, Element.Full, ClientPlayingStarter, ElementTables.Table2, ElementTables.Table4);
        }

        printf("\nEnd Sequence: \n    ");
        PrintLine(Element.Home, ElementTables.Table4);
        printf("\n");

        quit = 1;
    }

    Release(Element.Home.Line, &Element.Player1, &Element.Player2);

    return 0;
}