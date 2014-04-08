#include "stdafx.h"
#include "CD3D11HullShader.h"

bool CD3D11HullShader::loadFromFile(const char* szFileName, const char* szFunctionName)
{
	HRESULT hr;
	if (!compile(szFileName, szFunctionName, EST_HULL_SHADER))
		return false;

	hr = md3dDevice->CreateHullShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), NULL, &md3dHullShader);

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void CD3D11HullShader::submit()
{
	u32 bufferCount = mConstantBuffers.size();

	md3dDeviceContext->HSSetConstantBuffers(0, bufferCount, md3dConstantBuffers);

	md3dDeviceContext->HSSetShader(md3dHullShader, 0, 0);
}

