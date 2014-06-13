#pragma once
#include "ISaveable.h"

namespace HdB {
    value struct Costs {
        Costs(int gold, int blockterie, int food) : Gold(gold), Blockterie(blockterie), Food(food) { }
        int Gold;
        int Blockterie;
        int Food;

        virtual System::String^ ToString()  override {
            return "Goldbarren: " + Gold + ", Blockterie: " + Blockterie + ", Kastenbrot: " + Food;
        }
    };

    enum ProductionType{eGold,eBlockterie,eFood};

    ref class Resources : ISaveable
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
        void Pay(Costs costs);

        /** Returns true if the player has the passed amount of the resources
        */
        bool CheckAmount(Costs costs);

        /** Expands the capacity about the passed amount
        */
        void ExpandCapacity(int Gold, int Blockterie, int Food);

        /** */
        float GoldLoad() { return (float)mGold / mGoldCapacity; }
        float BlockterieLoad() { return (float)mBlockterie / mBlockterieCapacity; }
        float FoodLoad() { return (float)mFood / mFoodCapacity; }

        /** */
        virtual void Save(BinaryWriter^ bw);

        /** */
        virtual void Load(BinaryReader^ br,Renderer^ renderer);

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
