////////////////////////////////////////////////////////////////////////////////
// Filename: LogManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LOGMANAGERCLASS_H_
#define _LOGMANAGERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <fstream>
#include <string>
#include <ctime>

////////////////////////////////////////////////////////////////////////////////
// Class name: LogManagerClass
////////////////////////////////////////////////////////////////////////////////
class LogManagerClass
{
private:
	static LogManagerClass* m_instance;
	std::ofstream m_file;
public:
	

	bool Initialize(const std::string& filepath);
	void Shutdown();

	static LogManagerClass& getI();

	void addLog(const std::string& log);
private:
	LogManagerClass();
	LogManagerClass(const LogManagerClass&);
	~LogManagerClass();
	std::string getTime();

};


#endif