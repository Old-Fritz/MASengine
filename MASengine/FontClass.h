////////////////////////////////////////////////////////////////////////////////
// Filename: fontclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <fstream>
#include <string>
#include <vector>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "PathManagerClass.h"
#include"TextureManagerClass.h"
#include <D3DX10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: FontClass
////////////////////////////////////////////////////////////////////////////////
class FontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
	struct FontGroupType
	{
		std::map<int, FontType*> fonts;

		FontType& operator [] (int ind)
		{
			//check for existing of symbol in map
			if (fonts.find(ind) == fonts.end())
				return *fonts[0];
			else
				return *fonts[ind];
		}

		void emplace(int ind, FontType* font)
		{
			fonts.emplace(std::pair<int, FontType*>(ind, font));
		}
		void Shutdown()
		{
			while (!fonts.empty())
			{
				::operator delete(fonts.begin()->second, sizeof(FontType), 4);
				fonts.begin()->second = 0;
				fonts.erase(fonts.begin());
			}
		}
	};

public:
	enum orient { LEFT, MIDDLE, RIGHT };
public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device* device, PathClass* filename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
	void BuildVertexArray(void* vertices, const std::wstring& sentence, float drawX, float drawY, float size, float width, orient orientation);

private:
	bool LoadFontData(PathClass* filename);
	void ReleaseFontData();
	std::vector<std::pair<std::wstring, int>> separateStrings(const std::wstring& sentence, float size, float width);

private:
	int m_maxSize; // max width of one symbol
	FontGroupType m_Font;
	PathClass* m_filename;

	int m_SymbolsNum;
	int m_SymbolsHeight;
};

#endif
