#pragma once
#include "BlackPearl/Window.h"
#include "BlackPearl/RHI/RHIShader.h"

namespace BlackPearl{

	EShaderPlatform GMaxRHIShaderPlatform = SP_PCD3D_SM5;

	/** The maximum feature level supported on this machine */
	ERHIFeatureLevel::Type GMaxRHIFeatureLevel = ERHIFeatureLevel::SM5;

	class Window;
	class DynamicModule;

	enum class RHIType {
		D3D12,
		OpenGL,
		Vulkan
	};

	//void    RHIInitLogger();
	Window* RHIInitWindow();
	void RHIEngineExit();

	class DynamicRHI
	{
	public:
		enum class Type {
			D3D12,
			OpenGL,
			Vulkan
		};
		
		virtual Window* InitWindow() = 0;
		virtual void InitRHI() = 0;

		//virtual void InitLogger() = 0;
		//virtual std::shared_ptr<Logger> GetCoreLogger() = 0;
		virtual void InitMathLib() {};
		virtual void EngineExit() {};

		static Type g_RHIType;
	};

	//DynamicRHI::Type g_RHIType;
	void DynamicRHIInit(DynamicRHI::Type rhiType);
}