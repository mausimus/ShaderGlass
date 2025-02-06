/*
ShaderGlass preset crt / zfast-crt-hdmask imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/zfast-crt-hdmask.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtZfastCrtHdmaskPresetDef : public PresetDef
{
public:
	CrtZfastCrtHdmaskPresetDef() : PresetDef{}
	{
		Name = "zfast-crt-hdmask";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersZfast_crtZfast_crt_coarsemaskShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
