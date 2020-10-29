/*
ShaderGlass preset dithering / mdapt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/dithering/mdapt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DitheringMdaptPresetDef : public PresetDef
{
public:
	DitheringMdaptPresetDef() : PresetDef{}
	{
		Name = "mdapt";
		Category = "dithering";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DitheringShadersMdaptPassesMdaptPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DitheringShadersMdaptPassesMdaptPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DitheringShadersMdaptPassesMdaptPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DitheringShadersMdaptPassesMdaptPass3ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DitheringShadersMdaptPassesMdaptPass4ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
