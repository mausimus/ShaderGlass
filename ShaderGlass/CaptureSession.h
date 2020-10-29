#pragma once

#include "ShaderGlass.h"

class CaptureSession
{
public:
	CaptureSession(
		winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice const& device,
		winrt::Windows::Graphics::Capture::GraphicsCaptureItem const& item,
		winrt::Windows::Graphics::DirectX::DirectXPixelFormat pixelFormat,
		ShaderGlass& shaderGlass
		);

	void OnFrameArrived(
		winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool const& sender,
		winrt::Windows::Foundation::IInspectable const& args);

	void Stop();

private:
	winrt::Windows::Graphics::Capture::GraphicsCaptureItem m_item{ nullptr };
	winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool m_framePool{ nullptr };
	winrt::Windows::Graphics::Capture::GraphicsCaptureSession m_session{ nullptr };
	winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice m_device{ nullptr };

	ShaderGlass& m_shaderGlass;
};
