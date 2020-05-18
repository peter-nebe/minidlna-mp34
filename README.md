# minidlna-mp34
Extended MiniDLNA server

### Dependencies
- QtWebKit 5.212
- FFmpeg 4.2.2

### Additional sources
- [MiniDLNA/ReadyMedia 1.2.1](https://downloads.sourceforge.net/project/minidlna/minidlna/1.2.1/minidlna-1.2.1.tar.gz)
- [FFmpeg 4.2.2](https://ffmpeg.org/releases/ffmpeg-4.2.2.tar.bz2)

### Building instructions
Install **minidlna-mp34** from the [AUR](https://aur.archlinux.org/packages/minidlna-mp34) with Pamac, or do it manually in the following steps:
1. Install the dependencies. The libraries and the header files are required.
2. Download and extract the additional sources.
3. Clone this repository. Now you should have a folder with the following subfolders:
   ```
   ffmpeg-4.2.2
   minidlna-1.2.1
   minidlna-mp34
   ```
4. Depending on your system, some paths may need to be adjusted. Here are **examples** of the paths in question:
   - in script *mp3-to-mp4/src/CMakeLists.txt*:\
   ```/usr/include/x86_64-linux-gnu/qt5```
   - in script *minidlna-mp34.service*:\
   ```ExecStart=/usr/local/bin/minidlna-mp34 -S -f /usr/local/etc/minidlna-mp34/minidlna-mp34.conf ...```
   - in script *post_install.sh*:\
   ```systemctl enable /usr/local/etc/systemd/system/minidlna-mp34.service```
5. Now enter the following commands:
   ```
   cd ffmpeg-4.2.2
   ./configure
   cd ../minidlna-1.2.1
   ./configure
   cd ../minidlna-mp34
   cmake .
   make
   cd mp3-to-mp4/src
   cmake .
   make
   sudo make install
   cd ../..
   sudo make install
   ```
6. Run *post_install.sh* if it wasn't done automatically.
7. Follow the general configuration instructions for MiniDLNA.

If you prefer the player template alternative, rename the folders:
- ```playerTemplate``` to ```playerTemplate.default```
- ```playerTemplate (alternative)``` to ```playerTemplate```

###### Sample output from the media server:
[![alt text](sample.png "Sample output from the media server")](https://www.youtube.com/watch?v=HVl2U0VPAU0)
