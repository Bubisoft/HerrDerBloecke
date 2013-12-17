#include "MainWindow.h"

using namespace HerrDerBloecke;
using namespace SlimDX;

[STAThreadAttribute]
int main(array<System::String^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

    MainWindow^ mainWindow = gcnew MainWindow();
    MessagePump::Run(mainWindow, mainWindow->Loop);
	return 0;
}
