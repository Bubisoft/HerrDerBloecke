#include "NotificationBox.h"
using namespace HdB;

NotificationBox::NotificationBox(Control^ target,int x,int y)
{
    _Location=Point(x,y);

    //init label
    mTitle=gcnew Label();
    mTitle->Location=_Location;
    mTitle->Text="Benachrichtigungen";
    mTitle->AutoSize=true;
    mTitle->Anchor=AnchorStyles::Bottom;
    target->Controls->Add(mTitle);

    //setting up the Box
    this->Enabled=false;    //disabling the box to avoid getting focused
    this->ForeColor=Color::Black;
    this->BorderStyle=System::Windows::Forms::BorderStyle::FixedSingle;
    this->ReadOnly=true;
    this->BackColor=SystemColors::Menu;
    this->Location=Point(_Location.X,_Location.Y+mTitle->Height);
    this->Multiline=true;
    this->ScrollBars=System::Windows::Forms::ScrollBars::Vertical;
    this->Size=System::Drawing::Size(target->Size.Width*0.4,60);
    this->Anchor=AnchorStyles::Bottom;
    this->SetStyle(ControlStyles::UserPaint,true);
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
    DateTime^ now=DateTime::Now;
    this->AppendText("[" + now->Hour.ToString() + ":" + now->Minute.ToString() + "]" + " " + msg + Environment::NewLine);
}


void NotificationBox::OnPaint(PaintEventArgs^ e)    //because the box is disabled we need to draw the text ourself if we want black font color
{
    SolidBrush^ br=gcnew SolidBrush(Color::Black);
    e->Graphics->DrawString(Text,Font,br,0,0);
    ControlPaint::DrawBorder(e->Graphics,this->ClientRectangle,System::Drawing::Color::Black,System::Windows::Forms::ButtonBorderStyle::Solid);
}