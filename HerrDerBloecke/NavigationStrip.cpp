#include "NavigationStrip.h"
#include "Globals.h"
#include "Unit.h"

using namespace HdB;
using namespace Diagnostics;

NavigationStrip::NavigationStrip(Control^ target,int x, int y)
{

	array<String^>^ buildings=gcnew array<String^>{"Blockhaus","Blockstatt","Blockwerk", "Kastenfarm"};
    array<Type^>^ buildTypes = gcnew array<Type^>{TestUnit::typeid, TestUnit::typeid, TestUnit::typeid, TestUnit::typeid};
	mParent=target;
     Location=Point(x,y);

    //Label
    mTitle=gcnew Label;
    mTitle->Text="Navigation";
    mTitle->AutoSize=true; 
    mTitle->Anchor=( AnchorStyles::Bottom | AnchorStyles::Left );
    target->Controls->Add(mTitle);
    mTitle->Location=Point(x + SPACE + BTN_WIDHT,y);

    mPBNavi = gcnew array<NavigationThumb^>(NUM_PB);

    for(int i=0;i<NUM_PB;++i)
    {
        mPBNavi[i] = gcnew NavigationThumb();
    }
    //left navigation button
    mBtnLeft=gcnew Button;
    mBtnLeft->Location=Point(Location.X,Location.Y + mTitle->Height);
    mBtnLeft->Size=Size(BTN_WIDHT, BTN_HEIGHT);
    mBtnLeft->Text="<";
    mBtnLeft->Anchor=( AnchorStyles::Bottom | AnchorStyles::Left );
    mBtnLeft->Click+=gcnew System::EventHandler(this, &NavigationStrip::Scroll);
    target->Controls->Add(mBtnLeft);

    //right navigation button
    mBtnRight=gcnew Button;
    mBtnRight->Location=Point(Location.X + BTN_WIDHT + SPACE + (PB_WIDTH + SPACE) * NUM_PB, Location.Y + mTitle->Height);
    mBtnRight->Size=Size(BTN_WIDHT, BTN_HEIGHT);
    mBtnRight->Text=">";
    mBtnRight->Anchor=( AnchorStyles::Bottom | AnchorStyles::Left );
    mBtnRight->Click+=gcnew System::EventHandler(this, &NavigationStrip::Scroll);
    target->Controls->Add(mBtnRight);

    //initialising the PictureBoxes
    int i=0;
    for each(NavigationThumb^ PB in mPBNavi)
    {
        String^ path=THUMB_PATH + buildings[i] + ".png";
        PB->Location=Point(Location.X + BTN_WIDHT + SPACE + (PB_WIDTH + SPACE) * i,Location.Y + mTitle->Height);
        PB->SizeMode=PictureBoxSizeMode::StretchImage;
        PB->BackgroundImageLayout=ImageLayout::Stretch;
        PB->BorderStyle=BorderStyle::FixedSingle;
        PB->Click+=gcnew EventHandler(this,&NavigationStrip::ChangeFocus);
        if(File::Exists(path))
        {
            PB->BackgroundImage=Image::FromFile(path);
            PB->Text = buildings[i];
            PB->UnitType = buildTypes[i];
        }
        else
            Debug::WriteLine("ERROR: Could not load file!");
        PB->Anchor=(AnchorStyles::Bottom | AnchorStyles::Left);
        target->Controls->Add(PB);
        ++i;
    }
	Resize();
}

void NavigationStrip::Scroll(Object^  sender, EventArgs^  e)
{
    Image^ temp1=mPBNavi[0]->BackgroundImage;
    Image^ temp2;
    if(sender == mBtnRight)
    {
        temp2=mPBNavi[0]->BackgroundImage;
        for(int i=NUM_PB-1;i>=0;i--)
        {
            temp1=mPBNavi[i]->BackgroundImage;
            mPBNavi[i]->BackgroundImage=temp2;
            temp2=temp1;
        }
    }
    else
    {
        for each(NavigationThumb^ PB in mPBNavi)
        {
            temp1=PB->BackgroundImage;
            PB->BackgroundImage=temp2;
            temp2=temp1;
        }
        mPBNavi[0]->BackgroundImage=temp2;
    }
}
void NavigationStrip::ChangeFocus(Object^ sender, EventArgs^ e)
{
    if(mFocusedPb!=nullptr)
        mFocusedPb->Image=nullptr; //remove the focusing frame from old focus
    NavigationThumb^ pb=(NavigationThumb^)sender;
    if(mFocusedPb!=pb)
    {
        if(File::Exists(THUMB_PATH + "focused.png"))
            pb->Image=Image::FromFile(THUMB_PATH + "focused.png");  //set the focus frame
        else
            Debug::WriteLine("ERROR: Could not load file!");
        mFocusedPb=pb;
    }
    else    //unfocusing the already focused
		mFocusedPb=nullptr;
	

	//Debug::WriteLine(mFocusedPb->Text);
}

void NavigationStrip::Resize()
{
    int i=0;
    if(((mParent->Size.Width*0.4)-BTN_WIDHT*2 - SPACE*6 - Location.X)/NUM_PB <PB_WIDTH)
    {
        int newSize=((mParent->Size.Width*0.4) - BTN_WIDHT*2 - SPACE * 6 - Location.X)/NUM_PB;
        for each(NavigationThumb^ PB in mPBNavi)
        {
            PB->Size=Size(newSize,newSize);
            PB->Location=Point(BTN_WIDHT + SPACE + ( PB->Size.Width +SPACE ) * i + Location.X,PB->Location.Y );
    
            ++i;
        }
        mBtnLeft->Size=Size(((mParent->Size.Width*0.4) - NUM_PB*mPBNavi[0]->Size.Width - SPACE*6 - Location.X)/2,mPBNavi[0]->Size.Height);
        mBtnRight->Size=Size(((mParent->Size.Width*0.4) - NUM_PB*mPBNavi[0]->Size.Width - SPACE*6 - Location.X)/2,mPBNavi[0]->Size.Height);
        mBtnRight->Location=Point(mPBNavi[NUM_PB-1]->Location.X + mPBNavi[NUM_PB-1]->Size.Width + SPACE,mBtnRight->Location.Y);
    }
    else
    {
        for each(NavigationThumb^ PB in mPBNavi)
        {
        
            PB->Size=Size(PB_WIDTH,PB_HEIGHT);
            PB->Location=Point(BTN_WIDHT + SPACE + ( PB->Size.Width +SPACE ) * i + Location.X,PB->Location.Y );
            ++i;
        }
        mBtnLeft->Size=Size(BTN_WIDHT,BTN_HEIGHT);
        mBtnRight->Size=Size(BTN_WIDHT,BTN_HEIGHT);
        mBtnRight->Location=Point(mPBNavi[NUM_PB-1]->Location.X + mPBNavi[NUM_PB-1]->Size.Width + SPACE,mBtnRight->Location.Y);
    }
    
}

void NavigationStrip::Update()
{

}

String^ NavigationStrip::GetModelString()
{
    if(mFocusedPb!=nullptr)
        return mFocusedPb->Text;
    return nullptr;
}

Type^ NavigationStrip::GetModelType()
{
    if (mFocusedPb != nullptr)
        return mFocusedPb->UnitType;
    return nullptr;
}

void NavigationStrip::Unfocus()
{
    if(mFocusedPb!=nullptr)
        mFocusedPb->Image=nullptr;
}