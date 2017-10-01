////////////////////////////////////////////////////////////////////////////////
// Filename: bitmapclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BITMAPCLASS_H_
#define _BITMAPCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include"GlobalManagerClass.h"



////////////////////////////////////////////////////////////////////////////////
// Class name: BitmapClass
////////////////////////////////////////////////////////////////////////////////
class BitmapClass
{
private:

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	BitmapClass();
	BitmapClass(const BitmapClass&);
	~BitmapClass();

	bool Initialize(ID3D11Device* device, int screenWidth, int screenHeight, PathClass* textureFilename,
		int bitmapWidth, int bitmapHeight);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY);

	bool setNewTexture(ID3D11Device* device, PathClass* filename);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_screenWidth, m_screenHeight;
	PathClass* m_filename;

	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
};

#endif