#ifndef SERVICE_H_
#define SERVICE_H_

#include <include/arg_parser.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct ServiceCommand {
  std::string cmdName;
  std::size_t nArgs;
};

using cmdVec = std::vector<ServiceCommand>;

class ServiceInterface {
  static constexpr size_t bufSize_ = 1024U;

 public:
  ServiceInterface(std::string serviceName,
                   const std::vector<ServiceCommand>& availCmds);
  void sendCommand(std::vector<argPair>&& requestVec);

 private:
  std::string requestToService(const char* json);
  std::string serviceName_;
  std::string sockPath_;
  const cmdVec& availCmds_;
  char buffer_[bufSize_];
};

static cmdVec readSensorsCmds = {
    {"track", 1U},
    {"untrack", 1U},
    {"register", 1U},
    {"unregister", 1U},
    {"set_measurement_period", 1U},
};

inline std::unordered_map<std::string, ServiceInterface> g_SupportedServices = {
    {"read-sensors", ServiceInterface{"read-sensors", readSensorsCmds}},
};

#endif
