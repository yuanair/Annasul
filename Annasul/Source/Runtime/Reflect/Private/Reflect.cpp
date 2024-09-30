
#include "Reflect.hpp"

namespace Annasul
{
	
	FReflect &FReflect::Get()
	{
		static FReflect instance;
		return instance;
	}
}
