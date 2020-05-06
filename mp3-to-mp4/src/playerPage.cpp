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

void PlayerPage::setContent(const Mp3Metadata &mm)
{
  QWebElement docElem = mainFrame()->documentElement();
  docElem.findFirst("#title").setInnerXml(QString::fromStdString(mm.title));
  docElem.findFirst("#artist").setInnerXml(QString::fromStdString(mm.artist));
  docElem.findFirst("#album").setInnerXml(QString::fromStdString(mm.album));
  if(mm.track)
    docElem.findFirst("#track").setInnerXml(QString::number(mm.track));
  docElem.findFirst("#file").setInnerXml(QString::fromStdString(mm.file));

  if(!mm.albumImageFile.empty())
  {
    docElem = docElem.findFirst("#albumImage");
    docElem.setAttribute("src", QString::fromStdString(mm.albumImageFile));
    docElem.setAttribute("class", "albumImage");
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
