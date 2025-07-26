#include "Arduino.h"
#include <rom/crc.h>
#include "DrefProvider.h"

bool DrefProvider::begin(uint16_t port)
{
  udp.onPacket([this](AsyncUDPPacket packet)
               { this->handlePacket(packet); });

  return udp.listen(port);
}

// Добавление слушателя для конкретного DataRef
void DrefProvider::addListener(const char *dataRefPath, std::function<void(float)> callback)
{
  uint32_t hash = crc32_le(0, (const uint8_t *)dataRefPath, strlen(dataRefPath));
  listeners[hash].push_back(callback);
}

// Удаление слушателя для конкретного DataRef
void DrefProvider::removeListener(const char *dataRefPath, std::function<void(float)> callback)
{
  uint32_t hash = crc32_le(0, (const uint8_t *)dataRefPath, strlen(dataRefPath));

  auto it = listeners.find(hash);
  if (it == listeners.end())
    return;

  auto &callbacks = it->second;
  for (auto cb_it = callbacks.begin(); cb_it != callbacks.end(); ++cb_it)
  {
    // Note: Сравнение std::function напрямую может не работать корректно
    // В реальных приложениях лучше использовать токены или ID
    callbacks.erase(cb_it);
    break;
  }

  // Если нет больше слушателей, удаляем запись
  if (callbacks.empty())
    listeners.erase(it);
}

void DrefProvider::handlePacket(AsyncUDPPacket packet)
{
  uint8_t *data = packet.data();
  size_t len = packet.length();

  if (len < 9)
    return;

  if (data[0] != 'D' || data[1] != 'R' || data[2] != 'E' || data[3] != 'F')
    return;

  float value;
  memcpy(&value, &data[5], sizeof(float));

  size_t path_start = 9;
  size_t path_end = path_start;

  while (path_end < len && data[path_end] != 0x00)
    path_end++;

  len = path_end - path_start;

  if (len == 0)
    return;

  uint32_t hash = crc32_le(0, &data[path_start], len);

  counter += 1;

  std::lock_guard<std::mutex> lock(diff_mux);
  diff[hash] = value;
}

void DrefProvider::loop()
{
  std::map<uint32_t, float> _diff;

  {
    std::lock_guard<std::mutex> lock(diff_mux);

    if (diff.empty())
      return;

    _diff = std::move(diff);
    diff.clear();
  }

  for (const auto &entry : _diff)
  {

    auto it = listeners.find(entry.first);
    if (it == listeners.end())
      return;

    for (const auto &callback : it->second)
      callback(entry.second);
  }
}