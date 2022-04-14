// Copyright 2021 The Khronos Group
// SPDX-License-Identifier: Apache-2.0

#pragma once

// std
#include <chrono>
#include <future>
#include <memory>


namespace generic {

class Future
{
  public:
  inline Future()
  {
    m_future = m_promise.get_future();
  }
  ~Future() = default;


inline void wait() const
{
  if (m_future.valid())
    m_future.wait();
}

inline bool isReady() const
{
  return m_future.wait_for(std::chrono::seconds(0))
      == std::future_status::ready;
}

inline void markComplete()
{
  m_promise.set_value();
}

 private:
  std::promise<void> m_promise;
  std::future<void> m_future;
};

using FuturePtr = std::shared_ptr<Future>;

// Inlined definitions ////////////////////////////////////////////////////////



} 