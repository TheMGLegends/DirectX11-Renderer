#pragma once

#include <exception>

#include <string>	

class BaseException : public std::exception
{ 
private:
	int line;
	std::string file;

protected:
	mutable std::string whatBuffer; // INFO: Mutable Allows for Modification of a Member Variable in a Const Function

public:
	inline BaseException(int _line, const char* _file) : line(_line), file(_file) {}
	virtual const char* what() const override;

	inline virtual const char* GetType() const { return "Base Exception"; }
	inline int GetLine() const { return line; }
	inline const std::string& GetFile() const { return file; }

	std::string GetOriginContext() const;

};

