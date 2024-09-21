#pragma once

#include "GenericRenderEngineFactory.hpp"

namespace Annasul
{
	
	struct FWindowsRenderEngineFactory : public FGenericRenderEngineFactory
	{
		
		static FGenericRenderEngine *Create(ERenderEngineType type);
		
	};
	
	typedef FWindowsRenderEngineFactory FRenderEngineFactory;
	
}
