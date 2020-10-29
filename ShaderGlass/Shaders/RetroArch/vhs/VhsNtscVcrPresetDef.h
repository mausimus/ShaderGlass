/*
ShaderGlass preset vhs / ntsc-vcr imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/vhs/ntsc-vcr.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class VhsNtscVcrPresetDef : public PresetDef
{
public:
	VhsNtscVcrPresetDef() : PresetDef{}
	{
		Name = "ntsc-vcr";
		Category = "vhs";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersNtscPass1Composite3phaseShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2")
.Param("scale_type", "source")
.Param("scale_x", "4.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(NtscShadersNtscPass23phaseShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "0.5")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(NtscShadersNtscStockShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type_x", "source")
.Param("scale_type_y", "viewport"));
         	ShaderDefs.push_back(VhsShadersStaticShaderDef()
.Param("scale_type", "source"));
         	ShaderDefs.push_back(MiscImageAdjustmentShaderDef()
.Param("scale", "1.0"));
            TextureDefs.push_back(VhsResourcesPlayTextureDef()
.Param("name", "play"));
	}
};
}
