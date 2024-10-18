#include "Locale.hpp"
#include <cstdio>
#include <cwctype>

namespace Annasul
{
	
	bool FLocale::IsEOF(int32 ch)
	{
		return ch == EOF || ch == WEOF;
	}
	
	bool FLocale::IsSpace(int32 ch)
	{
		switch (ch) {
			case '\t':
			case '\n':
			case '\v':
			case '\f':
			case '\r':
			case ' ':
				return true;
			default:
				return false;
		}
	}
	
	bool FLocale::IsSpaceIncludeNull(int32 ch)
	{
		return EOF == ch || WEOF == ch || '\0' == ch || IsSpace(ch);
	}
	
	bool FLocale::IsNewLine(int32 ch)
	{
		return ch == '\n' || ch == '\r';
	}
	
	bool FLocale::IsDigit(int32 ch)
	{
		return ch >= '0' && ch <= '9';
	}
	
	bool FLocale::IsUpper(int32 ch)
	{
		return ch >= 'A' && ch <= 'Z';
	}
	
	bool FLocale::IsLower(int32 ch)
	{
		return ch >= 'a' && ch <= 'z';
	}
	
	bool FLocale::IsAlpha(int32 ch)
	{
		return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
	}
	
	bool FLocale::IsXDigit(int32 ch)
	{
		return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f');
	}
	
	bool FLocale::IsADigit(int32 ch)
	{
		return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
	}
	
	bool FLocale::IsPunct(int32 ch)
	{
		return (ch >= '!' && ch <= '/') || (ch >= ':' && ch <= '@') || (ch >= '[' && ch <= '`') ||
		       (ch >= '{' && ch <= '~');
	}
	
	bool FLocale::IsCtrl(int32 ch)
	{
		return (ch >= '\x00' && ch <= '\x1F') || ch == '\x7F';
	}
	
	int32 FLocale::Escape(int32 ch)
	{
		switch (ch) {
			case '0':
				return '\0';
			case '1':
				return '\1';
			case '2':
				return '\2';
			case '3':
				return '\3';
			case '4':
				return '\4';
			case '5':
				return '\5';
			case '6':
				return '\6';
			case '7':
			case 'a':
				return '\a';
			case 'b':
				return '\b';
			case 'f':
				return '\f';
			case 'n':
				return '\n';
			case 'r':
				return '\r';
			case 't':
				return '\t';
			case 'v':
				return '\v';
			default:
				return ch;
		}
	}
	
	int32 FLocale::Unescape(int32 ch)
	{
		switch (ch) {
			case '\0':
				return '0';
			case '\1':
				return '1';
			case '\2':
				return '2';
			case '\3':
				return '3';
			case '\4':
				return '4';
			case '\5':
				return '5';
			case '\6':
				return '6';
			case '\a': // '\7'
				return 'a';
			case '\b':
				return 'b';
			case '\f':
				return 'f';
			case '\n':
				return 'n';
			case '\r':
				return 'r';
			case '\t':
				return 't';
			case '\v':
				return 'v';
			default:
				return ch;
		}
	}
}

