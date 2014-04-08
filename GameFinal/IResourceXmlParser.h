#ifndef __RESOURCE_XML_PARSER_INTERFACE_H__
#define __RESOURCE_XML_PARSER_INTERFACE_H__

#include "IReferenceCounted.h"
#include "IPipeline.h"
#include "IRenderState.h"
#include "IMaterial.h"

class IResourceXmlParser : public IReferenceCounted
{
public:
	
	/* �ýṹ�������洢����һ��shader��������� */
	struct SShaderCreateParams
	{
		E_SHADER_TYPE					Type;
		std::string						FileName;
		std::string						FunctionName;
	};

	struct SRenderStateCreateParams
	{
		E_RENDER_STATE					StateType;
		u32								DwordValue;
		f32								FloatValue;
	};

	/* �ýṹ�������洢����pipeline��������� */
	struct SPipelineCreateParams
	{
		std::string								Name;
		std::vector<SShaderCreateParams>		Shaders;
		std::vector<SShaderAutoVariable>		ShaderAutoVariables;
		std::vector<SInputLayoutElement>		InputLayoutElements;
		E_PRIMITIVE_TYPE						PrimitiveType;
		std::vector<SRenderStateCreateParams>	RenderStates;
		std::map<std::string, SSamplerDesc>		SamplerDescs;

		SPipelineCreateParams()
		{
			//std::fill(Shaders, Shaders + EST_SHADER_COUNT, nullptr);
			PrimitiveType = EPT_TRIANGLELIST;
		}
	};

	struct SMaterialTextureParam
	{
		std::string								Name;
		u32										Index;
	};

	struct SMaterialCreateParams
	{
		std::string								Name;
		SMaterial::Material						MaterialColors;
		std::vector<std::string>				PipelineNames;
		std::vector<SMaterialTextureParam>		TextureParams;
	};

public:

	virtual bool parsePipelineFile(const std::string& filepath, std::vector<SPipelineCreateParams>& createParamsArray) const = 0;

	virtual bool parseMaterialFile(const std::string& filepath, std::vector<SMaterialCreateParams>& createParamsArray) const = 0;

};


#endif
