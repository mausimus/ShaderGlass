/*
ShaderGlass preset cel / MMJ_Cel_Shader imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/cel/MMJ_Cel_Shader.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CelMMJ_Cel_ShaderPresetDef : public PresetDef
{
public:
	CelMMJ_Cel_ShaderPresetDef() : PresetDef{}
	{
		Name = "MMJ_Cel_Shader";
		Category = "cel";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CelShadersMMJ_CelMMJ_Cel_ShaderShaderDef());
	}
};
}
