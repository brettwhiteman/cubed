#ifndef CUBED_EXCEPTION_H
#define CUBED_EXCEPTION_H

#include <exception>
#include <string>
#include <utility>

class CubedException : public std::exception
{
public:
	CubedException(std::string message) :
		m_message(std::move(message)) { }

	virtual ~CubedException() { }

	const char* what() const { return m_message.c_str(); }

private:
	std::string m_message;
};

#endif