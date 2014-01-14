#pragma once
#include "Ressources.h"
namespace HdB {
    ref class Player
    {
    public:
        Player(void);

        /**let the Player get new ressources
        */
        void GetRessources();

    public:
        /** Manages all of the ressources of each player
        */
        Ressources^ mRes;

    private:

        
        short mGoldUnits;
        short mBlockterieUnits;
        short mFoodUnits;
    };
}

