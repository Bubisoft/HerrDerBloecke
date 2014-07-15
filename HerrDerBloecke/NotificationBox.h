#pragma once


namespace HdB{
    using namespace System;
    using namespace System::IO;
    using namespace System::Windows;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    ref class NotificationBox : TextBox
    {
    public:
        NotificationBox(Control^ target, int x, int y);
        ~NotificationBox();

        void SendMessage(String^ msg);  // Appends the message to the textbox

        void BoxGotFocus(Object^ sender, EventArgs^ e);

        void Resize(Control^ hwnd);

        /** Location of the label above the TextBox */
        property Point BoxLocation;
    private:
        Label^ mTitle;
    };
}

