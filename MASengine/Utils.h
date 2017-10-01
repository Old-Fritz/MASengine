////////////////////////////////////////////////////////////////////////////////
// Filename: UtilsClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _UTILSCLASS_H_
#define _UTILSCLASS_H_

#include <string>
#include <filesystem>
#include <corecrt_wstring.h>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////
// Class name: PathClass
////////////////////////////////////////////////////////////////////////////////
class Utils
{
public:
	static int getHash(const std::string & str);

	static std::string getTextFromFile(const std::string& name, const std::string& filename);
	static std::wstring getWTextFromFile(const std::string& name, const std::string&  filename);

	static std::wstring from_bytes(const std::string& str);
	static std::string to_bytes(const std::wstring& str);
private:
	Utils();

private:
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> m_converter;
};

#endif