#include "World.hpp"

namespace Annasul
{
	
	FWorld::FWorld()
	{
	
	}
	
	FWorld::~FWorld()
	{
	
	}
	
	FWorld &FWorld::Get()
	{
		static FWorld world;
		return world;
	}
	
	void FWorld::OnStart()
	{
	
	}
	
	void FWorld::OnTick()
	{
	
	}
	
	void FWorld::OnQuit()
	{
	
	}
}
