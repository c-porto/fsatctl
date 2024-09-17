#ifndef ARG_PARSER_H_
#define ARG_PARSER_H_

#include <string>

#include "cmds.h"

struct Arg {
    std::string service;
    Command     cmd;
    void*       data;
};

class ArgParser {};

#endif
