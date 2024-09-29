#include <Container/Pair.hpp>
#include <GenericWindow.hpp>
#include <GenericRenderEngineFactory.hpp>
#include <Vector.hpp>
#include <GenericSocket.hpp>
#include <Launch.hpp>

#include <memory>
#include <string>
#include "GenericDebug.hpp"

#include <chrono>

using namespace Annasul;

class FMyWindow : public FWindow
{
public:
	
	FMyWindow() = default;
	
	~FMyWindow() override = default;

public:
	
	void Create(const FWindowsWindowClass &windowClass)
	{
		FWindow::Create(windowClass, {TEXT("Test Window"), 400, 300, false});
		DragAcceptFiles(true);
	}

public:
	
	void OnChar(Annasul::uint64 code) override
	{
		auto buffer = TEXT("OnChar: ") + std::to_wstring(code);
		FDebug::Get().Log(EDebugLevel::Info, buffer);
	}
	
	void OnString(Annasul::FStringView str) override
	{
		auto buffer = TEXT("OnString: ") + std::wstring(str.GetData(), str.GetSize());
		FDebug::Get().Log(EDebugLevel::Info, buffer);
	}
	
	void OnDropFile(Annasul::FStringView file) override
	{
		auto buffer = TEXT("file: ") + std::wstring(file.GetData(), file.GetSize());
		FDebug::Get().Log(EDebugLevel::Info, buffer);
	}
	
	void OnDestroy() override
	{
		Quit(0);
	}
	
};

class FMyProgram
{
public:
	
	FMyProgram()
		: socket(), windowClass(), window()
	{
		windowClass.Register({TEXT("Test Window")});
		window.Create(windowClass);
		
		std::unique_ptr<FGenericRenderEngine> renderEngine
			{
				FRenderEngineFactory::Create(ERenderEngineType::DirectX2D01)
			};
		
		auto vector = MakeVector(0, 0);
		vector.x() = 10;
		
		window.Show();
	}
	
	~FMyProgram()
	{
	}

public:
	
	void Tick(double deltaTime)
	{
		auto nowTime = std::chrono::system_clock::now();
		auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime.time_since_epoch());

//		auto position = TFVector<double, 2>(
//			std::sin(seconds.count() / 100.0) * 10 + 400, std::cos(seconds.count() / 100.0) * 100 + 400
//		);
//		auto size = TFVector<double, 2>(
//			std::sin(seconds.count() / 100.0) * 10 + 400, std::cos(seconds.count() / 100.0) * 100 + 400
//		);
//		window.SetPositionAndSize(position.Cast<int32>(), size.Cast<int32>());
//
	}

private:
	
	FSocket socket;
	FWindowClass windowClass;
	FMyWindow window;
	
};
