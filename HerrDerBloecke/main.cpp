#include "MainWindow.h"

using namespace HerrDerBloecke;

[STAThreadAttribute]
int main(array<System::String^> ^args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false); 

    MainWindow^ mainWindow = gcnew MainWindow();
    return 0;
}
