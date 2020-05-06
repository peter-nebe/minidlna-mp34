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

#ifndef RENDERING_H_
#define RENDERING_H_

#include <stddef.h>

typedef void ImageBits_t;

#ifdef __cplusplus
extern "C" {
#endif

size_t getImageSize();
size_t getStreamLength();
const ImageBits_t *getNextImage();

#ifdef __cplusplus
} // extern "C"
#include "mp3Metadata.h"
#include "fifo.h"
#include "QtGui/qimage.h"
#include <thread>

class Piece
{
public:
  void render(const Mp3Metadata &metadata);
  const ImageBits_t *getNextImage();
  void reset();
  size_t streamLength = 0;

private:
  size_t imageCount = 0;
  Fifo<QImage> imageFifo;
  QImage image;
};

class Rendering
{
public:
  void startRenderPiece(Mp3Metadata &&metadata);
  static void getImageGeometry(size_t *width, size_t *height);
  size_t getStreamLength() const;
  const ImageBits_t *getNextImage();
  void cleanup();
  static Rendering &instance(){ return inst; }

private:
  Rendering(){}
  void threadFunc();

  static Rendering inst;
  Piece piece;
  Fifo<Mp3Metadata> jobFifo;
  std::thread thread;
};

#endif // __cplusplus
#endif // RENDERING_H_
