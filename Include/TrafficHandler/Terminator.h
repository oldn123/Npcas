#ifndef TERMINATOR_H
#define TERMINATOR_H

#include "Handler.h"

class Terminator : public Handler
{
public:
    virtual bool handlerRequest(std::string& request);
    virtual void setSuccessor(Handler* /*successor*/);
    virtual Handler* getNextSuccessor();

    virtual bool decode(std::string& bufer);
};

#endif // TERMINATOR_H
