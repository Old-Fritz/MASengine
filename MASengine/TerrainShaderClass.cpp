#include "TerrainShaderClass.h"

TerrainShaderClass::TerrainShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_paramsBuffer = 0;
	m_sampleState[0] = 0;
	m_sampleState[1] = 0;
	m_sampleState[2] = 0;

}
TerrainShaderClass::TerrainShaderClass(const TerrainShaderClass &)
{
}
TerrainShaderClass::~TerrainShaderClass()
{
}

bool TerrainShaderClass::Initialize(ID3D11Device * device, HWND hwnd , PathClass* filenameVS, PathClass* filenamePS)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, filenameVS->getWPath().c_str(), filenamePS->getWPath().c_str());
	if (!result)
	{
		return false;
	}

	return true;
}

void TerrainShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

bool TerrainShaderClass::Render(ID3D11DeviceContext * deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
	ID3D11ShaderResourceView * texture, ID3D11ShaderResourceView* physTexture, ID3D11ShaderResourceView** mapTextures,
	D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
	D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower, D3DXVECTOR4* provsColor)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix,
		texture, physTexture, mapTextures, lightDirection,  ambientColor,  diffuseColor,
		 cameraPosition,  specularColor,  specularPower,  provsColor);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 10-2");
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

bool TerrainShaderClass::InitializeShader(ID3D11Device * device, HWND hwnd, const WCHAR * vsFilename, const  WCHAR * psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc[3];
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC paramsBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "TerrainVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "TerrainPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc[0].Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc[0].AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc[0].AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc[0].AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc[0].MipLODBias = 0.0f;
	samplerDesc[0].MaxAnisotropy = 1;
	samplerDesc[0].ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc[0].BorderColor[0] = 0;
	samplerDesc[0].BorderColor[1] = 0;
	samplerDesc[0].BorderColor[2] = 0;
	samplerDesc[0].BorderColor[3] = 0;
	samplerDesc[0].MinLOD = 0;
	samplerDesc[0].MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc[0], &m_sampleState[0]);
	if (FAILED(result))
	{
		return false;
	}

	// Create a texture sampler state description.
	samplerDesc[1].Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc[1].AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc[1].AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc[1].AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc[1].MipLODBias = 0.0f;
	samplerDesc[1].MaxAnisotropy = 1;
	samplerDesc[1].ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc[1].BorderColor[0] = 0;
	samplerDesc[1].BorderColor[1] = 0;
	samplerDesc[1].BorderColor[2] = 0;
	samplerDesc[1].BorderColor[3] = 0;
	samplerDesc[1].MinLOD = 0;
	samplerDesc[1].MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc[1], &m_sampleState[1]);
	if (FAILED(result))
	{
		return false;
	}

	// Create a texture sampler state description.
	samplerDesc[2].Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc[2].AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc[2].AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc[2].AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc[2].MipLODBias = 0.0f;
	samplerDesc[2].MaxAnisotropy = 1;
	samplerDesc[2].ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc[2].BorderColor[0] = 0;
	samplerDesc[2].BorderColor[1] = 0;
	samplerDesc[2].BorderColor[2] = 0;
	samplerDesc[2].BorderColor[3] = 0;
	samplerDesc[2].MinLOD = 0;
	samplerDesc[2].MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc[2], &m_sampleState[2]);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType)+ 4;
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}


	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	paramsBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	paramsBufferDesc.ByteWidth = sizeof(ParamsBufferType) + 4;
	paramsBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	paramsBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	paramsBufferDesc.MiscFlags = 0;
	paramsBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&paramsBufferDesc, NULL, &m_paramsBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void TerrainShaderClass::ShutdownShader()
{
	// Release the light constant buffer.
	if (m_paramsBuffer)
	{
		m_paramsBuffer->Release();
		m_paramsBuffer = 0;
	}

	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the sampler state.
	if (m_sampleState[0])
	{
		m_sampleState[0]->Release();
		m_sampleState[0] = 0;
	}
	if (m_sampleState[1])
	{
		m_sampleState[1]->Release();
		m_sampleState[1] = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}

void TerrainShaderClass::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, const WCHAR * shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool TerrainShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
	ID3D11ShaderResourceView * texture, ID3D11ShaderResourceView* physTexture, ID3D11ShaderResourceView** mapTextures,
	D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
	D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower, D3DXVECTOR4* provsColor)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	ParamsBufferType* dataPtr2;

	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	dataPtr->cameraPosition = cameraPosition;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);



	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 1, &physTexture);
	deviceContext->PSSetShaderResources(2, NUM_OF_MAP_TEXTURES, mapTextures);

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_paramsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the light constant buffer.
	dataPtr2 = (ParamsBufferType*)mappedResource.pData;

	// Copy the lighting variables into the light constant buffer.
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->ambientColor = ambientColor;
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->cameraPosition = cameraPosition;
	dataPtr2->specularColor = specularColor;
	dataPtr2->specularPower = specularPower;
	for (int i = 0;i < 256;i++)
		dataPtr2->provsColor[i] = provsColor[i];

	// Unlock the light constant buffer.
	deviceContext->Unmap(m_paramsBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_paramsBuffer);

	return true;
}

void TerrainShaderClass::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 3, m_sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
