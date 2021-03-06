#include "NavigationStrip.h"
#include "Model.h"



using namespace HdB;
using namespace System;
using namespace System::Diagnostics;
HdB::NavigationStrip::NavigationStrip(Control^ target,ToolStripStatusLabel^ tooltip,int x, int y)
{
	mParent=target;
    mToolTip=tooltip;
    Location=Point(x,y); //upper left corner of the Strip

    //Label
    mTitle=gcnew Label;
    mTitle->AutoSize=true;
    mTitle->Text="Navigation";
    mTitle->Anchor=( AnchorStyles::Bottom | AnchorStyles::Left );
    target->Controls->Add(mTitle);
    mTitle->Location=Point(x + SPACE ,y);

    mPBNavi = gcnew List<NavigationThumb^>();

    //initialising the PictureBoxes
    BuildingMenuView();
}

HdB::NavigationStrip::~NavigationStrip()
{
    for each(NavigationThumb^ thumb in mPBNavi)
        delete thumb;
    delete mTitle;
    delete mToolTip;
    delete mFocusedPb;
}


void HdB::NavigationStrip::ChangeFocus(Object^ sender, EventArgs^ e)
{
    NavigationThumb^ pb=(NavigationThumb^)sender;
    if(mFocusedPb!=nullptr)
        mFocusedPb->Image=nullptr;
    if(mFocusedPb!=pb)
    {
        if(File::Exists(THUMB_PATH + "focused.png"))
            pb->Image=Image::FromFile(THUMB_PATH + "focused.png");  //set the focus frame
        else
            Debug::WriteLine("ERROR:NavigationStrip Could not load file!");
        mFocusedPb=pb;
    }
    else    //unfocusing the already focused
	{
        mFocusedPb=nullptr;
    }
}

void HdB::NavigationStrip::MouseOver(Object^ sender, EventArgs^ e)
{
    NavigationThumb^ t=(NavigationThumb^)sender;
    if(t->UnitType!=nullptr)
    {
        Type^ p=t->UnitType;

        Unit^ unit=safe_cast<Unit^>(Activator::CreateInstance(p,
                            gcnew array<Object^> {nullptr,
                            Vector3::Zero}));
        Costs cost = unit->GetCosts();
        mToolTip->Text = t->Text + " | " + cost.ToString() + " | Bauzeit: " + unit->BuildTime() + "s | " + unit->Description();
    }
    else if(t->Text=="tearoff")
        mToolTip->Text="Abriss";
    else if(t->Text=="goldcoin")
        mToolTip->Text="Goldproduktion umschalten";
}

void HdB::NavigationStrip::MouseLeave(Object^ sender,EventArgs^ e)
{
    mToolTip->Text="";
}

void HdB::NavigationStrip::Resize()
{
    //variables for properly centering depending on the number of boxes ( odd or straight )

    //odd setup
    int i=1; 
    int z=0;

    //straigth setup
    if(mPBNavi->Count%2==0) 
    {
        i=0;
        z=0;
    }
    int newSize=0;
    if(((mParent->Size.Width*0.4) - SPACE*mPBNavi->Count - Location.X)/mPBNavi->Count <PB_WIDTH)
    {    
        newSize=((mParent->Size.Width*0.4) - SPACE * mPBNavi->Count - Location.X)/mPBNavi->Count; 
    }
    for each(NavigationThumb^ PB in mPBNavi)
    {
        if(newSize!=0)
            PB->Size=Size(newSize,newSize);
        if(mPBNavi->Count % 2== 0 ) //straigth
            PB->Location=Point((mParent->Size.Width*0.4-Location.X)/2 + (PB->Size.Width*z*Math::Pow(-1,i)) + (SPACE/2*z*Math::Pow(-1,i)) + Location.X,PB->Location.Y );
        else//odd
        {
            PB->Location=Point((mParent->Size.Width*0.4-Location.X)/2 + (PB->Size.Width/2+(z*PB->Size.Width))*Math::Pow(-1,i)  + (SPACE/2*(i-(z+1))*Math::Pow(-1,i)) + Location.X,PB->Location.Y );
        }
        ++i;
        if(i%2==1)
            ++z;
    }
    mTitle->Location=Point((mParent->Size.Width*0.4/2-Location.X)-mTitle->Size.Width/2 + Location.X,mTitle->Location.Y);

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

bool HdB::NavigationStrip::Unfocus()
{
    if(mFocusedPb!=nullptr)
    {
        mFocusedPb->Image=nullptr;
        mFocusedPb=nullptr;
        mFocusedUnit=nullptr;
        return true;
    }
    return false;
}

void HdB::NavigationStrip::BlockhausViewClick(Object^  sender, EventArgs^  e)
{
    NavigationThumb^ pb=(NavigationThumb^)sender;

    if(pb->Image!=nullptr) //enabled gold production
    {
        pb->Image=nullptr;
        dynamic_cast<Blockhuette^>(mFocusedUnit)->Enabled=true;
        ProductionSwitched(1);
    }
    else //Disabled gold Production -> showing overlayed cross
    {
        pb->Image=Image::FromFile(THUMB_PATH+"cross.png");
        dynamic_cast<Blockhuette^>(mFocusedUnit)->Enabled=false;
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
    ClearThumbnails();
    Unfocus();
    mFocusedUnit=u;
    String^ foreground=nullptr;
    if(!dynamic_cast<Blockhuette^>(u)->Enabled)
        foreground=gcnew String("cross");
    this->AddPictureBox("goldcoin",foreground,gcnew EventHandler(this,&HdB::NavigationStrip::BlockhausViewClick), nullptr);
    this->AddPictureBox("tearoff",nullptr,gcnew EventHandler(this,&HdB::NavigationStrip::TearOffCall),nullptr);

    Resize();
}

void HdB::NavigationStrip::BuildingMenuView()
{
    ClearThumbnails();
    List<String^>^ buildings=gcnew List<String^>();
    List<Type^>^ buildTypes = gcnew List<Type^>(); 
    int i=0;
    for each(String^ s in initBuildings)
    {
        buildings->Add(s);
        buildTypes->Add(initTypes[i]);
        ++i;
    }
    
    for(int x=0;x<buildings->Count;++x)
        this->AddPictureBox(buildings[x],nullptr,gcnew EventHandler(this,&HdB::NavigationStrip::ChangeFocus),buildTypes[x]);

	Resize();

}

void HdB::NavigationStrip::HauptgebaeudeView(Unit^ u)
{
    ClearThumbnails();
    Unfocus();
    mFocusedUnit=u;
    this->AddPictureBox("tearoff",nullptr,gcnew EventHandler(this,&HdB::NavigationStrip::TearOffCall), nullptr);
    Resize();
}

void HdB::NavigationStrip::BlockfarmView(Unit^ u)
{
    ClearThumbnails();
    Unfocus();
    mFocusedUnit=u;
    this->AddPictureBox("tearoff",nullptr,gcnew EventHandler(this,&HdB::NavigationStrip::TearOffCall), nullptr);
    Resize();
}

void HdB::NavigationStrip::BlockstattView(Unit^ u)
{
    ClearThumbnails();
    Unfocus();
    mFocusedUnit=u;
    array<EventHandler^>^ calledFunctions=gcnew array<EventHandler^>{gcnew EventHandler(this,&HdB::NavigationStrip::ChangeFocus),gcnew EventHandler(this,&HdB::NavigationStrip::BuildUnitCall), gcnew EventHandler(this,&HdB::NavigationStrip::ChangeFocus)}; //HACKS!!!!
    this->AddPictureBox("ZuseZ3",nullptr,calledFunctions,ZuseZ3::typeid);   
    this->AddPictureBox("Wirth",nullptr,calledFunctions,Wirth::typeid);
    this->AddPictureBox("Gates",nullptr,calledFunctions,Gates::typeid);
    this->AddPictureBox("Turing",nullptr,calledFunctions,Turing::typeid);
    this->AddPictureBox("tearoff",nullptr,gcnew EventHandler(this,&HdB::NavigationStrip::TearOffCall), nullptr);
    Resize();
}

void HdB::NavigationStrip::BlockwerkView(Unit^ u)
{
    ClearThumbnails();
    Unfocus();
    mFocusedUnit=u;
    this->AddPictureBox("tearoff",nullptr,gcnew EventHandler(this,&HdB::NavigationStrip::TearOffCall), nullptr);
    Resize();
}

void HdB::NavigationStrip::UnitView(Unit^ u)
{
    ClearThumbnails();
    Unfocus();
    mFocusedUnit=u;
    this->AddPictureBox("tearoff", nullptr, gcnew EventHandler(this, &HdB::NavigationStrip::TearOffCall), nullptr);
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
    Vector3 pos = mFocusedUnit->Position + (mFocusedUnit->Model->Bounds.Minimum * 1.5);

    if (mFocusedPb)
        UnitBuildEvent(pos);
}

void HdB::NavigationStrip::AddPictureBox(String^ Background,String^ ForeGround,array<EventHandler^>^ CalledFunction,Type^ unittype)
{
    mPBNavi->Add(gcnew NavigationThumb());
    mPBNavi[mPBNavi->Count-1]->BackgroundImage=Image::FromFile(THUMB_PATH + Background + ".png");
    if(ForeGround!=nullptr)
        mPBNavi[mPBNavi->Count-1]->Image=Image::FromFile(THUMB_PATH + ForeGround + ".png");
    for each(EventHandler^ e in CalledFunction)
        mPBNavi[mPBNavi->Count-1]->Click+=e;
    mPBNavi[mPBNavi->Count-1]->MouseHover+=gcnew EventHandler(this,&NavigationStrip::MouseOver);
    mPBNavi[mPBNavi->Count-1]->MouseLeave+=gcnew EventHandler(this,&NavigationStrip::MouseLeave);
    mPBNavi[mPBNavi->Count-1]->Location=Point(Location.X,mTitle->Location.Y+mTitle->Size.Height);
    mPBNavi[mPBNavi->Count-1]->Size=Size(PB_WIDTH,PB_HEIGHT);
    mPBNavi[mPBNavi->Count-1]->SizeMode=PictureBoxSizeMode::StretchImage;
    mPBNavi[mPBNavi->Count-1]->BackgroundImageLayout=ImageLayout::Stretch;
    mPBNavi[mPBNavi->Count-1]->BorderStyle=BorderStyle::FixedSingle;
    mPBNavi[mPBNavi->Count-1]->Anchor=(AnchorStyles::Bottom | AnchorStyles::Left);
    mPBNavi[mPBNavi->Count-1]->UnitType=unittype;
    mPBNavi[mPBNavi->Count-1]->Text=Background;
    mParent->Controls->Add(mPBNavi[mPBNavi->Count-1]);
}

void HdB::NavigationStrip::AddPictureBox(String^ Background,String^ ForeGround,EventHandler^ CalledFunction,Type^ unittype)
{
    mPBNavi->Add(gcnew NavigationThumb());
    mPBNavi[mPBNavi->Count-1]->BackgroundImage=Image::FromFile(THUMB_PATH + Background + ".png");
    if(ForeGround!=nullptr)
        mPBNavi[mPBNavi->Count-1]->Image=Image::FromFile(THUMB_PATH + ForeGround + ".png");
    mPBNavi[mPBNavi->Count-1]->Click+=CalledFunction;
    mPBNavi[mPBNavi->Count-1]->MouseHover+=gcnew EventHandler(this,&NavigationStrip::MouseOver);
    mPBNavi[mPBNavi->Count-1]->MouseLeave+=gcnew EventHandler(this,&NavigationStrip::MouseLeave);
    mPBNavi[mPBNavi->Count-1]->Location=Point(Location.X,mTitle->Location.Y+mTitle->Size.Height);
    mPBNavi[mPBNavi->Count-1]->Size=Size(PB_WIDTH,PB_HEIGHT);
    mPBNavi[mPBNavi->Count-1]->SizeMode=PictureBoxSizeMode::StretchImage;
    mPBNavi[mPBNavi->Count-1]->BackgroundImageLayout=ImageLayout::Stretch;
    mPBNavi[mPBNavi->Count-1]->BorderStyle=BorderStyle::FixedSingle;
    mPBNavi[mPBNavi->Count-1]->Anchor=(AnchorStyles::Bottom | AnchorStyles::Left);
    mPBNavi[mPBNavi->Count-1]->UnitType=unittype;
    mPBNavi[mPBNavi->Count-1]->Text=Background;
    mParent->Controls->Add(mPBNavi[mPBNavi->Count-1]);
}
