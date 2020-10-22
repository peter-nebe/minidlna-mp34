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

#ifndef FFMPEGADAPTER_H_
#define FFMPEGADAPTER_H_

#include "mp3Metadata.h"
#include <vector>

class FfmpegAdapter
{
public:
  int invokeFfmpeg(const std::vector<std::string> &args);
  static size_t getAudioDuration();
  static bool hasActualOutput();
  static void getMp3Metadata(Mp3Metadata &metadata);
  static FfmpegAdapter &instance(){ return inst; }

private:
  FfmpegAdapter ();
  ~FfmpegAdapter ();
  FfmpegAdapter (const FfmpegAdapter &other) = delete;
  FfmpegAdapter (FfmpegAdapter &&other) = delete;
  FfmpegAdapter&
  operator= (const FfmpegAdapter &other) = delete;
  FfmpegAdapter&
  operator= (FfmpegAdapter &&other) = delete;

  bool needsCleanup = false;
  static FfmpegAdapter inst;
};

std::string ffmpegQuoted(const std::string &str);

#endif /* FFMPEGADAPTER_H_ */
