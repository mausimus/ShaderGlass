/*
ShaderGlass preset misc / print-resolution imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/print-resolution.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscPrintResolutionPresetDef : public PresetDef
{
public:
	MiscPrintResolutionPresetDef() : PresetDef{}
	{
		Name = "print-resolution";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersPrintResolutionPrintResolutionGenerateAndCacheShaderDef()
.Param("scale_type", "source"));
         	ShaderDefs.push_back(MiscShadersPrintResolutionPrintResolutionApplyShaderDef()
.Param("scale_type", "source"));
	}
};
}
