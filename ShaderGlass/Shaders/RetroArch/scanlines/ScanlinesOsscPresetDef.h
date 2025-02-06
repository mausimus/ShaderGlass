/*
ShaderGlass preset scanlines / ossc imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/scanlines/ossc.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ScanlinesOsscPresetDef : public PresetDef
{
public:
	ScanlinesOsscPresetDef() : PresetDef{}
	{
		Name = "ossc";
		Category = "scanlines";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScanlinesShadersOsscShaderDef()
.Param("scale_type", "viewport"));
	}
};
}
