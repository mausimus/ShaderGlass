/*
ShaderGlass preset xsal / 4xsal-level2-crt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xsal/4xsal-level2-crt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Xsal4xsalLevel2CrtPresetDef : public PresetDef
{
public:
	Xsal4xsalLevel2CrtPresetDef() : PresetDef{}
	{
		Name = "4xsal-level2-crt";
		Category = "xsal";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XsalShaders2xsalLevel2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "4.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XsalShaders2xsalLevel2Pass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersDotmaskShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
