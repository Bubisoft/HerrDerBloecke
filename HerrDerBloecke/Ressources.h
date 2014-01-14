#pragma once
namespace HdB {

    #define STD_GOLD 10
    #define STD_BLOCKTERIE 6
    #define STD_FOOD 8

    #define STD_MAX_GOLD 1000   // Maybe expandable with store units
    #define STD_MAX_BLOCKTERIE 1000
    #define STD_MAX_FOOD 1000

    ref class Ressources
    {
    public:
        Ressources(void);
  
        /** Adds the Ressources depending on the amount of buildings
        */
        void AddRessources(short _GoldUnits, short _BlockterieUnits, short _FoodUnits);

        /** Pays the passed amount of the ressources 
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

