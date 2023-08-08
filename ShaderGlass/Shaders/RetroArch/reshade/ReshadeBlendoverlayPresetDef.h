/*
ShaderGlass preset reshade / blendoverlay imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/reshade/blendoverlay.slangp
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
         	ShaderDefs.push_back(ReshadeShadersBlendoverlayBlendoverlayShaderDef()
.Param("scale_type", "viewport"));
            TextureDefs.push_back(ReshadeShadersBlendoverlayGrayscale_slotmaskTextureDef()
.Param("name", "overlay"));
	}
};
}
