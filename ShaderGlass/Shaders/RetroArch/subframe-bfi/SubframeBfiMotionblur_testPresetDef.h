/*
ShaderGlass preset subframe-bfi / motionblur_test imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/subframe-bfi/motionblur_test.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class SubframeBfiMotionblur_testPresetDef : public PresetDef
{
public:
	SubframeBfiMotionblur_testPresetDef() : PresetDef{}
	{
		Name = "motionblur_test";
		Category = "subframe-bfi";
	}

	virtual void Build() {
         	ShaderDefs.push_back(SubframeBfiShadersMotionblur_testShadersMotionblur_testShaderDef());
            TextureDefs.push_back(SubframeBfiShadersMotionblur_testResources60TextureDef()
.Param("name", "base"));
            TextureDefs.push_back(SubframeBfiShadersMotionblur_testResources120TextureDef()
.Param("name", "twoSub"));
            TextureDefs.push_back(SubframeBfiShadersMotionblur_testResources180TextureDef()
.Param("name", "threeSub"));
            TextureDefs.push_back(SubframeBfiShadersMotionblur_testResources240TextureDef()
.Param("name", "fourSub"));
	}
};
}
