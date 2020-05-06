/*
 * mp3-to-mp4 converter
 * Copyright (c) 2020 Peter Nebe (mail@peter-nebe.dev)
 *
 * This file is part of mp3-to-mp4.
 *
 * mp3-to-mp4 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mp3-to-mp4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mp3-to-mp4.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SRC_FIFO_H_
#define SRC_FIFO_H_

#include <array>
#include <mutex>
#include <condition_variable>

template<typename T>
class Fifo
{
public:
  void push(T &&item)
  {
    std::unique_lock<std::mutex> lock(mutex);
    notFull.wait(lock, [this]{ return numItems < container.size(); });

    container[pushPos] = std::forward<T>(item);
    increment(pushPos);
    ++numItems;

    lock.unlock();
    notEmpty.notify_one();
  }

  T pop()
  {
    std::unique_lock<std::mutex> lock(mutex);
    notEmpty.wait(lock, [this]{ return numItems > 0; });

    T item = std::move(container[popPos]);
    increment(popPos);
    --numItems;

    lock.unlock();
    notFull.notify_one();

    return item;
  }

  void clear()
  {
    std::unique_lock<std::mutex> lock(mutex);
    while(numItems > 0)
    {
      T item = std::move(container[popPos]);
      increment(popPos);
      --numItems;
    }
    pushPos = 0;
    popPos = 0;
  }

private:
  void increment(size_t &pos)
  {
    if(++pos == container.size())
      pos = 0;
  }

  size_t pushPos = 0;
  size_t popPos = 0;
  size_t numItems = 0;
  std::array<T, 10> container;
  std::mutex mutex;
  std::condition_variable notEmpty;
  std::condition_variable notFull;
};

#endif /* SRC_FIFO_H_ */
