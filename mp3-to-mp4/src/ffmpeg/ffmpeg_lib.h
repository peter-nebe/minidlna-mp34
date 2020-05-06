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

#ifndef FFMPEG_LIB_H_
#define FFMPEG_LIB_H_

#include <stddef.h>

extern const char *rawvideoUrlDummy;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MetadataPod
{
  char *title;
  char *artist;
  char *album;
  int track;
} MetadataPod;

int ffmpeg_main(int argc, char **argv);
size_t get_audio_duration();
int get_nb_output_streams();
void get_mp3_metadata(MetadataPod *mp);
void do_ffmpeg_cleanup();

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // FFMPEG_LIB_H_
