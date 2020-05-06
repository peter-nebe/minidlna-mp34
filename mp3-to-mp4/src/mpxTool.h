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

#ifndef MPXTOOL_H_
#define MPXTOOL_H_

#include "mp3Metadata.h"
#include <vector>

class MpxTool
{
public:
  int convert(const std::string &input);

private:
  int convertFolder(const std::string &folder);
  int convertFile(const std::string &mp3File);
  Mp3Metadata extractMetadata(const std::string &mp3File) const;
  int convertToMp4(const std::string &mp3File);
  size_t getDuration(const std::string &mp3File) const;
  std::string extractAlbumImage(const std::string &mp3File) const;
  int concatenate(const std::string &outMp4File) const;

  std::vector<std::string> mp4Files;
};

#endif /* MPXTOOL_H_ */
