/*
ShaderGlass preset crt / crt-hyllian-multipass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-hyllian-multipass.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtHyllianMultipassPresetDef : public PresetDef
{
public:
	CrtCrtHyllianMultipassPresetDef() : PresetDef{}
	{
		Name = "crt-hyllian-multipass";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtHyllianMultipassCrtHyllianPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtHyllianMultipassCrtHyllianPass1ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
