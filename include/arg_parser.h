#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <string>
#include <utility>
#include <vector>

using argPair = std::pair<char, std::string>;

std::vector<argPair> parseArgs(int argc, char **argv);

void sortArgs(std::vector<argPair> &args);

struct ArgConfig {
  bool verbose;
};

inline ArgConfig g_ArgConf = {
    .verbose = false,
};

#endif
