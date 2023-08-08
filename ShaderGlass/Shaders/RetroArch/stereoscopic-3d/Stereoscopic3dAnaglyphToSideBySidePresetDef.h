/*
ShaderGlass preset stereoscopic-3d / anaglyph-to-side-by-side imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/stereoscopic-3d/anaglyph-to-side-by-side.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Stereoscopic3dAnaglyphToSideBySidePresetDef : public PresetDef
{
public:
	Stereoscopic3dAnaglyphToSideBySidePresetDef() : PresetDef{}
	{
		Name = "anaglyph-to-side-by-side";
		Category = "stereoscopic-3d";
	}

	virtual void Build() {
         	ShaderDefs.push_back(Stereoscopic3dShadersAnaglyphToSideBySideShaderDef()
.Param("scale_type", "source"));
	}
};
}
