node-livestreaming -- HTTP Live Streaming segmenter and manifest support for
node.js
====================================

node-livestreaming is a library that targets the [HTTP Live Streaming](
http://tools.ietf.org/html/draft-pantos-http-live-streaming-07) specification.
It contains a segmenter, enabling the segmentation of MPEG TS streams, as well
as a manifest generation utility for spitting out the proper playlists.

For more information on HTTP Live Streaming, see the following resources:

* [IETF Spec](http://tools.ietf.org/html/draft-pantos-http-live-streaming-07)
* [Apple Docs](http://developer.apple.com/library/ios/#documentation/networkinginternet/conceptual/streamingmediaguide/Introduction/Introduction.html)

Currently supported features:

* Nothing!

Coming soon (maybe):

* Everything!

## Quickstart

    npm install livestreaming
    node
    > var segmenter = require('livestreaming').createSegmenter('file.ts', 10);
    > segmenter.on('segment', function(segment) {
        // A segment has been added/manifest is updated
      });
    > segmenter.on('end', function() {
        // Done!
      });
    > segmenter.segmentToPath('stream/', 'manifest.m3u8', 'prefix-');

## Installation

With [npm](http://npmjs.org):

    npm install livestreaming

From source:

    cd ~
    git clone https://benvanik@github.com/benvanik/node-livestreaming.git
    npm link node-livestreaming/

### Dependencies

node-livestreaming requires `ffmpeg` and its libraries `avformat` and `avcodec`.
Make sure it's installed and on your path.

#### Source

    ./configure \
        --enable-gpl --enable-nonfree --enable-pthreads \
        --enable-libfaac --enable-libfaad --enable-libmp3lame \
        --enable-libx264
    sudo make install

#### Mac OS X

The easiest way to get ffmpeg is via [MacPorts](http://macports.org).
Install it if needed and run the following from the command line:

    sudo port install ffmpeg +gpl +lame +x264 +xvid

You may also need to add the MacPorts paths to your `~./profile`:

    export C_INCLUDE_PATH=$C_INCLUDE_PATH:/opt/local/include/
    export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/opt/local/include/
    export LIBRARY_PATH=$LIBRARY_PATH:/opt/local/lib/

#### FreeBSD

    sudo pkg_add ffmpeg

#### Linux

    sudo apt-get install ffmpeg

## API

### Segmenting Transport Streams
