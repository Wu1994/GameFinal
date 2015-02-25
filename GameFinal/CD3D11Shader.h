#ifndef __CD3D11SHADER_H__
#define __CD3D11SHADER_H__

#include "GameFinal.h"
#include "D3DUtil.h"
#include "CD3D11Driver.h"
#include "CShader.h"
namespace gf
{
	struct SShaderConstantBuffer
	{
		std::string			Name;
		u32					Size;
		bool				Dirty;

		void*				pData;


		SShaderConstantBuffer(const char* Name)
			:Name(Name)
		{
			Dirty = false;
			pData = NULL;
		}

		~SShaderConstantBuffer()
		{
			free(pData);
			pData = nullptr;
		}
	};

	struct SShaderConstantVariable
	{
		u32								ConstantBufferIndex;
		bool							AlreadyUpdated; /* this flag is just for those per-frame-updated variables. */
		D3D11_SHADER_VARIABLE_DESC		Desc;
		D3D11_SHADER_TYPE_DESC			TypeDesc;
	};

	

	class CD3D11Shader : public CShader
	{
	public:
		enum E_SHADER_BIND_RESOURCE_TYPE
		{
			ESBRT_SHADER_RESOURCE,
			ESBRT_UNORDERRED_ACCESS,
			ESBRT_SAMPLER,
		};

		struct SD3D11ShaderBindResourceInfo
		{
			E_SHADER_BIND_RESOURCE_TYPE				Type;
			D3D11_SHADER_INPUT_BIND_DESC			Desc;
		};

	public:
		CD3D11Shader(u32 id,
			const std::string& name,
			const SShaderMacroSet& macros,
			ID3D11Device* pd3dDevice,
			ID3D11DeviceContext* pd3dDeviceContext,
			CD3D11Driver* pd3dDriver)
			:CShader(id, name, macros),
			md3dDevice(pd3dDevice),
			md3dDeviceContext(pd3dDeviceContext),
			md3dDriver(pd3dDriver),
			mSrvNum(0),
			mUavNum(0),
			mSamplerNum(0),
			mShaderBuffer(nullptr)
		{
			memset(md3dConstantBuffers, 0, sizeof(md3dConstantBuffers));
		}

		virtual const char* getShaderProfile() const = 0;

		virtual bool loadFromFile(const char* szFileName, const char* szFunctionName) = 0;

		bool getShaderFileContent(const char* szFullPath, char content[]);

		ID3D10Blob* getShaderBuffer()
		{
			return mShaderBuffer;
		}

		bool compile(const char* szFileName, const char* szFunctionName);

		bool initContext();

		virtual ~CD3D11Shader()
		{
			for (u32 i = 0; i < MAX_CONSTANT_BUFFER_NUM; i++)
				ReleaseCOM(md3dConstantBuffers[i]);

			ReleaseCOM(mShaderBuffer);
		}


		SShaderConstantVariable* getConstantVariable(const std::string& varname);

		virtual bool setRawData(const std::string& varname, void* data, u32 size, bool ignoreIfAlreadyUpdate = false);

		virtual u32 setArray(const std::string& varname, void* data, u32 arraySize, u32 elementSize, bool ignoreIfAlreadyUpdate = false);

		virtual bool setUint(const std::string& varname, u32 val, bool ignoreIfAlreadyUpdate = false);

		virtual bool setFloat(const std::string& varname, f32 val, bool ignoreIfAlreadyUpdate = false);

		virtual bool setVector(const std::string& varname, const XMFLOAT4& val, bool ignoreIfAlreadyUpdate = false);

		virtual bool setVector(const std::string& varname, const f32* val, bool ignoreIfAlreadyUpdate = false);

		virtual bool setAttribute(const std::string& varname, const XMFLOAT4& val, bool ignoreIfAlreadyUpdate = false);

		//virtual bool setVector3(const std::string& varname, const f32* val, bool ignoreIfAlreadyUpdate = false) = 0;

		virtual bool setMatrix(const std::string& varname, const f32* matrix, bool ignoreIfAlreadyUpdate = false);

		virtual bool setMatrix(const std::string& varname, CXMMATRIX matrix, bool ignoreIfAlreadyUpdate = false);

		virtual bool setMatrix(const std::string& varname, const XMFLOAT4X4& matrix, bool ignoreIfAlreadyUpdate = false);

		virtual bool setTexture(const std::string& varname, ITexture* texture);

		virtual bool setTransposedMatrixArray(const std::string& var, const f32* matrixs, u32 count, bool ignoreIfAlreadyUpdate = false);

		virtual bool setMatrixArray(const std::string& var, XMFLOAT4X4 matrixs[], u32 count, bool ignoreIfAlreadyUpdate = false);

		virtual bool setSampler(const std::string& varname, ISampler* sampler);

		virtual bool existSampler(const std::string& varname) const;

		bool applyConstantBuffer(ID3D11DeviceContext* pd3dDeviceContext, u32 index);

		virtual void update(bool forceUpdate = false);

		virtual void submit() = 0;

		virtual u32 getTextureCount() const
		{
			return mSrvNum;
		}

		virtual u32 getSamplerCount() const
		{
			return mSamplerNum;
		}

		virtual u32 getRWTextureCount() const
		{
			return mUavNum; 
		}

		virtual void reset();

		virtual bool isAlreadyUpdated(const std::string& varname);

		virtual bool isContantVariable(const std::string& varname) const;

		//virtual bool isTextureVariable(const std::string& varname) const;

		virtual void registerAutoVariables(const std::map<std::string, SShaderVariableAttribute>& varMap);

	protected:
		const static u32 MAX_CONSTANT_BUFFER_NUM = 16;

		bool setRawData(SShaderConstantVariable* cv, void* data, u32 size);

		ID3D11Device*						md3dDevice;
		ID3D11DeviceContext*				md3dDeviceContext;
		ID3D10Blob*							mShaderBuffer;

		CD3D11Driver*						md3dDriver;

		std::string							mName;
		ID3D11Buffer*						md3dConstantBuffers[MAX_CONSTANT_BUFFER_NUM];
		u32									mSrvNum;
		u32									mUavNum;
		u32									mSamplerNum;

		std::vector<std::unique_ptr<SShaderConstantBuffer>>		mConstantBuffers;
		std::map<std::string, SShaderConstantVariable>			mConstantVariables;

		//std::map<std::string, D3D11_SHADER_INPUT_BIND_DESC>		mShaderResourceDescs;
		//std::map<std::string, D3D11_SHADER_INPUT_BIND_DESC>		mShaderSamplerDescs;

		std::map<std::string, SD3D11ShaderBindResourceInfo>		mBindingResourceInfos;
	};
}
#endif