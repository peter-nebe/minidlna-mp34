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

#ifndef MAINARGS_H_
#define MAINARGS_H_

#include <vector>
#include <string>
#include <cstring>

class MainArgs
{
public:
  MainArgs(const std::vector<std::string> &args)
  {
    nargs = args.size() + 1;
    margs = new char*[nargs + 1];

    char **ma = margs;
    *ma++ = createNullTerminated("programNameDummy");
    for(const std::string &arg : args)
      *ma++ = createNullTerminated(arg);
    *ma = nullptr;
  }

  ~MainArgs()
  {
    for(int i = 0; i < nargs; i++)
      delete[] margs[i];
    delete[] margs;
  }

  int count()
  {
    return nargs;
  }

  operator char**()
  {
    return margs;
  }

private:
  int nargs = 0;
  char **margs = nullptr;

  char *createNullTerminated(const std::string &str)
  {
    char *nt = new char[str.size() + 1];
    return strcpy(nt, str.c_str());
  }

  MainArgs (const MainArgs &other) = delete;
  MainArgs (MainArgs &&other) = delete;
  MainArgs&
  operator= (const MainArgs &other) = delete;
  MainArgs&
  operator= (MainArgs &&other) = delete;
};

#endif /* MAINARGS_H_ */
