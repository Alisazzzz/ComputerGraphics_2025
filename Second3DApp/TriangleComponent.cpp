#include "TriangleComponent.h"

int TriangleComponent::CompileShader(LPCWSTR shaderFile,
	const D3D_SHADER_MACRO* shaderMacros,
	ID3DInclude* pInclude,
	LPCSTR entryPoint,
	LPCSTR shaderType,
	UINT flags1,
	UINT flags2,
	ID3DBlob* code,
	ID3DBlob* errorMsg,
	HWND hWnd)
{
	HRESULT res = D3DCompileFromFile(shaderFile,
		shaderMacros,
		pInclude,
		entryPoint,
		shaderType,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&code,
		&errorMsg);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorMsg) {
			char* compileErrors = (char*)(errorMsg->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hWnd, shaderFile, L"Missing Shader File", MB_OK);
		}
	}

	return 0;
}
