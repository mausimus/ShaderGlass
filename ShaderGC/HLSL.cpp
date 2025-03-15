#include "pch.h"

#include "HLSL.h"

#include <stdexcept>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

std::vector<uint8_t> HLSL::CompileHLSL(const char* source, int size, const char* profile, std::ostream& log, bool& warn)
{
    //std::cout << "CompileHLSL...";

    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob  = nullptr;
    UINT      flags      = D3DCOMPILE_OPTIMIZATION_LEVEL3;
    HRESULT   hr         = D3DCompile(source, size, NULL, NULL, NULL, "main", profile, flags, 0, &shaderBlob, &errorBlob);

    if(FAILED(hr))
    {
        std::ostringstream msg;
        msg << "FXC Compilation Error" << std::endl;
        if(errorBlob)
        {
            msg << (char*)errorBlob->GetBufferPointer() << std::endl;
            errorBlob->Release();
        }

        if(shaderBlob)
            shaderBlob->Release();

        throw std::runtime_error(msg.str());
    }

    auto data  = shaderBlob->GetBufferPointer();
    auto bsize = shaderBlob->GetBufferSize();

    std::vector<uint8_t> bin;
    bin.resize(bsize);
    memcpy(bin.data(), data, bsize);
    shaderBlob->Release();

    //std::cout << "OK" << std::endl;

    return bin;
}
