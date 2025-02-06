/*
ShaderGlass preset presets-crt-beam-simulator / crt-beam-simulator-crt-fast-bilateral-super-xbr imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/crt-beam-simulator/crt-beam-simulator-crt-fast-bilateral-super-xbr.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsCrtBeamSimulatorCrtBeamSimulatorCrtFastBilateralSuperXbrPresetDef : public PresetDef
{
public:
	PresetsCrtBeamSimulatorCrtBeamSimulatorCrtFastBilateralSuperXbrPresetDef() : PresetDef{}
	{
		Name = "crt-beam-simulator-crt-fast-bilateral-super-xbr";
		Category = "presets-crt-beam-simulator";
	}

	virtual void Build() {
         	ShaderDefs.push_back(SubframeBfiShadersCrtBeamSimulatorShaderDef()
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(DenoisersShadersFastBilateralShaderDef()
.Param("alias", "XbrSource")
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersSuperXbrSuperXbrPass0ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersSuperXbrSuperXbrPass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "2.000000")
.Param("scale_y", "2.000000")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersSuperXbrSuperXbrPass2ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(InterpolationShadersJinc2ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "0.500000")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(CrtShadersHyllianCrtHyllianPass0ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersHyllianCrtHyllianPass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false"));
	}
};
}
