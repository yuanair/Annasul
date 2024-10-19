#pragma once

#include "Platform.hpp"
#include "Utility.hpp"

#include <emmintrin.h>

#if ANNASUL_MATH_USE_AVX

#include <immintrin.h>

#endif

namespace Annasul
{
	struct VectorRegisterConstInit {};
	
	typedef __m128 VectorRegister4Float;
	
	typedef __m128i VectorRegister4Int;
	
	typedef __m128i VectorRegister2Int64;
	
	typedef __m128d VectorRegister2Double;
	
	typedef struct {
		VectorRegister4Float val[4];
	} VectorRegister4x4Float;
	
	struct VectorRegister4Double {
#if !ANNASUL_MATH_USE_AVX
		VectorRegister2Double XY;
		VectorRegister2Double ZW;
		
		FORCEINLINE VectorRegister2Double GetXY() const { return XY; }
		
		FORCEINLINE VectorRegister2Double GetZW() const { return ZW; }

#else
		union {
			struct {
				VectorRegister2Double XY;
				VectorRegister2Double ZW;
			};
			__m256d XYZW;
		};
		
		// Use in preference when reading XY or ZW to extract values, it's better on MSVC than the generated memory reads.
		FORCEINLINE VectorRegister2Double GetXY() const
		{
			return _mm256_extractf128_pd(XYZW, 0);
		} // { return _mm256_castpd256_pd128(XYZW); } // Possible MSVC compiler bug in optimized bugs when using this cast, but can be more efficient.
		FORCEINLINE VectorRegister2Double GetZW() const { return _mm256_extractf128_pd(XYZW, 1); }

#endif
		
		FORCEINLINE VectorRegister4Double() = default;
		
		FORCEINLINE VectorRegister4Double(const VectorRegister2Double &InXY, const VectorRegister2Double &InZW)
		{
#if UE_PLATFORM_MATH_USE_AVX
			XYZW = _mm256_setr_m128d(InXY, InZW);
#else
			XY = InXY;
			ZW = InZW;
#endif
		}
		
		FORCEINLINE constexpr VectorRegister4Double(VectorRegister2Double InXY, VectorRegister2Double InZW,
		                                            VectorRegisterConstInit)
			: XY(InXY), ZW(InZW) {}
		
		// Construct from a vector of 4 floats
		FORCEINLINE VectorRegister4Double(const VectorRegister4Float &FloatVector)
		{
#if !ANNASUL_MATH_USE_AVX
			XY = _mm_cvtps_pd(FloatVector);
			ZW = _mm_cvtps_pd(_mm_movehl_ps(FloatVector, FloatVector));
#else
			XYZW = _mm256_cvtps_pd(FloatVector);
#endif
		}
		
		// Assign from a vector of 4 floats
		FORCEINLINE VectorRegister4Double &operator=(const VectorRegister4Float &FloatVector)
		{
#if !ANNASUL_MATH_USE_AVX
			XY = _mm_cvtps_pd(FloatVector);
			ZW = _mm_cvtps_pd(_mm_movehl_ps(FloatVector, FloatVector));
#else
			XYZW = _mm256_cvtps_pd(FloatVector);
#endif
			return *this;
		}

#if ANNASUL_MATH_USE_AVX
		// Convenience for things like 'Result = _mm256_add_pd(...)'
		FORCEINLINE VectorRegister4Double(const __m256d &Register)
		{
			XYZW = Register;
		}
		
		// Convenience for things like 'Result = _mm256_add_pd(...)'
		FORCEINLINE VectorRegister4Double &operator=(const __m256d &Register)
		{
			XYZW = Register;
			return *this;
		}
		
		// Convenience for passing VectorRegister4Double to _mm256_* functions without needing '.XYZW'
		FORCEINLINE operator __m256d() const
		{
			return XYZW;
		}

#endif
	
	};
	
	template<typename InElementType>
	using VectorRegister4 = std::conditional_t<std::is_same_v<InElementType, float>, VectorRegister4Float, std::conditional_t<std::is_same_v<InElementType, double>, VectorRegister4Double, void>>;
	
}