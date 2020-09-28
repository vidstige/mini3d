#!/bin/sh
#VIDEO_SIZE=506x253
VIDEO_SIZE=320x200
TARGET=$1
ffmpeg -v warning -f rawvideo -pixel_format rgb32 -framerate 30 -video_size $VIDEO_SIZE -i - $TARGET
