#include "Player.h"
using namespace HdB;

Player::Player(void):mBlockterieUnits(0),mFoodUnits(0),mGoldUnits(0)
{
    mRes=gcnew Ressources();

    /* *********
       * TEMP  *
       ********* */
    mBlockterieUnits=1;
    mGoldUnits=1;
    mFoodUnits=1;

    //*********  TEMP END  ***
}
void Player::GetRessources()
{
    mRes->AddRessources(mGoldUnits,mBlockterieUnits,mFoodUnits);
}