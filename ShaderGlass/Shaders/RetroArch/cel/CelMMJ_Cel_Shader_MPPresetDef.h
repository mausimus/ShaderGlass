/*
ShaderGlass preset cel / MMJ_Cel_Shader_MP imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/cel/MMJ_Cel_Shader_MP.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CelMMJ_Cel_Shader_MPPresetDef : public PresetDef
{
public:
	CelMMJ_Cel_Shader_MPPresetDef() : PresetDef{}
	{
		Name = "MMJ_Cel_Shader_MP";
		Category = "cel";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "MMJ_MP_refpass"));
         	ShaderDefs.push_back(CelShadersMMJ_CelMMJ_Cel_Shader_MP_Blur_HShaderDef()
.Param("alias", "MMJ_BlurPass_H"));
         	ShaderDefs.push_back(CelShadersMMJ_CelMMJ_Cel_Shader_MP_Blur_VShaderDef()
.Param("alias", "MMJ_BlurPass_V"));
         	ShaderDefs.push_back(CelShadersMMJ_CelMMJ_Cel_Shader_MP_OutlineShaderDef()
.Param("alias", "MMJ_OutlinePass"));
         	ShaderDefs.push_back(CelShadersMMJ_CelMMJ_Cel_Shader_MP_ColorShaderDef()
.Param("alias", "MMJ_ColorPass"));
	}
};
}
