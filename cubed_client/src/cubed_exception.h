#ifndef CUBED_EXCEPTION_H
#define CUBED_EXCEPTION_H

#include <exception>
#include <string>
#include <utility>

class cubed_exception : public std::exception
{
public:
	cubed_exception(std::string message) : m_message(std::move(message)) { }
	virtual ~cubed_exception() { }

	const char* what() const { return m_message.c_str(); }

private:
	std::string m_message;
};

#endif