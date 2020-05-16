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
4. Enter the following commands:
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
make install
cd ../..
make install
```
5. Run *post_install.sh* if it wasn't done automatically.
6. Follow the general configuration instructions for MiniDLNA.
7. If you prefer the player template alternative, rename the folders:
- ```playerTemplate``` to ```playerTemplate.default```
- ```playerTemplate (alternative)``` to ```playerTemplate```

###### Sample output from the media server:
[![alt text](sample.png "Sample output from the media server")](https://www.youtube.com/watch?v=HVl2U0VPAU0)
