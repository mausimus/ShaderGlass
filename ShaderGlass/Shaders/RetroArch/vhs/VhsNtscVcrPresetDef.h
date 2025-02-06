/*
ShaderGlass preset vhs / ntsc-vcr imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/vhs/ntsc-vcr.slangp
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
         	ShaderDefs.push_back(NtscShadersMaisterNtscPass1Composite3phaseShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2")
.Param("scale_type", "source")
.Param("scale_x", "4.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(NtscShadersMaisterNtscPass23phaseShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "0.5")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(NtscShadersMaisterNtscStockShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type_x", "source")
.Param("scale_type_y", "viewport"));
         	ShaderDefs.push_back(VhsShadersStaticShaderDef()
.Param("scale_type", "source"));
         	ShaderDefs.push_back(MiscShadersImageAdjustmentShaderDef()
.Param("scale_type", "source"));
            TextureDefs.push_back(VhsResourcesPlayTextureDef()
.Param("name", "play"));
	}
};
}
