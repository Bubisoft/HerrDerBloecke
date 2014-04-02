#pragma once

namespace HdB {
    ref class Resources
    {
    public:
        Resources(void);
  
        /** Adds the Resources depending on the amount of buildings
        */
        void AddResources(short _GoldUnits, short _BlockterieUnits, short _FoodUnits);

        /** Pays the passed amount of the resources 
        */
        void Pay(int _gold,int _blockterie, int _food);

        property int Gold
        {
            int get()
            {
                return mGold;
            }
        }
        
        property int Blockterie
        {
            int get()
            {
                return mBlockterie;
            }
        }

        property int Food
        {
            int get()
            {
                return mFood;
            }
        }

    private:
        int mGold;
        int mFood;
        int mBlockterie;
        int mGoldCapacity;
        int mFoodCapacity;
        int mBlockterieCapacity;
    };
}
