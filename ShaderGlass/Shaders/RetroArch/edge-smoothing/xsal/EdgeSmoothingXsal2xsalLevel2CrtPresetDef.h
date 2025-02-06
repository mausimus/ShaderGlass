/*
ShaderGlass preset edge-smoothing-xsal / 2xsal-level2-crt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/xsal/2xsal-level2-crt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingXsal2xsalLevel2CrtPresetDef : public PresetDef
{
public:
	EdgeSmoothingXsal2xsalLevel2CrtPresetDef() : PresetDef{}
	{
		Name = "2xsal-level2-crt";
		Category = "edge-smoothing-xsal";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingXsalShaders2xsalLevel2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersDotmaskShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
