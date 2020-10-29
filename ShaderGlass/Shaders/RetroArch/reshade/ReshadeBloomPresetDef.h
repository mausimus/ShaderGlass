/*
ShaderGlass preset reshade / bloom imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/reshade/bloom.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ReshadeBloomPresetDef : public PresetDef
{
public:
	ReshadeBloomPresetDef() : PresetDef{}
	{
		Name = "bloom";
		Category = "reshade";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ReshadeShadersBloomBloomPass0ShaderDef()
.Param("alias", "SamplerBloom1")
.Param("float_framebuffer", "true"));
         	ShaderDefs.push_back(ReshadeShadersBloomBloomPass1ShaderDef()
.Param("alias", "SamplerBloom2")
.Param("float_framebuffer", "true"));
         	ShaderDefs.push_back(ReshadeShadersBloomBloomPass2ShaderDef()
.Param("alias", "SamplerBloom3")
.Param("float_framebuffer", "true")
.Param("scale", "0.5")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(ReshadeShadersBloomBloomPass3ShaderDef()
.Param("alias", "SamplerBloom4")
.Param("float_framebuffer", "true")
.Param("scale", "0.25")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(ReshadeShadersBloomBloomPass4ShaderDef()
.Param("alias", "SamplerBloom5")
.Param("float_framebuffer", "true")
.Param("scale", "0.125")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(ReshadeShadersBloomLensFlarePass0ShaderDef()
.Param("alias", "LensFlare1")
.Param("float_framebuffer", "true"));
         	ShaderDefs.push_back(ReshadeShadersBloomLensFlarePass1ShaderDef()
.Param("alias", "LensFlare2")
.Param("float_framebuffer", "true")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(ReshadeShadersBloomLensFlarePass2ShaderDef()
.Param("alias", "LensFlare3")
.Param("float_framebuffer", "true")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(ReshadeShadersBloomLightingCombineShaderDef()
.Param("filter_linear", "true")
.Param("scale", "2.0")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(ReshadeShadersBloomLensDBTextureDef()
.Param("name", "Dirt"));
            TextureDefs.push_back(ReshadeShadersBloomLensSpriteTextureDef()
.Param("name", "Sprite"));
	}
};
}
