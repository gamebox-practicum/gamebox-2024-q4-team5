#include "SKUtils.h"

FIntPoint SKUtils::GameToAI(FIntPoint index)
{
    return FIntPoint{index.Y, index.X};
}

FCellIndex SKUtils::AIToGame(FCellIndex index)
{
    return FCellIndex{index.X, index.Y};
}
