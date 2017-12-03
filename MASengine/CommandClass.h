////////////////////////////////////////////////////////////////////////////////
// Filename: CommandClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _COMMANDCLASS_H_
#define _COMMANDCLASS_H_


//////////////
// INCLUDES //
//////////////

#include "CalculatorClass.h"
#include "ConditionCheckerClass.h"
#include <map>

////////////////////////////////////////////////////////////////////////////////
// Class name: CommandClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup CommandManagment
@{
*/
/*!
Исполняемый скрипт скриптоязыка
*/
class CommandClass
{
public:
	CommandClass();
	CommandClass(const CommandClass&);
	~CommandClass();

	//! Инициализация команд /param[in] strCommand - код скриптоязыка
	void Initialize(const std::string& strCommand);
	void Shutdown();

	//! Получение количества команд в скрипте \return кол-во команд
	int getCommandsNum();
	//! Получение параметров в команде \param[in] commandNumber - номер команды \return Кол-во параметров
	int getParamsNum(int commandNumber);

	//! Получение конкретного параметра со всеми заменами \param[in] commandNumber - номер команды \param[in] ParamNum - номер параметра \return Параметр
	std::string getParam(int commandNumber, int ParamNum);
	//! Получение конкретного параметра без замен \param[in] commandNumber - номер команды \param[in] ParamNum - номер параметра \return Параметр
	std::string getInitParam(int commandNumber, int ParamNum);

	//work with changes
	//! Добавление замены \param[in] key - заменяемое слово \param[in] value - замена
	void addChange(const std::string& key, const std::string& value);
	//! Добавление замены \param[in] key - заменяемое слово \param[in] value - замена
	void addChange(const std::string& key, float value);
	//! Добавление замены \param[in] key - заменяемое слово \param[in] value - замена
	void addChange(const std::string& key, int value);
	//! Редактирование параметра заменами \param[in] param - параметр \return Итоговый параметр 
	std::string makeChanges(const std::string& param);

	//! Разделение замен с другой командой \param[in] command - команда, в которую будут добавлены замены из текущей
	void shareChanges(CommandClass* command);
private:
	int m_commandsNum; //!< Количество команд
	std::vector<std::vector<std::string>> m_commands; //!< Команды
	std::map<std::string, std::string> m_changes; //!< Замены
};
/*! @} */
#endif