/*
ShaderGlass preset nedi / fast-bilateral-nedi imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/nedi/fast-bilateral-nedi.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class NediFastBilateralNediPresetDef : public PresetDef
{
public:
	NediFastBilateralNediPresetDef() : PresetDef{}
	{
		Name = "fast-bilateral-nedi";
		Category = "nedi";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DenoisersShadersFastBilateralShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(NediShadersNediPass0ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "2.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(NediShadersNediPass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "2.000000"));
         	ShaderDefs.push_back(NediShadersNediPass2ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(NediShadersNediJincShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false"));
	}
};
}
