#pragma once

#include "GenericDebug.hpp"

namespace Annasul
{
	
	class FWindowsDebug : public FGenericDebug
	{
	public:
		
		static FWindowsDebug &Get();
	
	public:
		
		void Log(EDebugLevel level, FStringView message, SourceLocation location = SourceLocation::current()) override;
		
		bool ConditionLog(bool condition, EDebugLevel level, FStringView message,
		                  SourceLocation location = SourceLocation::current()) override;
	
	public:
		
		void LastErrorLog(EDebugLevel level, FStringView message, SourceLocation location = SourceLocation::current());
		
		bool LastErrorConditionLog(bool condition, EDebugLevel level, FStringView message,
		                           SourceLocation location = SourceLocation::current());
		
		void ErrorLog(EDebugLevel level, FStringView message, uint32 errorCode,
		              SourceLocation location = SourceLocation::current());
		
		bool ErrorConditionLog(bool condition, EDebugLevel level, FStringView message, uint32 errorCode,
		                       SourceLocation location = SourceLocation::current());
	
	private:
		
		FWindowsDebug() = default;
		
		~FWindowsDebug() override = default;
	};
	
	typedef FWindowsDebug FDebug;
	
}
