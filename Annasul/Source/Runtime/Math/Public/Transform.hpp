#pragma once

#include "Vector.hpp"

namespace Annasul
{
	
	template<typename InElementType>
	class FTransform3D {
	public:
		
		static const FTransform3D Identity;
	
	public:
		
		FTransform3D() {}
		
		~FTransform3D() = default;
	
	public:
	
	
	protected:
		
		VectorRegister4<InElementType> Rotation;
		VectorRegister4<InElementType> Translation;
		VectorRegister4<InElementType> Scale;
		
	};
	
	typedef FTransform3D<float> FTransformF;
	typedef FTransform3D<double> FTransform;
	
}
