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
    > var segmenter = require('livestreaming').createSegmenter();
    > segmenter.addSource('file.ts');
    > segmenter.on('segment', function(segment) {
        // A segment has been added/manifest is updated
      });
    > segmenter.on('end', function() {
        // Done!
      });
    > segmenter.segmentToPath('stream/', 'name');

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
options object that allows you to override the defaults:

    var segmenter = livestreaming.createSegmenter({
      // Enable clients to cache the segments
      allowCaching: true,
      // Whether the output is getting streamed or being processed offline
      streaming: false,
      // Segment duration, in seconds
      duration: 10
    });

Add a source stream to the segmenter. Currently this must be a file. It must
contain a video stream, and can optionally contain an audio stream.

    segmenter.addSource('file.ts', {
      // Bitrate (bits/sec) of the combined stream
      bitrate: 7680000,
      // Codecs used in the stream
      codecs: ['avc1.4d001e', 'mp4a.40.5']
    });

Add an audio stream to the segmenter. Use this when adding additional audio
sources (such as alternate languages).

    segmenter.addAudioSource('english.ts', {
      // Bitrate (bits/sec)
      bitrate: 65000,
      // Codecs used in the stream
      codecs: ['mp4a.40.5'],
      // Name of the stream (used in the client UI)
      name: 'English',
      // Language of the audio
      language: 'en',
      // Whether the audio stream should be used by default
      default: true,
      // Whether the client should autoselect this if the language matches
      autoselect: true
    });

When all input is setup, begin the segmenting process. All files will be placed
in the given path as they are generated. The names will be based on the given
base name.

    segmenter.segmentToPath('path/', 'name');

#### Events

##### Event: `segment`

Emitted when a segment has been generated and the manifest file has been
updated.

    segmenter.on('segment', function(segment) {
      // segment = 'path/name.0.ts'
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

    var segmenter = require('livestreaming').createSegmenter();
    segmenter.addSource('video.ts');
    segmenter.addAudioSource('audio-en.ts', {
      name: 'English',
      language: 'en',
      default: true,
      autoselect: true
    });
    segmenter.addAudioSource('audio-de.ts', {
      name: 'Deutsche',
      language: 'de',
      default: false,
      autoselect: true
    });
    segmenter.on('end', function(segment) {
      console.log('segment: ' + segment);
    });
    segmenter.on('end', function() {
      console.log('completed!');
    });
    segmenter.segmentToPath('path/', 'name');

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
