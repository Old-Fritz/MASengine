///////////////////////////////////////////////////////////////////////////////
// Filename: fontclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "fontclass.h"

FontClass::FontClass()
{
	m_SymbolsNum = 0;
	m_SymbolsHeight = 0;
	m_maxSize = 0;
}
FontClass::FontClass(const FontClass& other)
{
}
FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device* device, const std::string& filename)
{
	bool result;
	std::string fontFilename;
	std::string textureFilename;

	//load filename from description
	std::ifstream file;
	file.open(ModManagerClass::getI().getDirectory(ModManagerClass::getI().getHash(filename))+filename);
	if (!file.is_open())
		return false;

	file >> fontFilename >> textureFilename;

	file.close();

	// Load in the text file containing the font data.
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	// Load the texture that has the font characters on it.
	m_filename = textureFilename;

	result = TextureManagerClass::getI().addTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{

	// Release the font data.
	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(const std::string& filename)
{
	std::ifstream fin;
	int i;
	std::string temp;


	// Read in the font size and spacing between chars.
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	fin >> m_SymbolsNum;
	fin >> m_SymbolsHeight;


	// Read in all used ascii characters for text.
	for (i = 0; i<m_SymbolsNum; i++)
	{
		FontType* newSymbol = new(4) FontType;
		int unicodeNum;

		fin >> unicodeNum;
		fin >> temp;
		fin >> newSymbol->left;
		fin >> newSymbol->right;
		fin >> newSymbol->size;
		if (newSymbol->size > m_maxSize) //save max size
			m_maxSize = newSymbol->size;
		//newSymbol.size /= 2;

		m_Font.emplace(unicodeNum, newSymbol);
	}

	// Close the file.
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	// Release the font data array.
	m_Font.Shutdown();

	return;
}

std::vector<std::pair<std::wstring, int>> FontClass::separateStrings(const std::wstring& sentence, float size, float width)
{
	std::vector<std::pair<std::wstring, int>> sentences;
	std::wstring tempsent;
	int deletedPart = 0;

	while (true)
	{
		
		int tempsize = 0;
		int i = deletedPart;
		while (i<sentence.size()-1 && tempsize < width)
		{
			tempsize += m_Font[(unsigned int)sentence[i]].size * size;
			i++;
		}
		// check for end of sentence
		if (tempsize < width)
		{
			for (int j = deletedPart; j <= sentence.size(); j++)
				tempsent += sentence[j];
			sentences.emplace_back(tempsent, tempsize);
			break;
		}

		int lastind = i - 1; //ind of last letter that is in string
		tempsize -= m_Font[(unsigned int)sentence[i]].size * size;
		int lastsize = tempsize;
		i--;

		//find end of last full word
		while (i > deletedPart && (unsigned int)sentence[i] != ' ')
		{
			tempsize -= (unsigned int)m_Font[sentence[i]].size * size;
			i--;
		}

		//if this is long word - separate it
		if ((unsigned char)sentence[i] != ' ')
		{
			for (int j = deletedPart; j <= lastind; j++)
				tempsent += sentence[j];
			sentences.emplace_back(tempsent, lastsize);
			deletedPart = lastind + 1;
		}
		// in another way separate only part of sentence
		else
		{
			//copy to vector
			for (int j = deletedPart; j < i; j++)
				tempsent += sentence[j];
			sentences.emplace_back(tempsent, tempsize);
			deletedPart = i + 1;
		}
		tempsent.clear();
	}

	return sentences;
}



ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return TextureManagerClass::getI().getTexture(m_filename);
}

void FontClass::BuildVertexArray(void* vertices, const std::wstring& sentence, float drawX, float drawY, float size, float width, orient orientation)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;

	//save start pos of drawX
	int drawXst = drawX;
	size /= m_SymbolsHeight;

	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	//check if string can be in borders
	if (m_maxSize*size > width)
		return;

	//separate string
	std::vector<std::pair<std::wstring, int>> sentences = separateStrings(sentence, size, width);

	// Initialize the index to the vertex array.
	index = 0;


	for (int j = 0; j < sentences.size(); j++)
	{
		drawX = drawXst;
		// Get the number of letters in the sentence.
		numLetters = sentences[j].first.size();
		switch (orientation)
		{
		case RIGHT:
		{
			drawX += width - sentences[j].second;
			break;
		}
		case MIDDLE:
		{
			drawX += (width - sentences[j].second) / 2;
			break;
		}
		default:
			break;
		}

		// Draw each letter onto a quad.
		for (i = 0; i<numLetters; i++)
		{
			FontType thisSymbol;

			letter = (int)sentences[j].first[i];

			thisSymbol = m_Font[letter];

			// If the letter is a space then just move over three pixels.
			if (letter == 0)
			{
				drawX = drawX + thisSymbol.size * size;
			}
			else
			{
				// First triangle in quad.
				vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
				vertexPtr[index].texture = D3DXVECTOR2(thisSymbol.left, 0.0f);
				index++;

				vertexPtr[index].position = D3DXVECTOR3((drawX + thisSymbol.size* size), (drawY - m_SymbolsHeight* size), 0.0f);  // Bottom right.
				vertexPtr[index].texture = D3DXVECTOR2(thisSymbol.right, 1.0f);
				index++;

				vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - m_SymbolsHeight* size), 0.0f);  // Bottom left.
				vertexPtr[index].texture = D3DXVECTOR2(thisSymbol.left, 1.0f);
				index++;

				// Second triangle in quad.
				vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
				vertexPtr[index].texture = D3DXVECTOR2(thisSymbol.left, 0.0f);
				index++;

				vertexPtr[index].position = D3DXVECTOR3(drawX + thisSymbol.size* size, drawY, 0.0f);  // Top right.
				vertexPtr[index].texture = D3DXVECTOR2(thisSymbol.right, 0.0f);
				index++;

				vertexPtr[index].position = D3DXVECTOR3((drawX + thisSymbol.size* size), (drawY - m_SymbolsHeight* size), 0.0f);  // Bottom right.
				vertexPtr[index].texture = D3DXVECTOR2(thisSymbol.right, 1.0f);
				index++;

				// Update the x location for drawing by the size of the letter and one pixel.
				drawX = drawX + thisSymbol.size * size;
			}

		}
		drawY -= m_SymbolsHeight*size + 2 * size;  // go down on height of symbols and little more
	}


	return;
}