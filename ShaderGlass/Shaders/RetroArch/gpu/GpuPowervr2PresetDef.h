/*
ShaderGlass preset gpu / powervr2 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/gpu/powervr2.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class GpuPowervr2PresetDef : public PresetDef
{
public:
	GpuPowervr2PresetDef() : PresetDef{}
	{
		Name = "powervr2";
		Category = "gpu";
	}

	virtual void Build() {
         	ShaderDefs.push_back(GpuShadersPowervr2ShaderDef());
	}
};
}
