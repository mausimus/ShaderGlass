/*
ShaderGlass preset gpu-shaders-3dfx-old / 3dfx_4x1 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/gpu/shaders/3dfx/old/3dfx_4x1.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class GpuShaders3dfxOld3dfx_4x1PresetDef : public PresetDef
{
public:
	GpuShaders3dfxOld3dfx_4x1PresetDef() : PresetDef{}
	{
		Name = "3dfx_4x1";
		Category = "gpu-shaders-3dfx-old";
	}

	virtual void Build() {
         	ShaderDefs.push_back(GpuShaders3dfxOld3dfx_pass_0ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(GpuShaders3dfxOld3dfx_pass_1ShaderDef()
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(GpuShaders3dfxOld3dfx_pass_2ShaderDef()
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(GpuShaders3dfxOld3dfx_pass_2ShaderDef()
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(GpuShaders3dfxOld3dfx_pass_2ShaderDef()
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(GpuShaders3dfxOld3dfx_pass_2ShaderDef());
	}
};
}
