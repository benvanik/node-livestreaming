var events = require('events');
var util = require('util');

var Segmenter = function(options) {
  events.EventEmitter.call(this);

  // {
  //   allowCaching: true,
  //   streaming: false,
  //   duration: 10
  // }
  this.options = JSON.parse(JSON.stringify(options));
};
util.inherits(Segmenter, events.EventEmitter);
exports.Segmenter = Segmenter;

Segmenter.prototype.addSource = function(source, info) {
  // info = {
  //   bitrate: number,     // bits/sec
  //   codecs: [string]     // ['avc1.4d001e"', ...]
  // }
};

Segmenter.prototype.addAudioSource = function(source, info) {
  // info = {
  //   default: boolean,
  //   autoselect: boolean,
  //   name: string,        // 'English', etc
  //   bitrate: number,     // bits/sec
  //   language: string,    // 'en', etc
  //   codecs: [string]     // ['mp4a.40.5', ...]
  // }
};

Segmenter.prototype.segmentToPath = function(path, name) {
  // write to path/, prefix with name-
};

Segmenter.prototype.abort = function() {
};
