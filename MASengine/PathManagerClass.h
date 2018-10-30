////////////////////////////////////////////////////////////////////////////////
// Filename: PathManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PATHMANAGERCLASS_H_
#define _PATHMANAGERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "PathClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GM
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Utils
@{
*/
/*!
”правление всеми пут¤ми до файлов, синглтон
*/
class PathManagerClass
{
public:
	void Shutdown();

	//! \brief ѕолучить экземпл¤р класса
	static PathManagerClass& getI();

	bool Initialize();

	//! —оздание пути с нулевыми параметрами \return новый путь
	PathClass* makePath();
	//! —оздание пути из строки \param[in] filename - путь до файла \return новый путь
	PathClass* makePath(const std::string& filename);
	//! —оздание пути из строки в UNICODE \param[in] filename - путь до файла в UNICODE \return новый путь
	PathClass* makePath(const std::wstring& filename);

private:
	PathManagerClass();
	PathManagerClass(const PathManagerClass&);
	~PathManagerClass();
private:
	static PathManagerClass* m_instance; //!<≈динственный экземпл¤р класса

	std::vector<PathClass*> m_pathes; //!<¬се пути до файлов
};
/*! @} */
#endif