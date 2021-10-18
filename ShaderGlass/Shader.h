#include "pch.h"

#pragma once

#include "ShaderDef.h"

constexpr auto PUSH_BUFFER = -1;
constexpr auto UBO_BUFFER  = 0;

struct float4
{
    float x, y, z, w;
};

union float4x4
{
    float4x4() { }

    float  m[4][4];
    float4 cols[4];

    inline float4 row(int i)
    {
        return {m[0][i], m[1][i], m[2][i], m[3][i]};
    }
};

class Shader
{
public:
    ShaderDef&                         m_shaderDef;
    winrt::com_ptr<ID3D11VertexShader> m_vertexShader;
    winrt::com_ptr<ID3D11PixelShader>  m_pixelShader;
    std::string                        m_alias {};
    float                              m_scaleX {1.0f};
    float                              m_scaleY {1.0f};
    bool                               m_scaleViewportX {false};
    bool                               m_scaleViewportY {false};
    bool                               m_scaleAbsoluteX {false};
    bool                               m_scaleAbsoluteY {false};
    bool                               m_formatSRGB {false};
    bool                               m_formatFloat {false};
    bool                               m_filterLinear {false};
    int                                m_frameCountMod {0};

    Shader(ShaderDef& shaderDef);
    Shader(Shader&& shader);
    ~Shader();

    void                      Create(winrt::com_ptr<ID3D11Device> d3dDevice);
    void                      Compile();
    std::vector<ShaderParam*> Params();
    void                      FillParams(int buffer, void* data);
    void                      SetParam(ShaderParam* p, void* v);
    void                      SetParam(std::string name, void* p);
    size_t                    BufferSize(int buffer);

private:
    std::unique_ptr<int[]>   m_pushBuffer;
    std::unique_ptr<int[]>   m_uboBuffer;
    winrt::com_ptr<ID3DBlob> m_vertexBlob;
    winrt::com_ptr<ID3DBlob> m_pixelBlob;

    bool IsTrue(const std::string& presetParam);
    bool Get(const std::string& presetParam, std::string& value);
};