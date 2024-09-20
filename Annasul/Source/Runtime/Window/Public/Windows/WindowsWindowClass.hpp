#pragma once

#include "GenericWindowClass.hpp"

namespace Annasul
{
    class FWindowsWindowClass final : public FGenericWindowClass
    {
    public:
		
		FORCEINLINE FWindowsWindowClass() : m_atom(0) {}
		FORCEINLINE ~FWindowsWindowClass() override { FWindowsWindowClass::Unregister(); }
		
    public:
	    
	    void Register(const FWindowClassDesc &desc) override;
	    
	    void Unregister() override;
	    
	    bool IsRegistered() const override;
		
    public:
		
		bool CheckAtom();
	    bool CheckAtomNoLog();
		
		FORCEINLINE FWindowsPlatformTypes::ATOM GetAtom() const { return m_atom; }
	    
    private:
		
		FWindowsPlatformTypes::ATOM m_atom;
		
    };
	
	typedef FWindowsWindowClass FWindowClass;
	
}
