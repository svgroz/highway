#pragma once

#include <cstddef>

namespace higway {

using SizeData = struct {
  const std::size_t size;
  const std::byte *data;
};

class Message {
public:
  explicit Message() = default;
  Message(Message &) = delete;
  Message(Message &&) = default;
  virtual ~Message() noexcept = default;

  
};

class Exctractor {
public:
  explicit Exctractor() = default;
  Exctractor(Exctractor &) = delete;
  Exctractor(Exctractor &&) = default;
  virtual ~Exctractor() noexcept = default;
  virtual int f();
};
} // namespace higway