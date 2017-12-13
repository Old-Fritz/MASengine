////////////////////////////////////////////////////////////////////////////////
// Filename: TerrainManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINMANAGERCLASS_H_
#define _TERRAINMANAGERCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "TerrainClass.h"
#include "LoadScreenManagerClass.h"
#include<string>

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
	Прорисовка блоков \param[in] D3D - API Directx для рендринга в текстуру \param[in] terrainShader - шейдер блоков \param[in] waterShader - шейдер воды 
	\param[in] fillShader - шейдер заполнения  \param[in] skyShader - шейдер неба  \param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрами 
	\param[in] topViewMatrix - 10 матриц для камеры сверху блока \param[in] lights - источники света \param[in] cameraPosition - позиция камеры
	\param[in] SCREEN_DEPTH - глубина экрана \param[in] frustum - конус усечения  \return false, если были ошибки
	*/
	bool Render(D3DClass* D3D, TerrainShaderClass* terrainShader, WaterShaderClass* waterShader, FillShaderClass* fillShader, SkyShaderClass* skyShader, 
		D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, std::vector<D3DXMATRIX> topViewMatrix, std::vector<LightClass::PointLightType*> lights,
		D3DXVECTOR3 cameraPosition, float SCREEN_DEPTH, FrustumClass* frustum);

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

	//! Получение позиций камеры, необходимой для рендринга в текстуру блока для 10-ти уровней детализации \return 10 координат камер
	std::vector<D3DXVECTOR3> getBlockTopCameraPos();
private:
	//! Загрузка текстур для физических карт  \param[in] device - графическое устройство
	bool initializeMapTextures(ID3D11Device* device);
private:
	std::vector<TerrainClass*> m_terrain; //!< Блоки
	SkyModelClass* m_sky; //!< Небо

	int m_mapTextureHashes[NUM_OF_MAP_TEXTURES]; //!< Хэши текстур для физических карт

	std::string m_pickCommand; //!< Имя команды, выпоняемой при клике
	std::string m_unPickCommand; //!< Имя команды, выпоняемой отжатии клиика

	RenderTextureClass* m_skyTexture; //!< Текстура отраженного неба для воды
};
/*! @} */
#endif
