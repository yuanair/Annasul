
#pragma once

#include "Platform.hpp"

namespace Annasul
{
	
	class FLocale {
	public:
		
		/// EOF, WEOF
		static bool IsEOF(int32 ch);
		
		static bool IsSpace(int32 ch);
		
		/// 0, EOF, WEOF
		static bool IsSpaceIncludeNull(int32 ch);
		
		static bool IsNewLine(int32 ch);
		
		static bool IsDigit(int32 ch);
		
		static bool IsUpper(int32 ch);
		
		static bool IsLower(int32 ch);
		
		static bool IsAlpha(int32 ch);
		
		static bool IsXDigit(int32 ch);
		
		static bool IsADigit(int32 ch);
		
		static bool IsPunct(int32 ch);
		
		static bool IsCtrl(int32 ch);
		
		static int32 Escape(int32 ch);
		
		static int32 Unescape(int32 ch);
		
	};
	
}
