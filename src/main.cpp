#include <include/arg_parser.h>
#include <include/service.h>

#include <cstdio>
#include <format>
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

  std::cout << std::format("Sending command to {} service...\n",
                           service->first);

  service->second.sendCommand(std::move(args));

  return 0;
}
