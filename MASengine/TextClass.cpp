///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "textclass.h"

TextClass::TextClass()
{
	m_sentencesNum = 0;
	m_maxLength = 0;
	m_sentences = 0;
	m_fontFilename = 0;
	//m_orientation = 1;
}

TextClass::TextClass(const TextClass& other)
{
}

TextClass::~TextClass()
{
}


bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, int sentencesNum,
	int maxLength, int orientation, PathClass* fontFilename)
{
	bool result;

	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_orientation = (orient)orientation;
	m_fontFilename = fontFilename;

	// Create the font object.
	result = FontManagerClass::getI().addFont(device, m_fontFilename);
	if (!result)
	{
		LogManagerClass::getI().addLog("Error 12-1");
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create sentences.
	m_maxLength = maxLength;
	m_sentencesNum = sentencesNum;
	m_sentences = new(4) SentenceType*[sentencesNum];
	if (!m_sentences)
	{
		return false;
	}


	// Initialize all sentences.
	for (int i = 0; i < sentencesNum; i++)
	{
		result = InitializeSentence(device, &m_sentences[i], maxLength);
		if (!result)
		{
			LogManagerClass::getI().addLog("Error 12-2");
			return false;
		}
		// Now update the sentence vertex buffer with the usual information.
		result = UpdateSentence(deviceContext, i, L" ", 0, 0, 0.35f, 0, D3DXVECTOR4(0,0,0,0));
		if (!result)
		{
			LogManagerClass::getI().addLog("Error 12-3");
			return false;
		}
	}
	return true;
}

void TextClass::Shutdown()
{

	// Release all sentences.
	for (int i = 0; i < m_sentencesNum; i++)
	{
		ReleaseSentence(&m_sentences[i]);
	}

	return;
}

bool TextClass::Render(FontShaderClass* FontShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix, D3DXMATRIX baseViewMatrix)
{
	bool result;

	// Draw all sentences.
	for (int i = 0; i < m_sentencesNum; i++)
	{
		result = RenderSentence(FontShader, deviceContext, m_sentences[i], worldMatrix, orthoMatrix, baseViewMatrix);
		if (!result)
		{
			LogManagerClass::getI().addLog("Error 12-9");
			return false;
		}
	}

	return true;
}

bool TextClass::InitializeSentence(ID3D11Device* device, SentenceType** sentence, int maxLength)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create a new sentence object.
	*sentence = new(4) SentenceType;
	if (!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new(3) VertexType[(*sentence)->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new(3) unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType)* (*sentence)->vertexCount));

	// Initialize the index array.
	for (i = 0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}
	


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		LogManagerClass::getI().addLog("Error 12-4");
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		LogManagerClass::getI().addLog("Error 12-5");
		return false;
	}

	// Release the vertex and index array as it is no longer needed.
	MemoryManagerClass::getI().cleanTemp();
	vertices = 0;
	indices = 0;

	return true;
}

bool TextClass::UpdateSentence(ID3D11DeviceContext* deviceContext, int sentenceNum, const std::wstring& text, int positionX, int positionY,  float size, float width, D3DXVECTOR4 color)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;
	SentenceType* sentence;

	if (sentenceNum < m_sentencesNum)
	{
		sentence = m_sentences[sentenceNum];
	}
	else
	{
		LogManagerClass::getI().addLog("Error 12-6");
		return false;
	}

	// Store the color of the sentence.
	sentence->color = color;

	// Get the number of letters in the sentence.
	numLetters = text.size();

	// Check for possible buffer overflow.
	if (numLetters > sentence->maxLength)
	{
		LogManagerClass::getI().addLog("Error 12-7");
		return false;
	}

	// Create the vertex array.
	vertices = new(3) VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType)* sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	FontManagerClass::getI().getFont(m_fontFilename)->BuildVertexArray((void*)vertices, text, drawX, drawY, size, width, (FontClass::orient)m_orientation);

	// Initialize the vertex buffer pointer to null first.
	verticesPtr = 0;

	// Lock the vertex buffer.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		LogManagerClass::getI().addLog("Error 12-8");
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the vertex array into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)* sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	MemoryManagerClass::getI().cleanTemp();
	vertices = 0;

	return true;
}

void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		// Release the sentence vertex buffer.
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		::operator delete(*sentence, sizeof(**sentence), 2);
		*sentence = 0;
	}

	return;
}

bool TextClass::RenderSentence(FontShaderClass* FontShader, ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix, D3DXMATRIX baseViewMatrix)
{
	bool result;
	unsigned int stride, offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render the text using the font shader.
	result = FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, baseViewMatrix, orthoMatrix, FontManagerClass::getI().getFont(m_fontFilename)->GetTexture(), sentence->color);
	if (!result)
	{
		return false;
	}

	return true;
}

