/*
ShaderGlass preset handheld / retro-v2 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/retro-v2.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldRetroV2PresetDef : public PresetDef
{
public:
	HandheldRetroV2PresetDef() : PresetDef{}
	{
		Name = "retro-v2";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersRetroV2ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
