#pragma once


namespace HdB{
    using namespace System;
    using namespace System::IO;
    using namespace System::Windows;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    ref class NotificationBox:TextBox
    {
    public:
        NotificationBox(Control^ target,int x,int y);

        void SendMessage(String^ msg);  //appends the message to the textbox

        void Resize(Control^ hwnd);

    public:
        /**Location of the label above the TextBox
        */
        Point _Location;
    private:
        Label^ mTitle;
    };
}

