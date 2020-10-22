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

#include "ffmpegAdapter.h"
#include "ffmpeg/ffmpeg_lib.h"
#include "mainArgs.h"
#include <sstream>
using namespace std;

FfmpegAdapter FfmpegAdapter::inst;

FfmpegAdapter::FfmpegAdapter ()
{
}

FfmpegAdapter::~FfmpegAdapter ()
{
  if(needsCleanup)
    do_ffmpeg_cleanup();
}

int FfmpegAdapter::invokeFfmpeg(const vector<string> &args)
{
  if(needsCleanup)
    do_ffmpeg_cleanup();
  needsCleanup = true;

  MainArgs mainArgs(args);

  return ffmpeg_main(mainArgs.count(), mainArgs);
}

size_t FfmpegAdapter::getAudioDuration()
{
  return get_audio_duration();
}

bool FfmpegAdapter::hasActualOutput()
{
  return get_nb_output_streams() > 0;
}

void FfmpegAdapter::getMp3Metadata(Mp3Metadata &metadata)
{
  MetadataTags mt{};
  get_mp3_metadata(&mt);

  metadata.tags.reserve(mt.tagsCount + 2);
  metadata.tags.push_back({});

  if(!mt.tagsCount)
    return;

  for(size_t i = 0; i < mt.tagsCount; i++)
  {
    metadata.tags.push_back(make_pair(mt.tags[i].key, mt.tags[i].value));

    const string &key = metadata.tags.back().first;
    const size_t ind = i + 1;

    if(key == "title")
      metadata.title = ind;
    else if(key == "artist")
      metadata.artist = ind;
    else if(key == "album")
      metadata.album = ind;
    else if(key == "track")
      metadata.track = ind;
  }

  free(mt.tags);
}

string ffmpegQuoted(const string &str)
{
  const char delim = '\'';
  ostringstream oss;

  oss << delim;
  for(char c : str)
  {
    if(c == delim)
      oss << "'\\'";
    oss << c;
  }
  oss << delim;

  return oss.str();
}
