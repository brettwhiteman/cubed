#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <string>

class Exception
{
public:
	Exception(std::string message) : m_message(std::move(message)) { }
	virtual ~Exception() { }

	virtual const std::string& getMessage() const { return m_message; }

private:
	std::string m_message;
};

#endif