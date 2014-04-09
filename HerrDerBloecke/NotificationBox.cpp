#include "NotificationBox.h"
using namespace HdB;

NotificationBox::NotificationBox(Control^ target,int x,int y)
{
	Parent=target;
    _Location=Point(x,y);

    //init label
    mTitle=gcnew Label();
    mTitle->Location=_Location;
    mTitle->Text="Benachrichtigungen";
    mTitle->AutoSize=true;
    mTitle->Anchor=AnchorStyles::Bottom;
    target->Controls->Add(mTitle);

    //setting up the Box
    this->Enabled=true;    //disabling the box to avoid getting focused
	this->UseWaitCursor=false;
	this->Cursor=Cursors::Arrow;
    this->ForeColor=Color::Black;
    this->BorderStyle=System::Windows::Forms::BorderStyle::FixedSingle;
    this->ReadOnly=true;
    this->BackColor=this->Parent->BackColor;
    this->Location=Point(_Location.X,_Location.Y+mTitle->Height);
    this->Multiline=true;
    this->ScrollBars=System::Windows::Forms::ScrollBars::Vertical;
    this->Size=System::Drawing::Size(target->Size.Width*0.4,60);
    this->Anchor=AnchorStyles::Bottom;
    target->Controls->Add(this);
}

void NotificationBox::Resize(Control^ hwnd)
{
    this->Location=Point(hwnd->Width*0.4,this->Location.Y);
    this->Size=System::Drawing::Size(hwnd->Width*0.4,this->Size.Height);
    mTitle->Location=Point(hwnd->Width*0.4,mTitle->Location.Y);
}

void NotificationBox::SendMessage(String^ msg)
{
    DateTime^ now = DateTime::Now;
    this->AppendText("[" + now->ToString("H:mm:ss") + "] " + msg + Environment::NewLine);
}



