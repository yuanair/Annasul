#include <iostream>
#include <chrono>

template<typename T>
class DynamicArray
{
public:
	
	using SizeType = size_t;

public:


private:
	
	T *arr;
	SizeType size;
	SizeType capacity;
	
};

template<typename T>
T *func(T n)
{
	if (n <= 0) return nullptr;
	T *arr = new T[n];
	T lastLayer = 1;
	arr[0] = 1;
	for (uint64_t i = 1; i < n; ++i)
	{
		arr[i] = arr[i - 1] + (lastLayer += i + 1);
	}
	return arr;
}


int main()
{
	int32_t n = 120;
	auto beginTime = std::chrono::high_resolution_clock::now();
	auto *arr = func<int64_t>(n);
	auto endTime = std::chrono::high_resolution_clock::now();
	
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - beginTime);
	std::cout << "Time taken by function: " << duration.count() << " nanoseconds" << std::endl;
	
	for (int32_t i = 0; i < n; ++i)
	{
		std::cout << i + 1 << ": " << arr[i] << std::endl;
	}
	
	return 0;
}
