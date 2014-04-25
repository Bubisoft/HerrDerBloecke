#include "Resources.h"

#define STD_GOLD 2
#define STD_BLOCKTERIE 3
#define STD_FOOD 2

#define STD_MAX_GOLD 10000   // Maybe expandable with store units
#define STD_MAX_BLOCKTERIE 10000
#define STD_MAX_FOOD 10000


HdB::Resources::Resources(int startGold, int startBlockterie, int startFood)
    : mGold(startGold), mBlockterie(startBlockterie), mFood(startFood), mBlockterieCapacity(STD_MAX_BLOCKTERIE), mFoodCapacity(STD_MAX_FOOD), mGoldCapacity(STD_MAX_GOLD)
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

void HdB::Resources::Pay(Cost Costs)
{
    mGold-=Costs.Gold;
    mBlockterie-=Costs.Blockterie;
    mFood-=Costs.Food;
}

bool HdB::Resources::CheckAmount(Cost Costs)
{
    if(mGold>=Costs.Gold)
        if(mFood>=Costs.Food)
            if(mBlockterie>=Costs.Blockterie)
                return true;    

    return false;
}
