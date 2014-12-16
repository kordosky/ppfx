#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <exception>
using namespace std;

struct NoParameterFound : public exception{

  const char* what() const throw(){
    return "=> The asked parameter is not definied!!";
  }
};

#endif
