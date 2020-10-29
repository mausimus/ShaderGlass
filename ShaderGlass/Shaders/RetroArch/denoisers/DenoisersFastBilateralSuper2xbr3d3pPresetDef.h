/*
ShaderGlass preset denoisers / fast-bilateral-super-2xbr-3d-3p imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/denoisers/fast-bilateral-super-2xbr-3d-3p.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DenoisersFastBilateralSuper2xbr3d3pPresetDef : public PresetDef
{
public:
	DenoisersFastBilateralSuper2xbr3d3pPresetDef() : PresetDef{}
	{
		Name = "fast-bilateral-super-2xbr-3d-3p";
		Category = "denoisers";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DenoisersShadersFastBilateral3dShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper2xbr3dPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper2xbr3dPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper2xbr3dPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(WindowedShadersJinc2ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
