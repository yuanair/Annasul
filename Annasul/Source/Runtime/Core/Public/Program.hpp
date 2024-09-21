#pragma once

namespace Annasul
{
	
	class FProgram
	{
	public:
		
		FProgram() = default;
		virtual ~FProgram() = default;
	
	public:
		
		virtual void Tick(double deltaTime) = 0;
		
	};
	
}
