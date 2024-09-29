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
	
	public:
		
		void LastErrorLog(EDebugLevel level, FStringView message, SourceLocation location = SourceLocation::current());
		
		FORCEINLINE bool LastErrorConditionLog(bool condition, EDebugLevel level, FStringView message,
		                                       SourceLocation location = SourceLocation::current())
		{
			if (condition) return true;
			LastErrorLog(level, message, location);
			return false;
		}
		
		void ErrorLog(EDebugLevel level, FStringView message, uint32 errorCode,
		              SourceLocation location = SourceLocation::current());
		
		FORCEINLINE bool ErrorConditionLog(bool condition, EDebugLevel level, FStringView message, uint32 errorCode,
		                                   SourceLocation location = SourceLocation::current())
		{
			if (condition) return true;
			ErrorLog(level, message, errorCode, location);
			return false;
		}
		
		bool ErrorLogIfFailed(EDebugLevel level, FStringView message, uint32 errorCode,
		                      SourceLocation location = SourceLocation::current());
	
	private:
		
		FWindowsDebug() = default;
		
		~FWindowsDebug() override = default;
	};
	
	typedef FWindowsDebug FDebug;
	
}
