
#pragma once

#include "GenericRenderEngineFactory.hpp"

namespace Annasul
{
	
	struct FLinuxRenderEngineFactory : public FGenericRenderEngineFactory {
		
		static FGenericRenderEngine *Create(ERenderEngineType type);
	};
	
	typedef FLinuxRenderEngineFactory FRenderEngineFactory;
	
}
