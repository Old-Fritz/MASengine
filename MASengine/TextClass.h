////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "fontclass.h"
#include "FontShaderClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextClass
////////////////////////////////////////////////////////////////////////////////
class TextClass
{
private:

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		D3DXVECTOR4 color;
	};
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, int sentencesNum,
		int maxLength, int orientation, PathClass* fontFilename);
	void Shutdown();
	bool Render(FontShaderClass* FontShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix, D3DXMATRIX baseViewMatrix);
	bool UpdateSentence(ID3D11DeviceContext* deviceContext, int sentenceNum, const std::wstring& text, int positionX, int positionY,
		float size, float width, D3DXVECTOR4 color);
private:
	bool InitializeSentence(ID3D11Device* device, SentenceType** sentence, int maxLength);
	void ReleaseSentence(SentenceType** sentence);
	bool RenderSentence(FontShaderClass* FontShader, ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix,
		D3DXMATRIX orthoMatrix, D3DXMATRIX baseViewMatrix);

private:
	PathClass* m_fontFilename;
	int m_screenWidth, m_screenHeight;
	int m_sentencesNum;
	int m_maxLength;
	SentenceType** m_sentences;
	enum orient { LEFT, MIDDLE, RIGHT } m_orientation = LEFT;

};

#endif