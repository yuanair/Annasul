#pragma once

#include "Platform.hpp"
#include "Container/StringView.hpp"
#include <type_traits>
#include <map>

#define A_CLASS_BODY(className) \
public:                         \
static_assert(std::is_class_v<className>, "className must be a class type or struct type"); \
FORCEINLINE static auto& GetStaticClass() { return FReflect::Get().Register<className>(TEXT(#className)); }

#define A_METHOD_REGISTER(className, funcName) \
public:                                        \
struct __Reflecter ## funcName { __Reflecter ## funcName() { GetStaticClass().RegisterMethod(TEXT(#funcName), &className::funcName); } } \
static const __Reflecter ## funcName __RegisterHelper_##funcName;

namespace Annasul
{
	
	
	class FReflect;
	
	class FClass;
	
	class FMethod;
	
	template<typename InType, typename OutType>
	using EnableIfClass = std::enable_if_t<std::is_class_v<InType>, OutType>;
	
	class FReflect final {
	public:
	
	public:
		
		static FReflect &Get();
	
	public:
		
		template<typename InSizeType, InSizeType InSize>
		FORCEINLINE static consteval FStringView ParseMethodName(const TCHAR (&s)[InSize])
		{
			FStringView prettyFunction(s);
			auto bracket = prettyFunction.RFind(TEXT('('));
			auto space = prettyFunction.RFind(TEXT(' '), bracket) + 1;
			return prettyFunction.SubView(space, bracket - space);
		}
		
		template<typename InType>
		FORCEINLINE EnableIfClass<InType, FClass &> Register(const FStringView &name);
	
	private:
		
		template<typename InType>
		FORCEINLINE EnableIfClass<InType, FClass &> RegisterClass(const FStringView &name);
	
	private:
		
		FORCEINLINE FReflect() = default;
		
		FORCEINLINE ~FReflect() = default;
	
	private:
		
		std::map<FStringView, FClass> m_classes;
		
	};
	
	class FClass final {
		
		friend class FReflect;
	
	public:
		
		FORCEINLINE ~FClass() = default;
	
	public:
		
		template<typename InClassType, typename InReturnType, typename... InArgumentsType>
		FORCEINLINE FMethod &
		RegisterMethod(const FStringView &methodName, InReturnType(InClassType::*methodPtr)(InArgumentsType...));
	
	public:
		
		[[nodiscard]] FORCEINLINE const FStringView &GetName() const { return m_name; }
	
	private:
		
		FORCEINLINE explicit FClass(FStringView name) : m_name(name) {}
	
	private:
		
		FStringView m_name;
		
		std::map<FStringView, FMethod> m_methods;
		
	};
	
	class FMethod final {
		
		friend class FClass;
	
	public:
		
		FORCEINLINE FMethod() : m_name(TEXT("<unknown method>")) {}
		
		FORCEINLINE ~FMethod() = default;
	
	public:
		
		[[nodiscard]] FORCEINLINE const FStringView &GetName() const { return m_name; }
	
	private:
		
		FORCEINLINE explicit FMethod(FStringView name, void *ptr) : m_name(name), m_ptr(ptr) {}
	
	private:
		
		FStringView m_name;
		
		void *m_ptr = nullptr;
		
	};
	
	template<typename InType>
	EnableIfClass<InType, FClass &> FReflect::Register(const FStringView &name)
	{
		static FClass classInstance{name};
		return classInstance;
	}
	
	template<typename InType>
	EnableIfClass<InType, FClass &> FReflect::RegisterClass(const FStringView &name)
	{
		return m_classes[name] = FClass{name};
	}
	
	template<typename InClassType, typename InReturnType, typename... InArgumentsType>
	FMethod &
	FClass::RegisterMethod(const FStringView &methodName, InReturnType(InClassType::*methodPtr)(InArgumentsType...))
	{
		return m_methods[methodName] = FMethod{methodName,
		                                       (void *) (InReturnType (InClassType::*)(
			                                       InArgumentsType...)) (methodPtr)};
	}
	
}
