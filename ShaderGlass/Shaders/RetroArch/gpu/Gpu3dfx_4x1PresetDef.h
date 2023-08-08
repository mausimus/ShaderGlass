/*
ShaderGlass preset gpu / 3dfx_4x1 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/gpu/3dfx_4x1.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Gpu3dfx_4x1PresetDef : public PresetDef
{
public:
	Gpu3dfx_4x1PresetDef() : PresetDef{}
	{
		Name = "3dfx_4x1";
		Category = "gpu";
	}

	virtual void Build() {
         	ShaderDefs.push_back(GpuShaders3dfx3dfx_pass_0ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(GpuShaders3dfx3dfx_pass_1ShaderDef()
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(GpuShaders3dfx3dfx_pass_1ShaderDef()
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(GpuShaders3dfx3dfx_pass_1ShaderDef()
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(GpuShaders3dfx3dfx_pass_1ShaderDef()
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(GpuShaders3dfx3dfx_pass_2ShaderDef());
	}
};
}
