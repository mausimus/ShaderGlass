#include "HLSL.h"

#ifdef USE_D3DC

#include <stdexcept>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

std::vector<uint8_t> HLSL::CompileHLSL(const char* source, int size, const char* profile)
{
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob  = nullptr;
    UINT      flags      = D3DCOMPILE_OPTIMIZATION_LEVEL3;
    HRESULT   hr         = D3DCompile(source, size, NULL, NULL, NULL, "main", profile, flags, 0, &shaderBlob, &errorBlob);

    if(FAILED(hr))
    {
        if(errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }

        if(shaderBlob)
            shaderBlob->Release();

        throw std::runtime_error("FXC compilation error");
    }

    auto data = shaderBlob->GetBufferPointer();
    auto bsize = shaderBlob->GetBufferSize();

    std::vector<uint8_t> bin;
    bin.resize(bsize);
    memcpy(bin.data(), data, bsize);
    shaderBlob->Release();
    return bin;
}

#endif
