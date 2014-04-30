#include "NavigationStrip.h"
#include "Globals.h"
#include "Unit.h"

using namespace HdB;
using namespace System;
using namespace Diagnostics;
HdB::NavigationStrip::NavigationStrip(Control^ target,int x, int y)
{
    mNumPB=NUM_PB;
    const array<String^>^ initBuildings= gcnew array<String^>{"Blockhaus","Blockstatt","Blockwerk", "Kastenfarm"};
    const array<Type^>^ initTypes= gcnew array<Type^>{Blockhuette::typeid, Blockstatt::typeid, Blockwerk::typeid, Kastenfarm::typeid};
	List<String^>^ buildings=gcnew List<String^>();
    List<Type^>^ buildTypes = gcnew List<Type^>(); 
    //init the string and type list with the arrays
    for(int i=0;i<mNumPB;++i)
    {
        buildings->Add(initBuildings[i]);
        buildTypes->Add(initTypes[i]);
    }
	mParent=target;
    Location=Point(x,y);

    //Label
    mTitle=gcnew Label;
    mTitle->Text="Navigation";
    mTitle->AutoSize=true; 
    mTitle->Anchor=( AnchorStyles::Bottom | AnchorStyles::Left );
    target->Controls->Add(mTitle);
    mTitle->Location=Point(x + SPACE + BTN_WIDHT,y);

    mPBNavi = gcnew List<NavigationThumb^>();

    for(int i=0;i<mNumPB;++i)
    {
        mPBNavi->Add(gcnew NavigationThumb());
    }
    /*
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
    mBtnRight->Location=Point(Location.X + BTN_WIDHT + SPACE + (PB_WIDTH + SPACE) * mNumPB, Location.Y + mTitle->Height);
    mBtnRight->Size=Size(BTN_WIDHT, BTN_HEIGHT);
    mBtnRight->Text=">";
    mBtnRight->Anchor=( AnchorStyles::Bottom | AnchorStyles::Left );
    mBtnRight->Click+=gcnew System::EventHandler(this, &NavigationStrip::Scroll);
    target->Controls->Add(mBtnRight);
    */
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
            Debug::WriteLine("ERROR:NavigationStrip Could not load file!");
        PB->Anchor=(AnchorStyles::Bottom | AnchorStyles::Left);
        target->Controls->Add(PB);
        ++i;
    }
	Resize();
}

void HdB::NavigationStrip::Scroll(Object^  sender, EventArgs^  e)
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
void HdB::NavigationStrip::ChangeFocus(Object^ sender, EventArgs^ e)
{
    if(mFocusedPb!=nullptr)
        mFocusedPb->Image=nullptr; //remove the focusing frame from old focus
    NavigationThumb^ pb=(NavigationThumb^)sender;
    if(mFocusedPb!=pb)
    {
        if(File::Exists(THUMB_PATH + "focused.png"))
            pb->Image=Image::FromFile(THUMB_PATH + "focused.png");  //set the focus frame
        else
            Debug::WriteLine("ERROR:NavigationStrip Could not load file!");
        mFocusedPb=pb;
    }
    else    //unfocusing the already focused
		mFocusedPb=nullptr;
}

void HdB::NavigationStrip::Resize()
{
    /*
    int i=0;
    if(((mParent->Size.Width*0.4)-BTN_WIDHT*2 - SPACE*6 - Location.X)/mNumPB <PB_WIDTH)
    {
        int newSize=((mParent->Size.Width*0.4) - BTN_WIDHT*2 - SPACE * 6 - Location.X)/mNumPB;
        for each(NavigationThumb^ PB in mPBNavi)
        {
            PB->Size=Size(newSize,newSize);
            PB->Location=Point(BTN_WIDHT + SPACE + ( PB->Size.Width +SPACE ) * i + Location.X,PB->Location.Y );
    
            ++i;
        }
        //mBtnLeft->Size=Size(((mParent->Size.Width*0.4) - mNumPB*mPBNavi[0]->Size.Width - SPACE*6 - Location.X)/2,mPBNavi[0]->Size.Height);
        //mBtnRight->Size=Size(((mParent->Size.Width*0.4) - mNumPB*mPBNavi[0]->Size.Width - SPACE*6 - Location.X)/2,mPBNavi[0]->Size.Height);
        //mBtnRight->Location=Point(mPBNavi[mNumPB-1]->Location.X + mPBNavi[mNumPB-1]->Size.Width + SPACE,mBtnRight->Location.Y);
    }
    else
    {
        for each(NavigationThumb^ PB in mPBNavi)
        {
        
            PB->Size=Size(PB_WIDTH,PB_HEIGHT);
            PB->Location=Point(BTN_WIDHT + SPACE + ( PB->Size.Width +SPACE ) * i + Location.X,PB->Location.Y );
            ++i;
        }
        //mBtnLeft->Size=Size(BTN_WIDHT,BTN_HEIGHT);
        //mBtnRight->Size=Size(BTN_WIDHT,BTN_HEIGHT);
        //mBtnRight->Location=Point(mPBNavi[mNumPB-1]->Location.X + mPBNavi[mNumPB-1]->Size.Width + SPACE,mBtnRight->Location.Y); 
    } */
    int i=1;
    int z=1;
    double factor=0.5;

    if(mNumPB%2==0)
    {
        i=0;
        z=0;
        factor=1;
    }
    if(((mParent->Size.Width*0.4)-BTN_WIDHT*2 - SPACE*6 - Location.X)/mNumPB <PB_WIDTH)
    {
        int newSize=((mParent->Size.Width*0.4) - BTN_WIDHT*2 - SPACE * 6 - Location.X)/mNumPB;
        for each(NavigationThumb^ PB in mPBNavi)
        {
            PB->Size=Size(newSize,newSize);
            PB->Location=Point((mParent->Size.Width*0.4)/2 + (PB->Size.Width*factor*z*Math::Pow(-1,i)) + (SPACE/2*z*Math::Pow(-1,i))-Location.X,PB->Location.Y );
    
            ++i;
            if(i%2==1)
                ++z;
        }
        mTitle->Location=Point((mParent->Size.Width*0.4/2)-mTitle->Size.Width/2 - Location.X,mTitle->Location.Y);
        //mBtnLeft->Size=Size(((mParent->Size.Width*0.4) - mNumPB*mPBNavi[0]->Size.Width - SPACE*6 - Location.X)/2,mPBNavi[0]->Size.Height);
        //mBtnRight->Size=Size(((mParent->Size.Width*0.4) - mNumPB*mPBNavi[0]->Size.Width - SPACE*6 - Location.X)/2,mPBNavi[0]->Size.Height);
        //mBtnRight->Location=Point(mPBNavi[mNumPB-1]->Location.X + mPBNavi[mNumPB-1]->Size.Width + SPACE,mBtnRight->Location.Y);
    }
    else
    {
        for each(NavigationThumb^ PB in mPBNavi)
        {
        
            PB->Size=Size(PB_WIDTH,PB_HEIGHT);
            PB->Location=Point((mParent->Size.Width*0.4)/2 + (PB->Size.Width*factor*z*Math::Pow(-1,i)) + (SPACE/2*z*Math::Pow(-1,i))-Location.X,PB->Location.Y );
    
            ++i;
            if(i%2==1)
                ++z;
        }
        mTitle->Location=Point((mParent->Size.Width*0.4/2)-mTitle->Size.Width/2 - Location.X,mTitle->Location.Y);
        //mBtnLeft->Size=Size(BTN_WIDHT,BTN_HEIGHT);
        //mBtnRight->Size=Size(BTN_WIDHT,BTN_HEIGHT);
        //mBtnRight->Location=Point(mPBNavi[mNumPB-1]->Location.X + mPBNavi[mNumPB-1]->Size.Width + SPACE,mBtnRight->Location.Y); 
    }
}

void HdB::NavigationStrip::Update()
{

}

String^ HdB::NavigationStrip::GetModelString()
{
    if(mFocusedPb!=nullptr)
        return mFocusedPb->Text;
    return nullptr;
}

Type^ HdB::NavigationStrip::GetModelType()
{
    if (mFocusedPb != nullptr)
        return mFocusedPb->UnitType;
    return nullptr;
}

void HdB::NavigationStrip::Unfocus()
{
    if(mFocusedPb!=nullptr)
        mFocusedPb->Image=nullptr;
}

void HdB::NavigationStrip::BlockhausViewClick(Object^  sender, EventArgs^  e)
{
    NavigationThumb^ pb=(NavigationThumb^)sender;
    if(pb->Image!=nullptr)
        pb->Image=nullptr;
    else
        pb->Image=Image::FromFile(THUMB_PATH+"cross.png");

    // Adding "gold switch" 
}

void HdB::NavigationStrip::BlockhausView()
{
    mNumPB=1;
    for each(NavigationThumb^ PB in mPBNavi)
        mParent->Controls->Remove(PB);
    mPBNavi->Clear();

    mPBNavi->Add(gcnew NavigationThumb());
    mPBNavi[0]->BackgroundImage=Image::FromFile(THUMB_PATH+"test3.jpg");
    mPBNavi[0]->Location=Point(Location.X,mTitle->Location.Y+mTitle->Size.Height);
    mPBNavi[0]->Size=Size(PB_WIDTH,PB_HEIGHT);
    mPBNavi[0]->SizeMode=PictureBoxSizeMode::StretchImage;
    mPBNavi[0]->BackgroundImageLayout=ImageLayout::Stretch;
    mPBNavi[0]->BorderStyle=BorderStyle::FixedSingle;
    mPBNavi[0]->Click+=gcnew EventHandler(this,&NavigationStrip::BlockhausViewClick);
    mPBNavi[0]->Anchor=(AnchorStyles::Bottom | AnchorStyles::Left);
    mParent->Controls->Add(mPBNavi[0]);
    Resize();
}

void HdB::NavigationStrip::BuildingMenuView()
{

}