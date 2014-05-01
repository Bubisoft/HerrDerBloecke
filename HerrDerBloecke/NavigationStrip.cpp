#include "NavigationStrip.h"



using namespace HdB;
using namespace System;
using namespace System::Diagnostics;
HdB::NavigationStrip::NavigationStrip(Control^ target,int x, int y)
{
    mNumPB=NUM_PB;
	mParent=target;
    Location=Point(x,y); //upper left corner of the Strip

    //Label
    mTitle=gcnew Label;
    mTitle->AutoSize=true;
    mTitle->Text="Navigation";
    mTitle->Anchor=( AnchorStyles::Bottom | AnchorStyles::Left );
    target->Controls->Add(mTitle);
    mTitle->Location=Point(x + SPACE + BTN_WIDHT,y);

    mPBNavi = gcnew List<NavigationThumb^>();

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
    BuildingMenuView();
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
    //variables for properly centering depending on the number of boxes ( odd or straight )
    int i=1;
    int z=1;
    double factor=0.5;

    if(mNumPB%2==0) 
    {
        i=0;
        z=0;
        factor=1;
    }
    if(((mParent->Size.Width*0.4) - SPACE*mNumPB - Location.X)/mNumPB <PB_WIDTH)
    {
        int newSize=((mParent->Size.Width*0.4) - SPACE * mNumPB - Location.X)/mNumPB;
        for each(NavigationThumb^ PB in mPBNavi)
        {
            PB->Size=Size(newSize,newSize);
            PB->Location=Point((mParent->Size.Width*0.4-Location.X)/2 + (PB->Size.Width*factor*z*Math::Pow(-1,i)) + (SPACE/2*z*Math::Pow(-1,i))+Location.X,PB->Location.Y );
    
            ++i;
            if(i%2==1)
                ++z;
        }
        mTitle->Location=Point((mParent->Size.Width*0.4/2-Location.X)-mTitle->Size.Width/2 + Location.X,mTitle->Location.Y);
        //mBtnLeft->Size=Size(((mParent->Size.Width*0.4) - mNumPB*mPBNavi[0]->Size.Width - SPACE*6 - Location.X)/2,mPBNavi[0]->Size.Height);
        //mBtnRight->Size=Size(((mParent->Size.Width*0.4) - mNumPB*mPBNavi[0]->Size.Width - SPACE*6 - Location.X)/2,mPBNavi[0]->Size.Height);
        //mBtnRight->Location=Point(mPBNavi[mNumPB-1]->Location.X + mPBNavi[mNumPB-1]->Size.Width + SPACE,mBtnRight->Location.Y);
    }
    else
    {
        for each(NavigationThumb^ PB in mPBNavi)
        {
        
            PB->Size=Size(PB_WIDTH,PB_HEIGHT);
            PB->Location=Point((mParent->Size.Width*0.4-Location.X)/2 + (PB->Size.Width*factor*z*Math::Pow(-1,i)) + (SPACE/2*z*Math::Pow(-1,i))+Location.X,PB->Location.Y );
    
            ++i;
            if(i%2==1)
                ++z;
        }
        mTitle->Location=Point((mParent->Size.Width*0.4/2-Location.X)-mTitle->Size.Width/2 + Location.X,mTitle->Location.Y);
        //mBtnLeft->Size=Size(BTN_WIDHT,BTN_HEIGHT);
        //mBtnRight->Size=Size(BTN_WIDHT,BTN_HEIGHT);
        //mBtnRight->Location=Point(mPBNavi[mNumPB-1]->Location.X + mPBNavi[mNumPB-1]->Size.Width + SPACE,mBtnRight->Location.Y); 
    }
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
    {
        mFocusedPb->Image=nullptr;
        mFocusedPb=nullptr;
        mFocusedUnit=nullptr;
    }
}

void HdB::NavigationStrip::BlockhausViewClick(Object^  sender, EventArgs^  e)
{
    NavigationThumb^ pb=(NavigationThumb^)sender;

    if(pb->Image!=nullptr) //enabled gold production
    {
        pb->Image=nullptr;
        dynamic_cast<Blockhuette^>(mFocusedUnit)->enabled=true;
        ProductionSwitched(1);
    }
    else //Disabled gold Production -> showing overlayed cross
    {
        pb->Image=Image::FromFile(THUMB_PATH+"cross.png");
        dynamic_cast<Blockhuette^>(mFocusedUnit)->enabled=false;
        ProductionSwitched(-1);
    }
}

void HdB::NavigationStrip::ClearThumbnails()
{
    //remmoving the controlls from the parent window 
    for each(NavigationThumb^ PB in mPBNavi)
        mParent->Controls->Remove(PB);
    //clearing the list
    mPBNavi->Clear();
}

void HdB::NavigationStrip::BlockhausView(Unit^ u)
{
    mFocusedUnit=u;
    mNumPB=2;
    Unfocus();
    ClearThumbnails();
    for(int i=0;i<mNumPB;++i)
        mPBNavi->Add(gcnew NavigationThumb());

    String^ foreground=nullptr;
    if(!dynamic_cast<Blockhuette^>(u)->enabled)
        foreground=gcnew String("cross.png");
    this->AddPictureBox(THUMB_PATH+"goldcoin.png",foreground,gcnew EventHandler(this,&HdB::NavigationStrip::BlockhausViewClick), Blockwerk::typeid);
    this->AddPictureBox(THUMB_PATH+"tearoff.png",nullptr,gcnew EventHandler(this,&HdB::NavigationStrip::TearOffCall),nullptr);

    Resize();
}

void HdB::NavigationStrip::BuildingMenuView()
{
    ClearThumbnails();
    mNumPB=4;
    List<String^>^ buildings=gcnew List<String^>();
    List<Type^>^ buildTypes = gcnew List<Type^>(); 
    for(int i=0;i<mNumPB;++i)
    {
        buildings->Add(initBuildings[i]);
        buildTypes->Add(initTypes[i]);
    }

    for(int i=0;i<mNumPB;++i)
        mPBNavi->Add(gcnew NavigationThumb());

    int i=0;
    for each(NavigationThumb^ PB in mPBNavi)
    {
        String^ path=THUMB_PATH + buildings[i] + ".png";
        PB->Location=Point(Location.X + BTN_WIDHT + SPACE + (PB_WIDTH + SPACE) * i,mTitle->Location.Y + mTitle->Height);
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
        mParent->Controls->Add(PB);
        ++i;
    }
	Resize();

}

void HdB::NavigationStrip::BlockfarmView(Unit^ u)
{
    ClearThumbnails();
    Unfocus();
    mNumPB=1;
    mFocusedUnit=u;
    this->AddPictureBox(THUMB_PATH+"tearoff.png",nullptr,gcnew EventHandler(this,&HdB::NavigationStrip::TearOffCall), Blockfarm::typeid);
    Resize();
}

void HdB::NavigationStrip::BlockstattView(Unit^ u)
{
    ClearThumbnails();
    Unfocus();
    mNumPB=2;
    mFocusedUnit=u;
    this->AddPictureBox(THUMB_PATH+"tearoff.png",nullptr,gcnew EventHandler(this,&HdB::NavigationStrip::TearOffCall), Blockstatt::typeid);
    Resize();
}

void HdB::NavigationStrip::BlockwerkView(Unit^ u)
{
    ClearThumbnails();
    Unfocus();
    mNumPB=1;
    mFocusedUnit=u;
    this->AddPictureBox(THUMB_PATH+"tearoff.png",nullptr,gcnew EventHandler(this,&HdB::NavigationStrip::TearOffCall), Blockwerk::typeid);
    Resize();
}

void HdB::NavigationStrip::TearOffCall(Object^  sender, EventArgs^  e)
{
    TearOffEvent(mFocusedUnit);
    Unfocus();
    BuildingMenuView();
}


void HdB::NavigationStrip::BuildUnitCall(Object^  sender, EventArgs^  e)
{
    UnitBuildEvent();
}

void HdB::NavigationStrip::AddPictureBox(String^ Background,String^ ForeGround,EventHandler^ CalledFunction,Type^ unittype)
{
    mPBNavi->Add(gcnew NavigationThumb());
    mPBNavi[mPBNavi->Count-1]->BackgroundImage=Image::FromFile(Background);
    if(ForeGround!=nullptr)
        mPBNavi[mPBNavi->Count-1]->Image=Image::FromFile(ForeGround);
    mPBNavi[mPBNavi->Count-1]->Click+=CalledFunction;
    mPBNavi[mPBNavi->Count-1]->Location=Point(Location.X,mTitle->Location.Y+mTitle->Size.Height);
    mPBNavi[mPBNavi->Count-1]->Size=Size(PB_WIDTH,PB_HEIGHT);
    mPBNavi[mPBNavi->Count-1]->SizeMode=PictureBoxSizeMode::StretchImage;
    mPBNavi[mPBNavi->Count-1]->BackgroundImageLayout=ImageLayout::Stretch;
    mPBNavi[mPBNavi->Count-1]->BorderStyle=BorderStyle::FixedSingle;
    mPBNavi[mPBNavi->Count-1]->Anchor=(AnchorStyles::Bottom | AnchorStyles::Left);
    mPBNavi[mPBNavi->Count-1]->UnitType=unittype;
    mParent->Controls->Add(mPBNavi[mPBNavi->Count-1]);
}
