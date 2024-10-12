
#pragma once

#include "GenericWindow.hpp"

namespace Annasul
{
	
	class FLinuxWindow : public FGenericWindow {
	public:
		
		void Create(const FWindowClass &windowClass, const FWindowDesc &desc) final;
		
		void DragAcceptFiles(bool accept) final;
		
		void SetDarkMode(bool darkMode) final;
		
		void Show() final;
		
		void Hide() final;
		
		void Close() final;
		
		void Destroy() final;
		
		[[nodiscard]] bool IsOpen() const final;
		
		void SetMinSize(const FVector2i &size) final;
		
		[[nodiscard]] FVector2i GetMinSize() const final;
		
		void SetMaxSize(const FVector2i &size) final;
		
		[[nodiscard]] FVector2i GetMaxSize() const final;
		
		void SetInputPoint(const FVector2i &point) final;
		
		[[nodiscard]] FVector2i GetInputPoint() const final;
		
		void SetPosition(const FVector2i &position) final;
		
		[[nodiscard]] FVector2i GetPosition() const final;
		
		void SetSize(const FVector2i &size) final;
		
		[[nodiscard]] FVector2i GetSize() const final;
		
		void SetPositionAndSize(const FVector2i &position, const FVector2i &size) final;
		
		[[nodiscard]] FVector4i GetPositionAndSize() const final;
		
	};
	
	typedef FLinuxWindow FWindow;
	
}
