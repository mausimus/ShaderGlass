/*
ShaderGlass preset scanlines / res-independent-scalines imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/scanlines/res-independent-scalines.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ScanlinesResIndependentScalinesPresetDef : public PresetDef
{
public:
	ScanlinesResIndependentScalinesPresetDef() : PresetDef{}
	{
		Name = "res-independent-scalines";
		Category = "scanlines";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScanlinesShadersResIndependentScanlinesShaderDef());
	}
};
}
