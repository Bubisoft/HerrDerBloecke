#include "Resources.h"

#define STD_GOLD 2
#define STD_BLOCKTERIE 3
#define STD_FOOD 2

#define STD_MAX_GOLD 10000   // Maybe expandable with store units
#define STD_MAX_BLOCKTERIE 10000
#define STD_MAX_FOOD 10000

HdB::Resources::Resources(void)
    : mGold(0), mFood(0), mBlockterie(0),
    mBlockterieCapacity(STD_MAX_BLOCKTERIE), mFoodCapacity(STD_MAX_FOOD), mGoldCapacity(STD_MAX_GOLD)
{

}

void HdB::Resources::AddResources(short _GoldUnits, short _BlockterieUnits, short _FoodUnits)
{
    if(mGold<mGoldCapacity)
        mGold+=STD_GOLD*_GoldUnits;
    if(mGold>mGoldCapacity)
        mGold=mGoldCapacity;

    if(mBlockterie<mBlockterieCapacity)
        mBlockterie+=STD_BLOCKTERIE*_BlockterieUnits;
    if(mBlockterie>mBlockterieCapacity)
        mBlockterie=mBlockterieCapacity;

    if(mFood<mFoodCapacity)
        mFood+=STD_FOOD*_FoodUnits;
    if(mFood>mFoodCapacity)
        mFood=mFoodCapacity;
}

void HdB::Resources::Pay(int _gold, int _blockterie, int _food)
{
    mGold-=_gold;
    mBlockterie-=_blockterie;
    mFood-=_food;
}
