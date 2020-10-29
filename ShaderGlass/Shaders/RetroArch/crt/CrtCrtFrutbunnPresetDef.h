/*
ShaderGlass preset crt / crt-frutbunn imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-frutbunn.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtFrutbunnPresetDef : public PresetDef
{
public:
	CrtCrtFrutbunnPresetDef() : PresetDef{}
	{
		Name = "crt-frutbunn";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtFrutbunnShaderDef());
	}
};
}
