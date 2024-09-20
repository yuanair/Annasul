#pragma once

#include "GenericDebug.hpp"

namespace Annasul
{
	
	class FWindowsDebug : public FGenericDebug
	{
	public:
		
		static FWindowsDebug& Get();
	
	public:
		
		void Log(EDebugLevel level, FStringView message) override;
		
		bool ConditionLog(bool condition, EDebugLevel level, FStringView message) override;
	
	public:
		
		void LastErrorLog(EDebugLevel level, FStringView message);
		
		bool LastErrorConditionLog(bool condition, EDebugLevel level, FStringView message);
	
	private:
		
		FWindowsDebug() = default;
		
		~FWindowsDebug() override = default;
	};
	
	typedef FWindowsDebug FDebug;
	
}
