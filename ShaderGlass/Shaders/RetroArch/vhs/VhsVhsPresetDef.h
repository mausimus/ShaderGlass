/*
ShaderGlass preset vhs / vhs imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/vhs/vhs.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class VhsVhsPresetDef : public PresetDef
{
public:
	VhsVhsPresetDef() : PresetDef{}
	{
		Name = "vhs";
		Category = "vhs";
	}

	virtual void Build() {
         	ShaderDefs.push_back(VhsShadersVhsShaderDef()
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(VhsShadersRewindShaderDef()
.Param("filter_linear", "true"));
            TextureDefs.push_back(VhsResourcesRewTextureDef()
.Param("name", "rew"));
            TextureDefs.push_back(VhsResourcesPlayTextureDef()
.Param("name", "play"));
	}
};
}
