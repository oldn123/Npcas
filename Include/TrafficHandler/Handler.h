#ifndef HANDLER_H
#define HANDLER_H

#include <string>

class Handler
{
public:
    virtual bool handlerRequest(const std::string& request) = 0;
    virtual void setSuccessor(Handler* successor) = 0;
    virtual Handler* getNextSuccessor() = 0;
};


#endif // HANDLER_H
