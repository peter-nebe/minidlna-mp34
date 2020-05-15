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
#include "playerPage.h"

namespace
{

const size_t _width = 1920;
const size_t _height = 1080;
const size_t _bytesPerPixel = 4; // ARGB
const size_t _stopDummy = -1;

class QtApp
{
  int zero = 0;
  QApplication app{ zero, nullptr };
}; // class QtApp

} // namespace

Rendering Rendering::inst;

size_t getImageSize()
{
  return _width * _height * _bytesPerPixel;
}

size_t getStreamLength()
{
  return Rendering::instance().getStreamLength();
}

const ImageBits_t *getNextImage()
{
  return Rendering::instance().getNextImage();
}

void Piece::render(const Mp3Metadata &metadata)
{
  PlayerPage page{ PlayerHtml() };
  page.setContent(metadata);
  const size_t secondsTotal = streamLength - 1;

  for(size_t seconds = 0; seconds < streamLength; seconds++)
  {
    page.progress(seconds, secondsTotal);
    qApp->processEvents();
    imageFifo.push(page.render());
  }
}

const ImageBits_t *Piece::getNextImage()
{
  if(imageCount++ < streamLength)
    image = imageFifo.pop();

  return image.bits();
}

void Piece::reset()
{
  streamLength = 0;
  imageCount = 0;
  imageFifo.clear();
  image = QImage();
}

void Rendering::startRenderPiece(Mp3Metadata &&metadata)
{
  piece.reset();
  piece.streamLength = metadata.duration;
  jobFifo.push(std::forward<Mp3Metadata>(metadata));

  if(!thread.joinable())
    thread = std::thread(&Rendering::threadFunc, this);
}

void Rendering::getImageGeometry(size_t *width, size_t *height)
{
  *width = _width;
  *height = _height;
}

size_t Rendering::getStreamLength() const
{
  return piece.streamLength;
}

const ImageBits_t *Rendering::getNextImage()
{
  return piece.getNextImage();
}

void Rendering::cleanup()
{
  piece.reset();
  jobFifo.clear();
  jobFifo.push(Mp3Metadata{ .title = _stopDummy });

  if(thread.joinable())
    thread.join();
}

void Rendering::threadFunc()
{
  QtApp qtApp;

  while(true)
  {
    const Mp3Metadata metadata = jobFifo.pop();
    if(metadata.title == _stopDummy)
      break;

    piece.render(metadata);
  }
}
