#pragma once
#include "Unit.h"
#include "Globals.h"
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
namespace HdB {
    using namespace System;
    using namespace System::IO;
    using namespace System::Windows;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    #define PB_WIDTH 60     //max picturebox width
    #define PB_HEIGHT 60    //max Picturebox height
    #define SPACE 6         //space between the Pictureboxes and buttons
    #define NUM_PB 4        //number of picturboxes

    delegate void GoldProductionEvent(UInt16 value);
    delegate void TearOff(Unit^ u);
    delegate void BuildUnit(Vector3 pos);

    ref class NavigationThumb : public PictureBox {
    public:
        NavigationThumb() : PictureBox() { }
        property Type^ UnitType;
    };

    ref class NavigationStrip
    {
    public:
        NavigationStrip(Control^ target,ToolStripStatusLabel^ ToolTip,int x, int y);
        ~NavigationStrip();

        /** Sets the focus a the clicked PictureBox (blue frame)(CLickEvent) */
        void ChangeFocus(Object^ sender, EventArgs^ e);

        /** Shows the tooltip on MousOver */
        void MouseOver(Object^ sender, EventArgs^ e);

        /** Hides the tooltip on leave*/
        void MouseLeave(Object^ sender, EventArgs^ e);

        /** Unfocus the PictureBoxes, returns true if something was focused */
        bool Unfocus();

        /** Returns the string to selected unit */
        String^ GetModelString();

        /** Returns the type of the selected unit */
        Type^ GetModelType();

        /** removes the pictureboxes from the parent window and cleares the lists */
        void ClearThumbnails();

        /** Menu for Hauptgebaeude */
        void HauptgebaeudeView(Unit^ u);

        /** Switches to the Blockhaus View (switching gold production) */
        void BlockhausView(Unit^ u);
        void BlockhausViewClick(Object^  sender, EventArgs^  e);
        event GoldProductionEvent^ ProductionSwitched;

        /** Menu for Blockfarm */
        void BlockfarmView(Unit^ u);

        /** Menu for Blockstatt */
        void BlockstattView(Unit^ u);

        /** Menu for Blockwerk */
        void BlockwerkView(Unit^ u);

        /** Menu for Units */
        void UnitView(Unit^ u);

        /** Standart View */
        void BuildingMenuView();

        /** Resizes the whole navigationstrip so it fills 40% of the window */
        void Resize();

        /**  Adds a new Picturebox with the default settings to the Strip */
        void AddPictureBox(String^ Background,String^ ForeGround,array<EventHandler^>^ CalledFunction,Type^ unittype);
        void AddPictureBox(String^ Background,String^ ForeGround,EventHandler^ CalledFunction,Type^ unittype);

        void TearOffCall(Object^  sender, EventArgs^  e);
        void BuildUnitCall(Object^ sender, EventArgs^ e);

        event TearOff^ TearOffEvent;
        event BuildUnit^ UnitBuildEvent;

    private:
        Point Location;
        Control^ mParent;
        ToolStripStatusLabel^ mToolTip;
        List<NavigationThumb^>^ mPBNavi;
        Label^ mTitle;           //titlelabel over the strip
        NavigationThumb^ mFocusedPb;    //currently focused picturbox
        Unit^ mFocusedUnit;
                                                           //Reihenfolge  3            2          4               1
    static array<String^>^ initBuildings = gcnew array<String^> {"Blockhaus", "Blockstatt", "Blockwerk", "Kastenfarm"};
    static array<Type^>^ initTypes = gcnew array<Type^> {Blockhuette::typeid, Blockstatt::typeid, Blockwerk::typeid, Blockfarm::typeid};
    };
}
