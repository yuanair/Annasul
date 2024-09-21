#pragma once

#include "Platform.hpp"

#if __cplusplus >= 201703L
#include <source_location>
#endif

namespace Annasul
{
	
	#if __cplusplus >= 201703L
	
	using SourceLocation = std::source_location;
	
	#else
	
	struct SourceLocation
	{
		[[nodiscard]] static constexpr SourceLocation current(
			const uint32 line = __builtin_LINE(),
			const uint32 column = __builtin_COLUMN(),
			const char *const file = __builtin_FILE(),
#if defined(__clang__) || defined(__EDG__)
			const char* const function = __builtin_FUNCTION()
#else
			const char *const function = __builtin_FUNCSIG()
#endif
		) noexcept
		{
			SourceLocation result{};
			result.m_line = line;
			result.m_column = column;
			result.m_file = file;
			result.m_function = function;
			return result;
		}
		
		constexpr SourceLocation() noexcept = default;
		
		[[nodiscard]] constexpr uint32 line() const noexcept
		{
			return m_line;
		}
		
		[[nodiscard]] constexpr uint32 column() const noexcept
		{
			return m_column;
		}
		
		[[nodiscard]] constexpr const char *file_name() const noexcept
		{
			return m_file;
		}
		
		[[nodiscard]] constexpr const char *function_name() const noexcept
		{
			return m_function;
		}
	
	private:
		uint32 m_line{};
		uint32 m_column{};
		const char *m_file = "";
		const char *m_function = "";
	};
	
	#endif
	
}
