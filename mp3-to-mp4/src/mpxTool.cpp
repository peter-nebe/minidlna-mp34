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

#include "mpxTool.h"
#include "log.h"
#include "rendering.h"
#include "ffmpegAdapter.h"
#include "ffmpeg/ffmpeg_lib.h" // rawvideoUrlDummy
#include <filesystem>
#include <sstream>
#include <fstream>
using namespace std;
namespace fs = std::filesystem;

int MpxTool::convert(const string &input)
{
  if(!fs::exists(input))
  {
    logerror << quoted(input) << " does not exist" << endl;
    return 1;
  }

  currentMp3FileIndex = 0;
  mp3Files.clear();

  int ret = 0;
  if(fs::is_directory(input))
  {
    ret = convertFolder(input);
  }
  else
  {
    mp3Files.insert(input);
    ret = convertFile(input);
  }
  Rendering::instance().cleanup();

  return ret;
}

int MpxTool::convertFolder(const string &folder)
{
  int retval = 0;
  loginfo << "converting folder " << quoted(folder) << "..." << endl;

  for(const auto &entry : fs::directory_iterator(folder))
  {
    if(entry.path().extension() == ".mp3")
      mp3Files.insert(entry.path());
  }

  mp4Files.clear();
  mp4Files.reserve(mp3Files.size());

  for(const auto &file : mp3Files)
  {
    const int err = convertFile(file);
    if(err)
      retval = err;
    ++currentMp3FileIndex;
  }

  fs::path folderPath(folder);
  fs::path::iterator last = --folderPath.end();
  if(folder.back() == '/') // If there is a directory separator after the last file-name in the path,
    --last;                // the last element before the end iterator is an empty element.
                           // (see https://en.cppreference.com/w/cpp/filesystem/path/begin, "4)" )

  const int err = concatenate(last->string() + ".mp4");
  if(err)
    retval = err;

  loginfo << "folder completed" << endl;

  return retval;
}

int MpxTool::convertFile(const string &mp3File)
{
  loginfo << "converting file " << quoted(mp3File) << "..." << endl;

  Mp3Metadata metadata = extractMetadata(mp3File);

  metadata.fileList.reserve(mp3Files.size());
  for(const auto &file : mp3Files)
    metadata.fileList.push_back(fs::path(file).filename());
  metadata.currentFileIndex = currentMp3FileIndex;

  Rendering::instance().startRenderPiece(move(metadata));

  const int err = convertToMp4(mp3File);
  loginfo << "file completed" << endl;

  return err;
}

Mp3Metadata MpxTool::extractMetadata(const string &mp3File) const
{
  Mp3Metadata mm;
  mm.duration = getDuration(mp3File);

  const string albumImageFile = extractAlbumImage(mp3File);
  if(!albumImageFile.empty())
    mm.albumImageFile = "file://"s.append(fs::current_path() / albumImageFile);

  FfmpegAdapter::getMp3Metadata(mm);

  if(!mm.title)
  {
    mm.title = mm.tags.size();
    mm.tags.push_back(make_pair("title", fs::path(mp3File).stem()));
  }

  return mm;
}

int MpxTool::convertToMp4(const string &mp3File)
{
  size_t width, height;
  Rendering::getImageGeometry(&width, &height);
  ostringstream imageGeom;
  imageGeom << width << 'x' << height;

  mp4Files.push_back(fs::path(mp3File).filename().replace_extension("mp4"));
  const vector<string> args
  {
    "-f", "rawvideo", "-s", imageGeom.str(), "-pix_fmt", "bgra", "-r", "1", "-i", rawvideoUrlDummy,
    "-i", mp3File,
    "-c:v", "libx264", "-pix_fmt", "yuv420p", "-y", mp4Files.back()
  };

  return FfmpegAdapter::instance().invokeFfmpeg(args);
}

size_t MpxTool::getDuration(const string &mp3File) const
{
  const vector<string> args
  {
    "-v", "quiet", "-i", mp3File,
    "-vn", "-f", "null", "-"
  };
  const int error = FfmpegAdapter::instance().invokeFfmpeg(args);
  if(error)
    return 0;

  return FfmpegAdapter::getAudioDuration();
}

std::string MpxTool::extractAlbumImage(const string &mp3File) const
{
  const string jpgFile = fs::path(mp3File).filename().replace_extension("jpg");
  const vector<string> args
  {
    "-i", mp3File,
    "-an", "-c:v", "copy", "-y", jpgFile
  };

  return (FfmpegAdapter::instance().invokeFfmpeg(args) == 0 && FfmpegAdapter::hasActualOutput()) ? jpgFile : string();
}

int MpxTool::concatenate(const string &outMp4File) const
{
  const string concatListName = "concatList";

  ofstream concatList(concatListName);
  for(const auto &file : mp4Files)
    concatList << "file '" << file << "'" << endl;
  concatList.close();

  const vector<string> args
  {
    "-f", "concat", "-safe", "0",
    "-i", concatListName,
    "-c", "copy", "-y", outMp4File
  };

  return FfmpegAdapter::instance().invokeFfmpeg(args);
}
