////////////////////////////////////////////////////////////////////////////////
// Filename: TerrainManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINMANAGERCLASS_H_
#define _TERRAINMANAGERCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "TerrainClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Blocks
@{
*/
/*!
Менеджер блоков, составляющий общую карту
*/
class TerrainManagerClass
{
public:
	TerrainManagerClass();
	TerrainManagerClass(const TerrainManagerClass&);
	~TerrainManagerClass();

	//! Загрузка блоков \param[in] device, deviceContext - графическое устройство \param[in] terrainFilenameBase - основа для путей до файлов блоков
	//! \param[in] numOfBlocks - Количество блоков  \return false, если были ошибки
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string terrainFilenameBase, int numOfBlocks);
	void Shutdown();
	/*!
	Прорисовка блоков \param[in] terrainShader - шейдер блоков \param[in] waterShader - шейдер воды \param[in] deviceContext - графическое устройство
	\param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрами
	\param[in] lightDirection - направление света \param[in] ambientColor - цвет обтеквющего света \param[in] diffuseColor - цвет диффузного света
	\param[in] cameraPosition - позиция камеры \param[in] specularColor - цвет зеркального света \param[in] specularPower - мощность зеркального света
	\param[in] SCREEN_DEPTH - глубина экрана \param[in] frustum - конус усечения  \return false, если были ошибки
	*/
	bool Render(TerrainShaderClass* terrainShader, WaterShaderClass* waterShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor,
		D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,
		float SCREEN_DEPTH, FrustumClass* frustum);

	//pick actions
	//! Получение провинции, выбранной при клике \param[in] deviceContext - графическое устройство \param[in] mouseX, mouseY - позиция курсора 
	//! \param[out] provNum - индекс выбранной провинции \param[out] point - точка пересечения
	//! \return false, если не был выбран ни один элемент, true, если был
	bool pick(ID3D11DeviceContext * deviceContext, D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, int& provNum, D3DXVECTOR3& point);

	//gets
	//! Получение имени команды, выпоняемой при клике \return имя команды
	std::string getPickCommandName();
	//! Получение имени команды, выпоняемой отжатии клиика \return имя команды
	std::string getUnPickCommandName();
private:
	//! Загрузка текстур для физических карт  \param[in] device - графическое устройство
	bool initializeMapTextures(ID3D11Device* device);
private:
	std::vector<TerrainClass*> m_terrain; //!< Блоки

	int m_mapTextureHashes[NUM_OF_MAP_TEXTURES]; //!< Хэши текстур для физических карт

	std::string m_pickCommand; //!< Имя команды, выпоняемой при клике
	std::string m_unPickCommand; //!< Имя команды, выпоняемой отжатии клиика
};
/*! @} */
#endif
