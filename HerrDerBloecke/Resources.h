#pragma once

namespace HdB {

    typedef struct cost{
        int Gold;
        int Blockterie;
        int Food;
    }Cost;

    enum ProductionType{eGold,eBlockterie,eFood};

    ref class Resources
    {
    public:
        /** setting up the start resources and capacity
        */
        Resources(int startGold, int startBlockterie, int startFood);
        
        /** Adds the Resources depending on the amount of buildings
        */
        void AddResources(short _GoldUnits, short _BlockterieUnits, short _FoodUnits);

        /** Pays the passed amount of the resources 
        */
        void Pay(Cost Costs);

        /** Returns true if the player has the passed amount of the resources
        */
        bool CheckAmount(Cost Costs);

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
