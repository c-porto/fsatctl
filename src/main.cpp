#include <include/arg_parser.h>
#include <include/service.h>

#include <cstdio>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  std::vector<argPair> args = parseArgs(argc, argv);

  sortArgs(args);

  auto service{g_SupportedServices.find(args[0].second)};

  if (service == g_SupportedServices.end()) {
    std::cerr << "Unsupported service!!\n";
    exit(1);
  }

  std::cout << "Sending command to " << service->first << "service...\n";

  service->second.sendCommand(std::move(args));

  return 0;
}
