/*
ShaderGlass preset reshade / blendoverlay imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/reshade/blendoverlay.slangp
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
            OverrideParam("LUTHeight", (float)4.000000);
            OverrideParam("LUTWidth", (float)6.000000);
	}
};
}
