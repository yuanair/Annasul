#include "Platform.hpp"
#include "World.hpp"
#include "Launch.hpp"

namespace Annasul
{
	
	bool bIsRunning = true;
	int32 exitCode = 0;
	
	void Quit(int32 inExitCode)
	{
		bIsRunning = false;
		exitCode = inExitCode;
	}
	
	int32 Main()
	{
		FWorld::Get().OnStart();
		AnnasulMain();
		do {
			FWorld::Get().OnTick();
		} while (bIsRunning);
		FWorld::Get().OnQuit();
		return exitCode;
	}
}

int main(int argc, char *argv[])
{
	return Annasul::Main();
}
