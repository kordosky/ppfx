#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <exception>
#include <string>

struct NoParameterFound : public std::exception{

  NoParameterFound(const std::string& name)
  {
    m_error=std::string("Parameter not defined: ")+name;
  }

  virtual ~NoParameterFound() throw() {}

  virtual const char* what() const throw(){
    return m_error.c_str();
  }

  std::string m_error;
};

#endif
