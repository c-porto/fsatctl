#include <getopt.h>
#include <include/arg_parser.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>

const std::string_view USAGE =
    R"#(Usage: fsatctl [options] -s <service> -c <command> -a <arg1:arg2:arg3> 
        Options:
            -s      name of the service to send a command, in string format
            -c      command to be sent to service, in string format
            -a      arguments of said command, should be strings separate by `:` denoting a list of arguments
            -h      display this usage)#";

struct ProvidedArgs {
  unsigned service : 1;
  unsigned command : 1;
  unsigned args : 1;
};

std::vector<argPair> parseArgs(int argc, char **argv) {
  std::vector<argPair> args;
  ProvidedArgs pargs = {0};
  int opt = 1;
  char *arg = nullptr;

  while ((opt = getopt(argc, argv, "hvs:c:a:")) != -1) {
    switch (opt) {
      case 'h':
        std::cout << USAGE << std::endl;
        exit(0);
      case 's':
        arg = optarg;
        args.emplace_back(argPair{opt, arg});
        pargs.service = 1U;
        break;
      case 'c':
        arg = optarg;
        args.emplace_back(argPair{opt, arg});
        pargs.command = 1U;
        break;
      case 'a':
        arg = optarg;
        args.emplace_back(argPair{opt, arg});
        pargs.args = 1U;
        break;
      case 'v':
        g_ArgConf.verbose = true;
        break;
      default:
        std::cout << USAGE << std::endl;
        exit(1);
    }
  }

  if (!pargs.service) {
    std::cerr << "A service was not especified!" << std::endl;
    std::cout << USAGE << std::endl;
    exit(1);
  }

  if (!pargs.command) {
    std::cerr << "A command was not especified!" << std::endl;
    std::cout << USAGE << std::endl;
    exit(1);
  }

  return args;
}

void sortArgs(std::vector<argPair> &args) {
  std::sort(args.begin(), args.end(),
            [](argPair a, argPair b) { return a.first > b.first; });
}
