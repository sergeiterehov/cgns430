#ifndef DREF_PROVIDER_H
#define DREF_PROVIDER_H

#include <AsyncUDP.h>
#include <map>
#include <vector>
#include <mutex>
#include <functional>

class DrefProvider
{
private:
  AsyncUDP udp;
  std::map<uint32_t, std::vector<std::function<void(float)>>> listeners;

public:
  uint32_t counter = 0;

  bool begin(uint16_t port = 49000);
  void loop();
  void addListener(const char *dataRefPath, std::function<void(float)> callback);
  void removeListener(const char *dataRefPath, std::function<void(float)> callback);

private:
  std::map<uint32_t, float> diff;
  std::mutex diff_mux;

  void handlePacket(AsyncUDPPacket packet);
};

#endif