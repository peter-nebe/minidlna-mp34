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

#ifndef PLAYERPAGE_H_
#define PLAYERPAGE_H_

#include "mp3Metadata.h"
#include <QtWebKitWidgets/QtWebKitWidgets>

class PlayerHtml : public QString
{
public:
  PlayerHtml();
};

class PlayerPage : QWebPage
{
public:
  PlayerPage(const QString &html);
  void setContent(const Mp3Metadata &metadata);
  void progress(size_t seconds, size_t secondsTotal);
  QString toHtml() const;
  QImage render() const;

private:
  void setContentAlternative(const Mp3Metadata &metadata);
};

#endif /* PLAYERPAGE_H_ */
