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

#include "ffmpeg_lib.h"
#include "ffmpeg.h"

size_t get_audio_duration()
{
  for(int i = 0; i < nb_output_streams; i++)
  {
    OutputStream *ost = output_streams[i];
    if(ost->enc_ctx->codec_type == AVMEDIA_TYPE_AUDIO)
    {
      int64_t pts = av_stream_get_end_pts(ost->st);
      if(pts != AV_NOPTS_VALUE)
      {
        pts = av_rescale_q(pts, ost->st->time_base, AV_TIME_BASE_Q);
        return FFABS(pts) / AV_TIME_BASE;
      }
    }
  }
  return 0;
}

int get_nb_output_streams()
{
  return nb_output_streams;
}

void get_mp3_metadata(MetadataTags *mt)
{
  if(nb_input_files < 1 || input_files == NULL || input_files[0] == NULL || input_files[0]->ctx == NULL)
    return;
  AVFormatContext *fmt_ctx = input_files[0]->ctx;

  const int count = av_dict_count(fmt_ctx->metadata);
  if(count <= 0)
    return;
  mt->tags = malloc(count * sizeof *mt->tags);
  if(!mt->tags)
    return;
  mt->tagsCount = count;

  size_t i = 0;
  AVDictionaryEntry *tag = NULL;
  while((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
  {
    mt->tags[i].key = tag->key;
    mt->tags[i].value = tag->value;
    if(++i >= mt->tagsCount)
      break;
  }
}

void do_ffmpeg_cleanup()
{
  void exit_program_hooked(int ret);
  exit_program_hooked(0);

  nb_input_streams = 0;
  nb_input_files = 0;
  nb_output_streams = 0;
  nb_output_files = 0;
  nb_filtergraphs = 0;
}
