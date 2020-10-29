/*
ShaderGlass preset reshade / blendoverlay imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/reshade/blendoverlay.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ReshadeBlendoverlayPresetDef : public PresetDef
{
public:
	ReshadeBlendoverlayPresetDef() : PresetDef{}
	{
		Name = "blendoverlay";
		Category = "reshade";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ReshadeShadersBlendoverlayBlendoverlayShaderDef());
            TextureDefs.push_back(ReshadeShadersBlendoverlayGrayscale_slotmaskTextureDef()
.Param("name", "overlay"));
	}
};
}
