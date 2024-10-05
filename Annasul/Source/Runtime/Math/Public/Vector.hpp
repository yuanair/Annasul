#pragma once

#include "Platform.hpp"
#include "Utility.hpp"

namespace Annasul
{
	
	template<typename InElementType, SIZE_T InSize>
	class TFVector {
		
		template<typename, SIZE_T>
		friend
		class TFVector;
	
	public:
		
		typedef InElementType ElementType;
	
	public:
		
		static_assert(InSize > 0, "Vector size must be greater than 0");
		
		static constexpr SIZE_T SIZE = InSize;
	
	public:
		
		FORCEINLINE TFVector() : m_data{} {}
		
		template<typename... InArgumentTypes>
		FORCEINLINE explicit TFVector(InArgumentTypes &&... args)
			: m_data{std::forward<InArgumentTypes>(args)...}
		{
			static_assert(sizeof...(args) == SIZE, "Number of arguments does not match vector size");
		}
		
		FORCEINLINE explicit TFVector(const ElementType (&value)[SIZE])
		{
			for (SIZE_T i = 0; i < SIZE; ++i) {
				m_data[i] = value[i];
			}
		}
	
	public:
		
		FORCEINLINE bool operator==(const TFVector &other) const
		{
			for (SIZE_T i = 0; i < SIZE; ++i) {
				if (m_data[i] != other.m_data[i]) {
					return false;
				}
			}
			return true;
		}
		
		FORCEINLINE bool operator!=(const TFVector &other) const
		{
			for (SIZE_T i = 0; i < SIZE; ++i) {
				if (m_data[i] != other.m_data[i]) {
					return true;
				}
			}
			return false;
		}
		
		FORCEINLINE ElementType &operator[](SIZE_T index) { return m_data[index]; }
		
		FORCEINLINE const ElementType &operator[](SIZE_T index) const { return m_data[index]; }
		
		template<typename FunctionInElementType, SIZE_T FunctionInSize>
		FORCEINLINE TFVector<ElementType, FunctionInSize>
		operator[](const FunctionInElementType (&indices)[FunctionInSize])
		{
			TFVector<ElementType, FunctionInSize> result;
			for (SIZE_T i = 0; i < FunctionInSize; ++i) {
				switch (indices[i]) {
					case 'x':
						result.m_data[i] = m_data[0];
						break;
					case 'y':
						result.m_data[i] = m_data[1];
						break;
					case 'z':
						result.m_data[i] = m_data[2];
						break;
					case 'w':
						result.m_data[i] = m_data[3];
						break;
					case 'r':
						result.m_data[i] = m_data[0];
						break;
					case 'g':
						result.m_data[i] = m_data[1];
						break;
					case 'b':
						result.m_data[i] = m_data[2];
						break;
					case 'a':
						result.m_data[i] = m_data[3];
						break;
					default:
						result.m_data[i] = {};
						break;
				}
			}
			return result;
		}
	
	public:
		
		template<class CastElementType>
		FORCEINLINE TFVector<CastElementType, SIZE> Cast() const
		{
			TFVector<CastElementType, SIZE> result;
			for (SIZE_T i = 0; i < SIZE; ++i) {
				result.m_data[i] = static_cast<CastElementType>(m_data[i]);
			}
			return result;
		}
	
	public:
		
		FORCEINLINE void swap(TFVector &other)
		{
			using std::swap;
			swap(m_data, other.m_data);
		}
		
		FORCEINLINE ElementType &x() { return m_data[0]; }
		
		FORCEINLINE ElementType &y()
		{
			static_assert(SIZE >= 2, "Vector size must be greater than or equal to 2");
			return m_data[1];
		}
		
		FORCEINLINE ElementType &z()
		{
			static_assert(SIZE >= 3, "Vector size must be greater than or equal to 3");
			return m_data[2];
		}
		
		FORCEINLINE ElementType &w()
		{
			static_assert(SIZE >= 4, "Vector size must be greater than or equal to 4");
			return m_data[3];
		}
		
		FORCEINLINE const ElementType &x() const { return m_data[0]; }
		
		FORCEINLINE const ElementType &y() const
		{
			static_assert(
				SIZE >= 2, "Vector size must be greater than or equal to 2"
			);
			return m_data[1];
		}
		
		FORCEINLINE const ElementType &z() const
		{
			static_assert(
				SIZE >= 3, "Vector size must be greater than or equal to 3"
			);
			return m_data[2];
		}
		
		FORCEINLINE const ElementType &w() const
		{
			static_assert(
				SIZE >= 4, "Vector size must be greater than or equal to 4"
			);
			return m_data[3];
		}
		
		FORCEINLINE ElementType &r() { return x(); }
		
		FORCEINLINE ElementType &g() { return y(); }
		
		FORCEINLINE ElementType &b() { return z(); }
		
		FORCEINLINE ElementType &a() { return w(); }
		
		FORCEINLINE const ElementType &r() const { return x(); }
		
		FORCEINLINE const ElementType &g() const { return y(); }
		
		FORCEINLINE const ElementType &b() const { return z(); }
		
		FORCEINLINE const ElementType &a() const { return w(); }
	
	private:
		
		ElementType m_data[SIZE];
		
	};
	
	template<typename... InArgumentTypes>
	FORCEINLINE TFVector<typename std::common_type<InArgumentTypes...>::type, sizeof...(InArgumentTypes)>
	MakeVector(InArgumentTypes &&... args)
	{
		return TFVector<typename std::common_type<InArgumentTypes...>::type, sizeof...(InArgumentTypes)>{
			std::forward<InArgumentTypes>(args)...
		};
	}
	
	typedef TFVector<float, 2> FVector2f;
	typedef TFVector<float, 3> FVector3f;
	typedef TFVector<float, 4> FVector4f;
	typedef TFVector<double, 2> FVector2d;
	typedef TFVector<double, 3> FVector3d;
	typedef TFVector<double, 4> FVector4d;
	typedef TFVector<int32, 2> FVector2i;
	typedef TFVector<int32, 3> FVector3i;
	typedef TFVector<int32, 4> FVector4i;
	typedef TFVector<uint32, 2> FVector2ui;
	typedef TFVector<uint32, 3> FVector3ui;
	typedef TFVector<uint32, 4> FVector4ui;
	
}
