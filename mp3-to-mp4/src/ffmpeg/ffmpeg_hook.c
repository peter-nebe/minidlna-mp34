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

#include "rendering.h"
#include "ffmpeg.h"

const char *rawvideoUrlDummy = "rawvideoUrlDummy";
static size_t image_count = -1;

static int read_frame(void *opaque, uint8_t *buf, int buf_size)
{
  if(buf_size == getImageSize())
  {
    size_t image_count = (*(size_t*)opaque)++;
    if(image_count < getStreamLength())
    {
      memcpy(buf, getNextImage(), getImageSize());
      return buf_size;
    }
    return AVERROR_EOF;
  }

  return AVERROR_PATCHWELCOME;
}

static int64_t seek(void *unused0, int64_t unused1, int whence)
{
  if(whence == AVSEEK_SIZE)
    return (int64_t)getImageSize() * (int64_t)getStreamLength();

  return AVERROR_PATCHWELCOME;
}

int avformat_open_input_hooked(AVFormatContext **ps, const char *url, ff_const59 AVInputFormat *fmt, AVDictionary **options)
{
  if(strcmp(url, rawvideoUrlDummy) == 0)
  {
    uint8_t *avio_ctx_buffer = av_malloc(getImageSize());
    if(!avio_ctx_buffer) {
      print_error(url, AVERROR(ENOMEM));
      exit_program(1);
    }
    image_count = 0;
    AVIOContext *avio_ctx = avio_alloc_context(avio_ctx_buffer, getImageSize(), 0, &image_count, read_frame, 0, seek);
    if(!avio_ctx) {
      print_error(url, AVERROR(ENOMEM));
      exit_program(1);
    }
    (*ps)->pb = avio_ctx;
    url = 0;
  }

  return avformat_open_input(ps, url, fmt, options);
}

void exit_program_dummy(int ret){}

void exit_program(int ret)
{
  void exit_program_hooked(int ret);
  exit_program_hooked(ret);
  exit(ret);
}

void exit_hooked(int ret)
{
  if(ret)
    exit(ret);
//  fprintf(stderr, "Warning: exit(0) was called. It does not really exit now.\n");
}
