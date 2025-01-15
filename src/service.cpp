#include <fcntl.h>
#include <include/arg_parser.h>
#include <include/service.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <utility>
#include <vector>

#define MAX_RQ_LEN (1024U)

namespace fs = std::filesystem;
using json = nlohmann::json;

struct command_json {
  std::string service;
  std::string command;
  std::string args;
};

/* Defining JSON Serialization */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(command_json, service, command,
                                                args)

ServiceInterface::ServiceInterface(std::string serviceName,
                                   const std::vector<ServiceCommand> &availCmds)
    : serviceName_{serviceName}, availCmds_{availCmds} {
  sockPath_ = "/run/" + serviceName + "/" + serviceName + ".sock";
}

void ServiceInterface::sendCommand(std::vector<argPair> &&requestVec) {
  std::vector<argPair> args = std::move(requestVec);
  command_json cmd;

  for (const auto &arg : args) {
    switch (arg.first) {
      case 's':
        cmd.service = arg.second;
        break;
      case 'c':
        cmd.command = arg.second;
        break;
      case 'a':
        cmd.args = arg.second;
        break;
      default:
        std::cerr << "Error generating request structure -> ArgType: "
                  << arg.first << '\n';
        exit(1);
    }
  }

  json jsonObj = cmd;

  std::string jsonStr = nlohmann::to_string(jsonObj);

  if (g_ArgConf.verbose)
    std::cout << "[Verbose] Raw request to be sent: " << jsonStr << '\n';

  auto res = requestToService(jsonStr.c_str());

  std::cout << res;
}

std::string ServiceInterface::requestToService(const char *json) {
  int sockFd = socket(AF_UNIX, SOCK_STREAM, 0);
  ssize_t rqSize = std::strlen(json);
  ssize_t resSize;

  rqSize = (rqSize > MAX_RQ_LEN) ? MAX_RQ_LEN : rqSize;

  if (sockFd < 0) {
    std::cerr << "Could not get Service Socket fd!!\n";
    goto cleanup_error;
  }

  if (!fs::exists(sockPath_)) {
    std::cerr << "Service socket file does not exist!\n";
    goto cleanup_error;
  }

  sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  std::strncpy(addr.sun_path, sockPath_.c_str(), sizeof(addr.sun_path) - 1);

  if (connect(sockFd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) <
      0) {
    std::cerr << "Failed to connect to service socket!\n";
    goto cleanup_error;
  }

  if (send(sockFd, json, rqSize, 0U) != rqSize) {
    std::cerr << "Failed to send request to service socket!\n";
    goto cleanup_error;
  }

  /* Setup response timeout */
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 250000;
  if (setsockopt(sockFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) <
      0) {
    std::cerr << "Failed to set socket response timeout\n";
    goto cleanup_error;
  }

  resSize = recv(sockFd, buffer_, bufSize_ - 1, 0);

  if (resSize < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    std::cout << "Response from service timedout!\n";
    exit(0);
  }

  buffer_[resSize] = '\0';

  close(sockFd);

  return std::string{"Response from Service: " + std::string{buffer_} + '\n'};

cleanup_error:
  close(sockFd);
  std::cerr << "Failed to complete request! Exiting...\n";
  exit(1);
}
