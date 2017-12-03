////////////////////////////////////////////////////////////////////////////////
// Filename: CommandManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COMMANDMANAGERCLASS_H_
#define _COMMANDMANAGERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "CommandClass.h"
#include "PathManagerClass.h"
#include <queue>
#include <fstream>
#include <unordered_map>

////////////////////////////////////////////////////////////////////////////////
// Class name: CommandManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup CommandManagment Управление командами
\ingroup GlobalManager
@{
*/
/*!
Класс для управления командами, синглтон
*/
class CommandManagerClass
{
public:
	//! Перечисление всех возмлжных команд
	enum command {
		updateInterface,
		setElVisible, startDrag, endDrag, elPos, //Updating params of element
		BMposX, BMposY, BMvertPercent, BMhorPercent,  
		BMtranspar, BMselIntens, BMselCol, setBMvisible, //Updating params of bitmaps
		TSposX, TSposY, TStext, TScolor, TSadding, setTvisible,  //Updating params of strings
		nextSector, backSector, //Special slider commands
		addElement, deleteElement, setLastElement, clearElement,  //Special list commands
		setBMCommand, //another commands

		updateGraphics, 
		cameraPosition, cameraRotation, pick, unPick,

		updateSystem, 
		position, rotation, setButCommand, setWheelCommand, setMoving,

		reboot,

		stop,

		nothing,

		operators,
		IF,
		
		get,
		getForward, getProvRegionId,
		
		set,
		setProvRegion
	};
public:
	/*!
	Инициализация класса
	\return false, если были ошибки
	*/
	bool Initialize();
	void Shutdown();
	//! \brief Получить экземпляр класса
	static CommandManagerClass& getI();

	//! Проверка на заполненность очереди \return true, если в очереди еще есть элементы, false, если очередь пуста
	bool isFull();
	//! Получение следующей команды и удаление её из очереди \return Первая команда в очереди
	CommandClass* nextCommand();
	//! Создание отдельной команды вне очереди \param[in] name - имя команды \param[in] filename - файл с кодом команды \return Полученная команда
	CommandClass* makeSingleCommand(const std::string & name, PathClass* filename);

	//! Добавление команды в очередь \param[in] name - имя команды \param[in] filename - файл с кодом команды \return false, если были ошибки
	bool addCommand(const std::string& name, PathClass* filename);
	//! Добавление замены для последней команды в очереди \param[in] key - заменяемое слово \param[in] value - замена
	void addChange(const std::string& key, const std::string& value);
	//! Добавление замены для последней команды в очереди \param[in] key - заменяемое слово \param[in] value - замена
	void addChange(const std::string& key, int value);
	//! Добавление замены для последней команды в очереди \param[in] key - заменяемое слово \param[in] value - замена
	void addChange(const std::string& key, float value);

	//! Получение типа команды по её имени \param[in] key - название команды \return тип команды из перечисления
	command getCommandEnum(const std::string& key);
	//! Получение типа команды по её хэшу названия \param[in] hash - хэш названия команды \return тип команды из перечисления
	command getCommandEnum(int hash);

	//! Добавление глобальной замены (для всех команд) \param[in] key - заменяемое слово \param[in] value - замена
	void addGlobalChange(const std::string& key, const std::string& value);
	//! Добавление глобальной замены (для всех команд) \param[in] key - заменяемое слово \param[in] value - замена
	void addGlobalChange(const std::string& key, int value);
	//! Добавление глобальной замены (для всех команд) \param[in] key - заменяемое слово \param[in] value - замена
	void addGlobalChange(const std::string& key, float value);
private:
	//! Разделение глобальных замен с другой командой \param[in] command - команда, в которую будут добавлены глобальные замены
	void shareGlobal(CommandClass* command);
	//! Загрузка команды из файла \param[in] name - имя команды \param[in] filename - файл с кодом команды \return полученная команда
	CommandClass* loadCommand(const std::string& name, PathClass* filename);
	CommandManagerClass();
	CommandManagerClass(const CommandManagerClass&);
	~CommandManagerClass();
private:
	std::queue<CommandClass*> m_commandsQueue; //! Очередь команд
	std::map<int, std::map<int, CommandClass*>> m_commands; //! Загруженные команды
	std::map<std::string, std::string> m_globalChanges; //! Глобальные замены
	std::unordered_map<int, command> m_commandMap; //! словарь хэшей названий и соответствующих типов из перечисления

	static CommandManagerClass* m_instance; //!<Единственный экземпляр класса
};
/*! @} */
#endif