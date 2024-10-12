#include <Container/Pair.hpp>
#include <Window.hpp>
#include <GenericRenderEngineFactory.hpp>
#include <Vector.hpp>
#include <GenericSocket.hpp>
#include <Launch.hpp>
#include <GenericDebug.hpp>
#include <World.hpp>

#include <memory>
#include <string>

#include <chrono>

using namespace Annasul;

class AMyWindow : public AWindow {
public:
	
	AMyWindow() = default;
	
	~AMyWindow() override = default;

protected:
	
	void OnBeginPlay() override
	{
		AWindow::OnBeginPlay();
		DragAcceptFiles(true);
		Show();
	}
	
	void OnTick(double deltaTime) override
	{
		auto nowTime = std::chrono::system_clock::now();
		auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime.time_since_epoch());
		
		
		auto position = TFVector<double, 2>(
			std::sin(seconds.count() / 100.0) * 10 + 400, std::cos(seconds.count() / 100.0) * 100 + 400
		);
		auto size = TFVector<double, 2>(
			std::sin(seconds.count() / 100.0) * 10 + 400, std::cos(seconds.count() / 100.0) * 100 + 400
		);
		SetPositionAndSize(position.Cast<int32>(), size.Cast<int32>());
		
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

class FMainLevel : public FLevel {
public:
	
	FMainLevel()
		: socket(), windowClass(), window()
	{
		m_actors.Emplace(&windowClass);
		m_actors.Emplace(&window);
		window.SetWindowClass(&windowClass);
		std::unique_ptr<FGenericRenderEngine> renderEngine
			{
				FRenderEngineFactory::Create(ERenderEngineType::DirectX2D01)
			};
		
		auto vector = MakeVector(0, 0);
		vector.x() = 10;
		
	}
	
	~FMainLevel() override = default;

private:
	
	FSocket socket;
	AWindowClass windowClass;
	AMyWindow window;
	
};

void AnnasulMain()
{
	FWorld::Get().AddToWorld(new FMainLevel());
}
