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
    > require('livestreaming').createSegmenter('file.ts').segmentToPath('stream/', 'name')

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

Create a segmenter with the factory method. You can optionally pass in an
options object that allows you to override the defaults. Currently the source
must be a file.

    var segmenter = livestreaming.createSegmenter('file.ts', {
      // Enable clients to cache the segments
      allowCaching: true,
      // Whether the output is getting streamed or being processed offline
      streaming: false,
      // Segment duration, in seconds
      duration: 10
    });

When all input is setup, begin the segmenting process. All files will be placed
in the given path as they are generated. The names will be based on the given
base name.

    segmenter.segmentToPath('path/', 'name', function(err) {
      // Done!
    });

#### Events

##### Event: `segment`

Emitted when a segment has been generated and the manifest file has been
updated.

    segmenter.on('segment', function(id, path) {
      // id = 123
      // path = 'path/name.123.ts'
    });

##### Event: `error`

Emitted when an error occurs during segmenting.

    segmenter.on('error', function(err) {
      //
    });

##### Event: `end`

Emitted when segmenting has completed.

    segmenter.on('end', function() {
      //
    });

#### Example

    var segmenter = require('livestreaming').createSegmenter('file.ts');
    segmenter.on('segment', function(segment) {
      console.log('segment: ' + segment);
    });
    segmenter.segmentToPath('path/', 'name', function(err) {
      // Done!
    });

    // Produces:
    // path/name.m3u8
    //     /name.0.ts
    //     /name.1.ts
    //     /...
    //     /name.audio.en.m3u8
    //     /name.audio.en.0.ts
    //     /name.audio.en.1.ts
    //     /...
    //     /name.audio.de.m3u8
    //     /name.audio.de.0.ts
    //     /name.audio.de.1.ts
    //     /...
