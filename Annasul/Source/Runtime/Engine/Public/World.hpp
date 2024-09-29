#pragma once

#include "Platform.hpp"

#include <vector>

#pragma message("TODO: delete this include<vector>")

namespace Annasul
{
	
	class FScene;
	
	class FWorld final
	{
		
		friend int32 Main();
	
	public:
		
		static FWorld &Get();
	
	public:
	
	
	private:
		
		void OnStart();
		
		void OnTick();
		
		void OnQuit();
	
	private:
		
		FWorld();
		~FWorld();
	
	private:
		
		std::vector<FScene *> m_scenes;
		
	};
	
	
}
