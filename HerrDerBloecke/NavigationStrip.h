#pragma once
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
    #define BTN_WIDHT 0    //max button width 30
    #define BTN_HEIGHT 0   //max Button heigth 60
    #define SPACE 6 //space between the Pictureboxes and buttons
    #define NUM_PB 4    //number of picturboxes

    ref class NavigationThumb : public PictureBox {
    public:
        NavigationThumb() : PictureBox() { }
        property Type^ UnitType;
    };

    ref class NavigationStrip
    {
    public:
        NavigationStrip(Control^ target,int x, int y);

        /** Scrolls in the Navigation (PictureBoxes) when clicking one of the two buttons
        */
        void Scroll(Object^  sender, EventArgs^  e);

        /** Sets the focus a the clicked PictureBox (blue frame)
        */
        void ChangeFocus(Object^ sender, EventArgs^ e);

		/** Unfocus the PictureBoxes
		*/
		void Unfocus();

		/** Returns the string to selected unit
		*/
		String^ GetModelString();

        /** Returns the type of the selected unit */
        Type^ GetModelType();

		/**Updates the images of the PictureBoxes
		*/
		void Update();

        /** Switches to the Blockhaus View (switching gold production)
        */
        void BlockhausView();
        void BlockhausViewClick(Object^  sender, EventArgs^  e);

        /** Standart View
        */
        void BuildingMenuView();
		/**Resizes the whole navigationstrip so it fills 40% of the window
		*/
        void Resize();

    public:
        property UInt16 NumPB{
            void set(UInt16 _num){ mNumPB=_num; }
        }
    private:
        Point Location;
		Control^ mParent;			
        List<NavigationThumb^>^ mPBNavi;
        Label^ mTitle;           //titlelabel over the strip
        NavigationThumb^ mFocusedPb;    //currently focused picturbox
        Button^ mBtnLeft;       //button at the left of the navigation
        Button^ mBtnRight;      //button at the right of the navigation
        UInt16 mNumPB;
    };
    
}


