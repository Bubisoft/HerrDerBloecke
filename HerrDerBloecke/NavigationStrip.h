#pragma once
namespace HdB {
    using namespace System;
    using namespace System::IO;
    using namespace System::Windows;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    #define PB_WIDTH 60     //max picturebox width
    #define PB_HEIGHT 60    //max Picturebox height
    #define BTN_WIDHT 30    //max button width
    #define BTN_HEIGHT 60   //max Button heigth
    #define SPACE 5 //space between the Pictureboxes and buttons
    #define NUM_PB 5    //number of picturboxes
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

        void Resize(Control^ hwnd);
    private:
        Point Location;
        array<PictureBox^>^ mPBNavi;    
        Label^ mTitle;          
        PictureBox^ focused;    //currently focused picturbox
        enum ViewType type;
        Button^ mBtnLeft;       //button at the left of the navigation
        Button^ mBtnRight;      //button at the right of the navigation
    };
    enum ViewType{gebäude,einheiten,forschung};
}


