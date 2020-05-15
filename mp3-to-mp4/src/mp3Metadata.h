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

#ifndef MP3METADATA_H_
#define MP3METADATA_H_

#include <string>
#include <vector>

struct Mp3Metadata
{
  size_t title{};
  size_t artist{};
  size_t album{};
  size_t track{};
  size_t duration{};
  std::string albumImageFile;
  std::vector<std::pair<std::string, std::string>> tags;
  std::vector<std::string> fileList;
  size_t currentFileIndex{};

  const std::string &getTitle() const
  {
    return tags[title].second;
  }
  const std::string &getArtist() const
  {
    return tags[artist].second;
  }
  const std::string &getAlbum() const
  {
    return tags[album].second;
  }
  const std::string &getTrack() const
  {
    return tags[track].second;
  }
};

#endif /* MP3METADATA_H_ */
