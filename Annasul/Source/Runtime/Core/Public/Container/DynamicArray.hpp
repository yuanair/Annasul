#pragma once

#include "GenericPlatform.hpp"
#include "Utility.hpp"
#include "HeapAllocator.hpp"
#include "Helpers.hpp"

#include <functional>

namespace Annasul
{
	
	template<typename InElementType, typename InAllocatorType = TFHeapAllocator<int32>>
	class TFDynamicArray {
	public:
		
		using ElementType = InElementType;
		using AllocatorType = InAllocatorType;
		using SizeType = typename InAllocatorType::SizeType;
		using USizeType = typename InAllocatorType::USizeType;
	
	public:
		
		FORCEINLINE TFDynamicArray() : m_allocator(), m_capacity(m_allocator.GetInitialCapacity()), m_size(0) {}
		
		FORCEINLINE TFDynamicArray(std::initializer_list<ElementType> initList)
		{
			CopyToEmpty(initList.begin(), (SizeType) initList.size(), 0);
		}
		
		template<SizeType InSize>
		FORCEINLINE TFDynamicArray(const ElementType (&data)[InSize]) // NOLINT(*-explicit-constructor)
		{
			CopyToEmpty(data, InSize, 0);
		}
		
		FORCEINLINE TFDynamicArray(const ElementType *data, SizeType num)
		{
			if (num < 0) {
				Private::OnInvalidArrayNum((unsigned long long) (USizeType) num);
			}
			CopyToEmpty(data, num, 0);
		}
		
		FORCEINLINE TFDynamicArray(const TFDynamicArray &other)
		{
			CopyToEmpty(other.GetData(), other.GetSize(), 0);
		}
		
		FORCEINLINE TFDynamicArray(TFDynamicArray &&other) noexcept
		{
			Move(*this, other, 0);
		}
		
		FORCEINLINE ~TFDynamicArray()
		{
			DestructItems(GetData(), m_size);
		}
	
	public:
		
		FORCEINLINE TFDynamicArray &operator=(std::initializer_list<ElementType> initList)
		{
			DestructItems(GetData(), m_size);
			CopyToEmpty(initList.begin(), (SizeType) initList.size(), m_capacity);
			return *this;
		}
		
		template<SizeType InSize>
		FORCEINLINE TFDynamicArray &operator=(const ElementType (&data)[InSize])
		{
			DestructItems(GetData(), m_size);
			CopyToEmpty(data, InSize, m_capacity);
			return *this;
		}
		
		FORCEINLINE TFDynamicArray &operator=(const TFDynamicArray &other)
		{
			if (this != &other) {
				DestructItems(GetData(), m_size);
				CopyToEmpty(other.GetData(), other.GetSize(), m_capacity);
			}
			return *this;
		}
		
		FORCEINLINE TFDynamicArray &operator=(TFDynamicArray &&other) noexcept
		{
			if (this != &other) {
				DestructItems(GetData(), m_size);
				Move(*this, other, m_capacity);
			}
			return *this;
		}
		
		FORCEINLINE ElementType &operator[](SizeType index)
		{
			RangeCheck(index);
			return GetData()[index];
		}
		
		FORCEINLINE const ElementType &operator[](SizeType index) const
		{
			RangeCheck(index);
			return GetData()[index];
		}
		
		FORCEINLINE bool operator==(const TFDynamicArray &other) const { return Equals(other); }
		
		FORCEINLINE bool operator!=(const TFDynamicArray &other) const { return !Equals(other); }
		
		FORCEINLINE TFDynamicArray &operator+=(ElementType element)
		{
			Add(element);
			return *this;
		}
		
		FORCEINLINE TFDynamicArray &operator+=(const TFDynamicArray &other)
		{
			Append(other);
			return *this;
		}
		
		FORCEINLINE TFDynamicArray &operator+=(TFDynamicArray &&other)
		{
			Append(std::move(other));
			return *this;
		}
		
		FORCEINLINE TFDynamicArray &operator+=(std::initializer_list<ElementType> initList)
		{
			Append(initList);
			return *this;
		}
		
		template<SizeType InSize>
		FORCEINLINE TFDynamicArray &operator+=(const ElementType (&data)[InSize])
		{
			Append(data);
			return *this;
		}
	
	public:
		
		FORCEINLINE void RangeCheck(SizeType index) const
		{
			CheckInvariants();
			assert(index >= 0 && index < m_size);
		}
		
		FORCEINLINE void CheckInvariants() const
		{
			assert((m_size >= 0) & (m_capacity >= m_size));
		}
		
		FORCEINLINE void CheckAddress(const ElementType *Addr) const
		{
			assert(Addr < GetData() || Addr >= (GetData() + m_capacity));
		}
		
		FORCEINLINE SizeType AddUninitialized()
		{
			CheckInvariants();
			
			const auto oldNum = (USizeType) m_size;
			const USizeType newNum = oldNum + (USizeType) 1;
			m_size = (SizeType) newNum;
			if (newNum > (USizeType) m_capacity) {
				ResizeGrow((SizeType) oldNum);
			}
			
			return oldNum;
		}
		
		FORCEINLINE SizeType AddUninitialized(SizeType count)
		{
			CheckInvariants();
			assert(count >= 0);
			
			const auto oldNum = (USizeType) m_size;
			const USizeType newNum = oldNum + (USizeType) count;
			m_size = (SizeType) newNum;
			if ((USizeType) count > (USizeType) m_capacity - oldNum) {
				ResizeGrow((SizeType) oldNum);
			}
			
			return oldNum;
		}
		
		template<typename... ArgsType>
		FORCEINLINE SizeType Emplace(ArgsType &&... args)
		{
			const SizeType index = AddUninitialized();
			new(GetData() + index) ElementType(std::forward<ArgsType>(args)...);
			return index;
		}
		
		template<typename... ArgsType>
		FORCEINLINE ElementType &Emplace_GetRef(ArgsType &&... args)
		{
			const SizeType index = AddUninitialized();
			ElementType *Ptr = GetData() + index;
			new(Ptr) ElementType(std::forward<ArgsType>(args)...);
			return *Ptr;
		}
		
		FORCEINLINE SizeType Add(ElementType &&Item)
		{
			CheckAddress(&Item);
			return Emplace(std::move(Item));
		}
		
		FORCEINLINE SizeType Add(const ElementType &Item)
		{
			CheckAddress(&Item);
			return Emplace(Item);
		}
		
		FORCEINLINE ElementType &Add_GetRef(ElementType &&Item)
		{
			CheckAddress(&Item);
			return Emplace_GetRef(std::move(Item));
		}
		
		FORCEINLINE ElementType &Add_GetRef(const ElementType &Item)
		{
			CheckAddress(&Item);
			return Emplace_GetRef(Item);
		}
		
		FORCEINLINE bool Equals(const TFDynamicArray &other) const;
		
		template<typename OtherElementType, typename OtherAllocatorType>
		void Append(const TFDynamicArray<OtherElementType, OtherAllocatorType> &source)
		{
			assert((void *) this != (void *) &source);
			
			SizeType sourceCount = source.GetSize();
			
			if (!sourceCount) {
				return;
			}
			
			SizeType pos = AddUninitialized(sourceCount);
			ConstructItems<ElementType>(GetData() + pos, source.GetData(), sourceCount);
		}
		
		template<typename OtherElementType, typename OtherAllocator>
		void Append(TFDynamicArray<OtherElementType, OtherAllocator> &&source)
		{
			assert((void *) this != (void *) &source);
			
			SizeType sourceCount = source.GetSize();
			
			if (!sourceCount) {
				return;
			}
			
			SizeType pos = AddUninitialized(sourceCount);
			RelocateConstructItems<ElementType>(GetData() + pos, source.GetData(), sourceCount);
			source.m_size = 0;
		}
		
		FORCEINLINE void Append(const ElementType *ptr, SizeType count)
		{
			assert(ptr != nullptr || count == 0);
			
			SizeType pos = AddUninitialized(count);
			ConstructItems<ElementType>(GetData() + pos, ptr, count);
		}
		
		
		template<SizeType InSize>
		FORCEINLINE void Append(const ElementType (&data)[InSize])
		{
			SizeType pos = AddUninitialized(InSize);
			ConstructItems<ElementType>(GetData() + pos, data, InSize);
		}
		
		FORCEINLINE void Append(std::initializer_list<ElementType> InitList)
		{
			auto count = (SizeType) InitList.size();
			
			SizeType pos = AddUninitialized(count);
			ConstructItems<ElementType>(GetData() + pos, InitList.begin(), count);
		}
		
		FORCEINLINE TFDynamicArray &Insert(SizeType index, ElementType element);
		
		FORCEINLINE TFDynamicArray &Insert(SizeType index, TFDynamicArray other);
		
		SizeType Insert(std::initializer_list<ElementType> InitList, const SizeType InIndex)
		{
			SizeType NumNewElements = (SizeType) InitList.size();
			
			InsertUninitializedImpl(InIndex, NumNewElements);
			ConstructItems<ElementType>(GetData() + InIndex, InitList.begin(), NumNewElements);
			
			return InIndex;
		}
		
		template<typename OtherAllocator>
		SizeType Insert(const TFDynamicArray<ElementType, OtherAllocator> &items, const SizeType inIndex)
		{
			assert((const void *) this != (const void *) &items);
			
			auto numNewElements = items.GetSize();
			
			InsertUninitializedImpl(inIndex, numNewElements);
			ConstructItems<ElementType>(GetData() + inIndex, items.GetData(), numNewElements);
			
			return inIndex;
		}
		
		template<typename OtherAllocator>
		SizeType Insert(TFDynamicArray<ElementType, OtherAllocator> &&items, const SizeType inIndex)
		{
			assert((const void *) this != (const void *) &items);
			
			auto numNewElements = items.GetSize();
			
			InsertUninitializedImpl(inIndex, numNewElements);
			RelocateConstructItems<ElementType>(GetData() + inIndex, items.GetData(), numNewElements);
			items.m_size = 0;
			
			return inIndex;
		}
		
		SizeType Insert(const ElementType *ptr, SizeType count, SizeType index)
		{
			assert(ptr != nullptr);
			
			InsertUninitializedImpl(index, count);
			ConstructItems<ElementType>(GetData() + index, ptr, count);
			
			return index;
		}
		
		SizeType Insert(ElementType &&item, SizeType index)
		{
			CheckAddress(&item);
			
			InsertUninitializedImpl(index);
			new(GetData() + index) ElementType(MoveTempIfPossible(item));
			return index;
		}
		
		SizeType Insert(const ElementType &item, SizeType index)
		{
			CheckAddress(&item);
			
			InsertUninitializedImpl(index);
			new(GetData() + index) ElementType(item);
			return index;
		}
		
		ElementType &Insert_GetRef(ElementType &&item, SizeType index)
		{
			CheckAddress(&item);
			
			InsertUninitializedImpl(index);
			ElementType *ptr = GetData() + index;
			new(ptr) ElementType(MoveTempIfPossible(item));
			return *ptr;
		}
		
		ElementType &Insert_GetRef(const ElementType &item, SizeType index)
		{
			CheckAddress(&item);
			
			InsertUninitializedImpl(index);
			ElementType *ptr = GetData() + index;
			new(ptr) ElementType(item);
			return *ptr;
		}
		
		void Reset(SizeType NewSize = 0)
		{
			if (NewSize < 0) {
				Private::OnInvalidArrayNum((unsigned long long) (USizeType) NewSize);
			}
			
			if (NewSize <= m_capacity) {
				DestructItems(GetData(), m_size);
				m_size = 0;
			} else {
				Empty(NewSize);
			}
		}
		
		void Empty(SizeType Slack = 0)
		{
			if (Slack < 0) {
				Private::OnInvalidArrayNum((unsigned long long) (USizeType) Slack);
			}
			
			DestructItems(GetData(), m_size);
			
			assert(Slack >= 0);
			m_size = 0;
			
			if (m_capacity != Slack) {
				ResizeTo(Slack);
			}
		}
		
		FORCEINLINE bool Find(const ElementType &Item, SizeType &Index) const
		{
			Index = this->Find(Item);
			return Index != INDEX_NONE;
		}
		
		FORCEINLINE SizeType Find(const ElementType &Item) const
		{
			const ElementType *RESTRICT Start = GetData();
			for (const ElementType *RESTRICT Data = Start, *RESTRICT DataEnd = Data + m_size; Data != DataEnd; ++Data) {
				if (*Data == Item) {
					return static_cast<SizeType>(Data - Start);
				}
			}
			return INDEX_NONE;
		}
		
		FORCEINLINE bool FindLast(const ElementType &Item, SizeType &Index) const
		{
			Index = this->FindLast(Item);
			return Index != INDEX_NONE;
		}
		
		FORCEINLINE SizeType FindLast(const ElementType &Item) const
		{
			for (const ElementType *RESTRICT Start = GetData(), *RESTRICT Data = Start + m_size; Data != Start;) {
				--Data;
				if (*Data == Item) {
					return static_cast<SizeType>(Data - Start);
				}
			}
			return INDEX_NONE;
		}
		
		SizeType RemoveSingle(const ElementType &item)
		{
			SizeType index = Find(item);
			if (index == INDEX_NONE) {
				return 0;
			}
			
			auto *removePtr = GetData() + index;
			
			DestructItems(removePtr, 1);
			RelocateConstructItems<ElementType>(removePtr, removePtr + 1, m_size - (index + 1));
			
			--m_size;
			
			return 1;
		}
		
		SizeType Remove(const ElementType &Item)
		{
			CheckAddress(&Item);
			return RemoveAll([&Item](ElementType &Element) { return Element == Item; });
		}
		
		/**
		 * Remove all instances that match the predicate, maintaining order but not indices
		 * Optimized to work with runs of matches/non-matches
		 *
		 * @param Predicate Predicate class instance
		 * @returns Number of removed elements.
		 * @see Add, Insert, RemoveAllSwap, RemoveSingle, RemoveSwap
		 */
		template<class PREDICATE_CLASS>
		SizeType RemoveAll(const PREDICATE_CLASS &Predicate)
		{
			const SizeType originalNum = m_size;
			if (!originalNum) {
				return 0;
			}
			
			ElementType *data = GetData();
			
			SizeType writeIndex = 0;
			SizeType readIndex = 0;
			bool bNotMatch = !Invoke(
				Predicate, data[readIndex]
			);
			do {
				SizeType runStartIndex = readIndex++;
				while (readIndex < originalNum && bNotMatch == !Invoke(Predicate, data[readIndex])) {
					readIndex++;
				}
				SizeType runLength = readIndex - runStartIndex;
				assert(runLength > 0);
				if (bNotMatch) {
					if (writeIndex != runStartIndex) {
						RelocateConstructItems<ElementType>(data + writeIndex, data + runStartIndex, runLength);
					}
					writeIndex += runLength;
				} else {
					DestructItems(data + runStartIndex, runLength);
				}
				bNotMatch = !bNotMatch;
			} while (readIndex < originalNum);
			
			m_size = writeIndex;
			
			return originalNum - m_size;
		}
		
		FORCEINLINE void Reserve(SizeType number)
		{
			assert(number >= 0);
			if (number < 0) Private::OnInvalidArrayNum((unsigned long long) (USizeType) number);
			else if (number > m_capacity) ResizeTo(number);
		}
	
	public:
		
		FORCEINLINE void Foreach(const std::function<void(ElementType &)> &callback)
		{
			for (SizeType i = 0; i < m_size; ++i) {
				callback((*this)[i]);
			}
		}
		
		FORCEINLINE void Foreach(const std::function<void(const ElementType &)> &callback) const
		{
			for (SizeType i = 0; i < m_size; ++i) {
				callback((*this)[i]);
			}
		}
	
	public:
		
		FORCEINLINE void swap(TFDynamicArray &other)
		{
			using std::swap;
			swap(m_allocator, other.m_allocator);
			swap(m_size, other.m_size);
			swap(m_capacity, other.m_capacity);
		}
	
	private:
		
		void AllocatorResizeAllocation(SizeType currentArrayNum, SizeType newArrayMax)
		{
			m_allocator.ResizeAllocation(currentArrayNum, newArrayMax, sizeof(ElementType), alignof(ElementType));
		}
		
		SizeType AllocatorCalculateSlackShrink(SizeType currentArrayNum, SizeType newArrayMax)
		{
			return m_allocator.CalculateSlackShrink(
				currentArrayNum, newArrayMax, sizeof(ElementType), alignof(ElementType));
		}
		
		SizeType AllocatorCalculateSlackGrow(SizeType currentArrayNum, SizeType newArrayMax)
		{
			return m_allocator.CalculateSlackGrow(
				currentArrayNum, newArrayMax, sizeof(ElementType), alignof(ElementType));
		}
		
		SizeType AllocatorCalculateSlackReserve(SizeType newArrayMax)
		{
			return m_allocator.CalculateSlackReserve(newArrayMax, sizeof(ElementType), alignof(ElementType));
		}
		
		FORCENOINLINE void ResizeGrow(SizeType oldNum)
		{
			SizeType localArrayNum = m_size;
			
			if (localArrayNum < oldNum) {
				Private::OnInvalidArrayNum((unsigned long long) (USizeType) localArrayNum);
			}
			m_capacity = AllocatorCalculateSlackGrow(localArrayNum, m_capacity);
			AllocatorResizeAllocation(oldNum, m_capacity);
		}
		
		FORCENOINLINE void ResizeShrink()
		{
			const SizeType newArrayMax = AllocatorCalculateSlackShrink(m_size, m_capacity);
			if (newArrayMax != m_capacity) {
				m_capacity = newArrayMax;
				assert(m_capacity >= m_size);
				AllocatorResizeAllocation(m_size, m_capacity);
			}
		}
		
		FORCENOINLINE void ResizeTo(SizeType newMax)
		{
			if (newMax) {
				newMax = AllocatorCalculateSlackReserve(newMax);
			}
			if (newMax != m_capacity) {
				m_capacity = newMax;
				AllocatorResizeAllocation(m_size, m_capacity);
			}
		}
		
		FORCENOINLINE void ResizeForCopy(SizeType newMax, SizeType prevMax)
		{
			if (newMax) {
				newMax = AllocatorCalculateSlackReserve(newMax);
			}
			if (newMax > prevMax) {
				AllocatorResizeAllocation(0, newMax);
				m_capacity = newMax;
			} else {
				m_capacity = prevMax;
			}
		}
		
		void InsertUninitializedImpl(SizeType index)
		{
			CheckInvariants();
			assert((index >= 0) & (index <= m_size));
			
			const auto oldNum = (USizeType) m_size;
			const USizeType newNum = oldNum + (USizeType) 1;
			m_size = (SizeType) newNum;
			
			if (newNum > (USizeType) m_capacity) {
				ResizeGrow((SizeType) oldNum);
			}
			ElementType *data = GetData() + index;
			RelocateConstructItems<ElementType>(data + 1, data, oldNum - index);
		}
		
		template<typename OtherSizeType>
		void InsertUninitializedImpl(SizeType index, OtherSizeType count)
		{
			CheckInvariants();
			assert((count >= 0) & (index >= 0) & (index <= m_size));
			
			SizeType convertedCount = count;
			assert((OtherSizeType) convertedCount == count &&
			       TEXT("Invalid number of elements to add to this array type"));
			
			const auto oldNum = (USizeType) m_size;
			const USizeType newNum = oldNum + (USizeType) count;
			m_size = (SizeType) newNum;
			
			if ((USizeType) count > (USizeType) m_capacity - oldNum) {
				ResizeGrow((SizeType) oldNum);
			}
			ElementType *data = GetData() + index;
			RelocateConstructItems<ElementType>(data + count, data, oldNum - index);
		}
		
		template<typename OtherElementType, typename OtherSizeType>
		void CopyToEmpty(const OtherElementType *otherData, OtherSizeType otherNum, SizeType prevMax)
		{
			auto newNum = (SizeType) otherNum;
			assert((OtherSizeType) newNum == otherNum &&
			       TEXT("Invalid number of elements to add to this array type: %lld"));
			
			m_size = newNum;
			if (otherNum || prevMax) {
				ResizeForCopy(newNum, prevMax);
				ConstructItems<ElementType>(GetData(), otherData, otherNum);
			} else {
				m_capacity = m_allocator.GetInitialCapacity();
			}
		}
		
		template<typename FromArrayType, typename ToArrayType>
		static FORCEINLINE void Move(ToArrayType &toArray, FromArrayType &fromArray, SizeType prevMax)
		{
			
			static_assert(
				std::is_same_v<TFDynamicArray, ToArrayType>,
				"Move is expected to be called with the current array type as the destination"
			);
			
			using FromAllocatorType = typename FromArrayType::AllocatorType;
			using ToAllocatorType = typename ToArrayType::AllocatorType;
			
			toArray.m_allocator.MoveToEmpty(fromArray.m_allocator);
			
			toArray.m_size = (SizeType) fromArray.m_size;
			toArray.m_capacity = (SizeType) fromArray.m_capacity;
			
			if constexpr (sizeof(SizeType) < sizeof(typename FromArrayType::SizeType)) {
				if (toArray.m_size != fromArray.m_size || toArray.m_capacity != fromArray.m_capacity)
					Private::OnInvalidArrayNum((unsigned long long) (USizeType) toArray.m_size);
			}
			
			fromArray.m_size = 0;
			fromArray.m_capacity = fromArray.m_allocator.GetInitialCapacity();
		}
	
	public:
		
		template<typename ElementType, typename SizeType, bool bReverse = false>
		struct TCheckedPointerIterator {
			
			explicit TCheckedPointerIterator(const SizeType &inNum, ElementType *inPtr)
				: ptr(inPtr), currentNum(inNum), initialNum(inNum)
			{
			}
			
			FORCEINLINE ElementType &operator*() const
			{
				if constexpr (bReverse) {
					return *(ptr - 1);
				} else {
					return *ptr;
				}
			}
			
			FORCEINLINE TCheckedPointerIterator &operator++()
			{
				if constexpr (bReverse) {
					--ptr;
				} else {
					++ptr;
				}
				return *this;
			}
			
			FORCEINLINE TCheckedPointerIterator &operator--()
			{
				if constexpr (bReverse) {
					++ptr;
				} else {
					--ptr;
				}
				return *this;
			}
			
			FORCEINLINE bool operator!=(const TCheckedPointerIterator &rhs) const
			{
				assert(currentNum == initialNum && TEXT("Array has changed during ranged-for iteration!"));
				return ptr != rhs.ptr;
			}
		
		private:
			ElementType *ptr;
			const SizeType &currentNum;
			SizeType initialNum;
		};
		
		typedef TCheckedPointerIterator<ElementType, SizeType, false> RangedForIteratorType;
		typedef TCheckedPointerIterator<const ElementType, SizeType, false> RangedForConstIteratorType;
		typedef TCheckedPointerIterator<ElementType, SizeType, true> RangedForReverseIteratorType;
		typedef TCheckedPointerIterator<const ElementType, SizeType, true> RangedForConstReverseIteratorType;
	
	public:
		
		FORCEINLINE RangedForIteratorType begin() { return RangedForIteratorType(m_size, GetData()); }
		
		FORCEINLINE RangedForConstIteratorType begin() const { return RangedForConstIteratorType(m_size, GetData()); }
		
		FORCEINLINE RangedForIteratorType end() { return RangedForIteratorType(m_size, GetData() + GetSize()); }
		
		FORCEINLINE RangedForConstIteratorType end() const
		{
			return RangedForConstIteratorType(
				m_size, GetData() + GetSize());
		}
		
		FORCEINLINE RangedForReverseIteratorType rbegin()
		{
			return RangedForReverseIteratorType(
				m_size, GetData() + GetSize());
		}
		
		FORCEINLINE RangedForConstReverseIteratorType rbegin() const
		{
			return RangedForConstReverseIteratorType(
				m_size, GetData() + GetSize());
		}
		
		FORCEINLINE RangedForReverseIteratorType rend() { return RangedForReverseIteratorType(m_size, GetData()); }
		
		FORCEINLINE RangedForConstReverseIteratorType rend() const
		{
			return RangedForConstReverseIteratorType(
				m_size, GetData());
		}
	
	public:
		
		[[nodiscard]] FORCEINLINE AllocatorType &GetAllocator() { return m_allocator; }
		
		[[nodiscard]] FORCEINLINE const AllocatorType &GetAllocator() const { return m_allocator; }
		
		[[nodiscard]] FORCEINLINE SizeType GetCapacity() const { return m_capacity; }
		
		[[nodiscard]] FORCEINLINE SizeType GetSize() const { return m_size; }
		
		[[nodiscard]] FORCEINLINE ElementType *GetData() { return (ElementType *) m_allocator.GetData(); }
		
		[[nodiscard]] FORCEINLINE const ElementType *GetData() const { return (const ElementType *) m_allocator.GetData(); }
		
		[[nodiscard]] FORCEINLINE bool IsEmpty() const { return m_size == 0; }
	
	private:
		
		AllocatorType m_allocator;
		SizeType m_capacity;
		SizeType m_size;
		
	};
	
	
}

