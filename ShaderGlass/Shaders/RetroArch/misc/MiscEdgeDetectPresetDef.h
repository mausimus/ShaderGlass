/*
ShaderGlass preset misc / edge-detect imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/misc/edge-detect.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscEdgeDetectPresetDef : public PresetDef
{
public:
	MiscEdgeDetectPresetDef() : PresetDef{}
	{
		Name = "edge-detect";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersEdgeDetectShaderDef()
.Param("scale_type", "source"));
	}
};
}
