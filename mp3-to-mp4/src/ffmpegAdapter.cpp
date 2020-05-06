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
  MetadataPod mp{};
  get_mp3_metadata(&mp);

  if(mp.title)
    metadata.title = mp.title;
  if(mp.artist)
    metadata.artist = mp.artist;
  if(mp.album)
    metadata.album = mp.album;
  if(mp.track > 0)
    metadata.track = mp.track;
}
