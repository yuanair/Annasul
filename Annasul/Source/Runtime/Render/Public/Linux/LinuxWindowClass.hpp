
#pragma once

#include "GenericWindowClass.hpp"

namespace Annasul
{
	
	class FLinuxWindowClass : public FGenericWindowClass {
	public:
		
		void Register(const FWindowClassDesc &desc) override;
		
		void Unregister() override;
		
		bool IsRegistered() const override;
		
	};
	
	typedef FLinuxWindowClass FWindowClass;
	
}
