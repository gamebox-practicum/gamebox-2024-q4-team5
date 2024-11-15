#include "SKUtils.h"

FIndex2D SKUtils::GameToAI(FIndex2D index)
{
    return FIndex2D{index.Y, index.X};
}

FCellIndex SKUtils::AIToGame(FCellIndex index)
{
    return FCellIndex{index.X, index.Y};
}
