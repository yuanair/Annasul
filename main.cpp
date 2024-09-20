#include <Core.hpp>
#include <Container/Pair.hpp>
#include <GenericWindow.hpp>
#include <GenericApplication.hpp>

using namespace Annasul;

int main()
{
	FCore::Test();
	FWindowClass windowClass;
	windowClass.Register({TEXT("Test Window")});
	FWindow window;
	window.Create(windowClass, {TEXT("Test Window"), 400, 300, false});
	window.Show();
	return FApplication::Loop();
}
