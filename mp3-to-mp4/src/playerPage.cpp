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

#include "playerPage.h"
#include "rendering.h"
#include "config_mp34.h"
#include <map>

namespace
{

const QString playerHtml = "player.html";
const QString playerTemplate = "/playerTemplate/";

QString getTemplatePath()
{
  static QString templatePath;
  if(templatePath.isEmpty())
    templatePath = DATA_PATH + playerTemplate;

  return templatePath;
} // getTemplatePath

} // namespace

PlayerHtml::PlayerHtml()
{
  QFile file(getTemplatePath() + playerHtml);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  QString::operator=(file.readAll());
}

PlayerPage::PlayerPage(const QString &html)
{
  mainFrame()->setHtml(html, QUrl::fromLocalFile(getTemplatePath()));

  size_t width, height;
  Rendering::getImageGeometry(&width, &height);
  setViewportSize(QSize(width, height));
}

namespace
{
  void setElementContent(QWebElement &element, const std::string &content)
  {
    element.setInnerXml(QString::fromStdString(content));
  }
  void setElementContent(const QWebElement &documentElement, const char *selector, const std::string &content)
  {
    documentElement.findFirst(selector).setInnerXml(QString::fromStdString(content));
  }
} // namespace

void PlayerPage::setContent(const Mp3Metadata &mm)
{
  const QWebElement docElem = mainFrame()->documentElement();
  setElementContent(docElem, "#title", mm.getTitle());

  if(!mm.albumImageFile.empty())
  {
    QWebElement elem = docElem.findFirst("#albumImage");
    elem.setAttribute("src", QString::fromStdString(mm.albumImageFile));
    elem.setAttribute("class", "albumImage");
  }

  QWebElement elem = docElem.findFirst("#artist");
  if(elem.isNull())
  {
    // player template alternative
    setContentAlternative(mm);
    return;
  }

  setElementContent(elem, mm.getArtist());
  setElementContent(docElem, "#album", mm.getAlbum());
  setElementContent(docElem, "#track", mm.getTrack());
  setElementContent(docElem, "#file", mm.fileList[mm.currentFileIndex]);
}

namespace
{

enum class TagId
{
  title,
  artist,
  album,
  track,
  albumArtist,
  disc,
  composer,
  performer,
  date,
  genre,
  language,
  filename,
  creationTime,
  publisher,
  copyright,
  serviceName,
  serviceProvider,
  encodedBy,
  encoder,
  variantBitrate,
  comment,
  tagIdMax
}; // TagId

const char *const designation[]
{
  "Title:",
  "Artist:",
  "Album:",
  "Track:",
  "Album Artist:",
  "Disc:",
  "Composer:",
  "Performer:",
  "Date:",
  "Genre:",
  "Language:",
  "Filename:",
  "Creation Time:",
  "Publisher:",
  "Copyright:",
  "Service Name:",
  "Service Provider:",
  "Encoded By:",
  "Encoder:",
  "Variant Bitrate:",
  "Comment:"
}; // designation

} // namespace

void PlayerPage::setContentAlternative(const Mp3Metadata &mm)
{
  static const std::map<std::string, TagId> tagNames
  {
    { "album", TagId::album },
    { "album_artist", TagId::albumArtist },
    { "artist", TagId::artist },
    { "comment", TagId::comment },
    { "composer", TagId::composer },
    { "copyright", TagId::copyright },
    { "creation_time", TagId::creationTime },
    { "date", TagId::date },
    { "disc", TagId::disc },
    { "encoded_by", TagId::encodedBy },
    { "encoder", TagId::encoder },
    { "filename", TagId::filename },
    { "genre", TagId::genre },
    { "language", TagId::language },
    { "performer", TagId::performer },
    { "publisher", TagId::publisher },
    { "service_name", TagId::serviceName },
    { "service_provider", TagId::serviceProvider },
    { "title", TagId::title },
    { "track", TagId::track },
    { "variant_bitrate", TagId::variantBitrate }
  }; // tagNames

  std::map<TagId, std::string> tagsToDisplay;
  for(const auto &tag : mm.tags)
  {
    const auto tn = tagNames.find(tag.first);
    if(tn != tagNames.end())
      tagsToDisplay[tn->second] = tag.second;
  }
  tagsToDisplay.erase(TagId::title); // title was already set

  const QWebElement docElem = mainFrame()->documentElement();
  QWebElement elem = docElem.findFirst(".desig");

  static_assert(std::size(designation) == static_cast<size_t>(TagId::tagIdMax));
  for(const auto &tag : tagsToDisplay)
  {
    elem.setInnerXml(designation[static_cast<size_t>(tag.first)]);
    elem = elem.nextSibling();
    setElementContent(elem, tag.second);
    elem = elem.nextSibling();
  }

  elem = docElem.findFirst("#fileList").firstChild();
  for(size_t i = 0; i < mm.fileList.size(); i++)
  {
    setElementContent(elem, mm.fileList[i]);
    if(i == mm.currentFileIndex)
      elem.setAttribute("class", "currentFile");
    elem = elem.nextSibling();
  }
}

void PlayerPage::progress(size_t seconds, size_t secondsTotal)
{
  const auto toString = [](size_t secs)
  {
    return QTime(0, 0).addSecs(secs).toString(secs < 3600 ? "m:ss" : "h:mm:ss");
  };
  const QString timeElapsed = toString(seconds);
  const QString timeTotal = toString(secondsTotal);
  const QString progressVal = QString::number(static_cast<double>(seconds) / secondsTotal);

  QWebElement docElem = mainFrame()->documentElement();
  docElem.findFirst("#timeElapsed").setInnerXml(timeElapsed);
  docElem.findFirst("#timeTotal").setInnerXml(timeTotal);
  docElem.findFirst("progress").setAttribute("value", progressVal);
}

QString PlayerPage::toHtml() const
{
  return mainFrame()->toHtml();
}

QImage PlayerPage::render() const
{
  QImage image(viewportSize(), QImage::Format_ARGB32_Premultiplied);
  QPainter painter(&image);
  mainFrame()->render(&painter);
  painter.end();

  return image;
}
