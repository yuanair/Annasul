#include <GenericPlatform.hpp>
#include <GenericTimer.hpp>
#include <Reflect.hpp>
#include <Container/StringView.hpp>
#include <iostream>
#include <chrono>

using namespace Annasul;

class Allocator {
public:
	
	using SizeType = SIZE_T;
	
};

template<typename InElementType>
class ArrayAllocator {
public:
	

};

template<typename InElementType, typename InAllocatorType = Allocator>
class DynamicArray {
public:
	
	using ElementType = InElementType;
	using AllocatorType = InAllocatorType;
	using SizeType = typename Allocator::SizeType;

public:
	
	DynamicArray();

private:
	
	AllocatorType allocator;
	SizeType size;
	SizeType capacity;
	
};

class Hello {
A_CLASS_BODY(Hello)
	
};

template<typename T>
T *func(T n)
{
	if (n <= 0) return nullptr;
	T *arr = new T[n];
	T lastLayer = 1;
	arr[0] = 1;
	for (uint64_t i = 1; i < n; ++i) {
		arr[i] = arr[i - 1] + (lastLayer += i + 1);
	}
	return arr;
}


int main()
{
	std::wcout << Annasul::FPlatform::PLATFORM_NAME << " " << std::endl
	           << "Is User Admin: " << Annasul::FPlatform::IsUserAnAdmin() << std::endl;
	
	std::wcout << Hello::GetStaticName().GetNullTerminatedData() << std::endl;
	
	int32_t n = 120;
	Annasul::FTimer timer;
	auto *arr = func<int64_t>(n);
	timer.Tick();
	
	std::wcout << "Time taken by function: " << timer.GetDeltaTime() << " seconds" << std::endl;
	
	for (int32_t i = 0; i < n; ++i) {
		std::wcout << i + 1 << ": " << arr[i] << std::endl;
	}
	
	return 0;
}
