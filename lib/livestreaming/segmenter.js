var events = require('events');
var util = require('util');

var binding = require('../../build/Release/livestreaming.node');

function clone(obj) {
  return JSON.parse(JSON.stringify(obj));
};

var Segmenter = function(options) {
  events.EventEmitter.call(this);

  // {
  //   allowCaching: true,
  //   streaming: false,
  //   duration: 10
  // }
  this.options = options ? clone(options) : {};
  if (this.options.allowCaching === undefined) {
    this.options.allowCaching = true;
  }
  if (this.options.streaming === undefined) {
    this.options.streaming = false;
  }
  if (this.options.duration === undefined) {
    this.options.duration = 10;
  }

  this.sources_ = [];

  this.ctx_ = null;
};
util.inherits(Segmenter, events.EventEmitter);
exports.Segmenter = Segmenter;

Segmenter.prototype.addSource = function(source, info) {
  if (this.ctx_) {
    throw new Error('Cannot change the segmenter while it is running');
  }

  // info = {
  //   bitrate: number,     // bits/sec
  //   codecs: [string]     // ['avc1.4d001e"', ...]
  // }
  this.sources_.push({
    type: 'default',
    info: info ? clone(info) : {}
  });
};

Segmenter.prototype.addAudioSource = function(source, info) {
  if (this.ctx_) {
    throw new Error('Cannot change the segmenter while it is running');
  }

  // info = {
  //   default: boolean,
  //   autoselect: boolean,
  //   name: string,        // 'English', etc
  //   bitrate: number,     // bits/sec
  //   language: string,    // 'en', etc
  //   codecs: [string]     // ['mp4a.40.5', ...]
  // }
  this.sources_.push({
    type: 'audio',
    info: info ? clone(info) : {}
  });
};

Segmenter.prototype.segmentToPath = function(path, name) {
  if (this.ctx_) {
    throw new Error('Already executing');
  }

  this.ctx_ = new binding.SegmenterContext(this, {
    path: path,
    name: name,
    options: this.options_,
    sources: this.sources_
  });
  this.ctx_.execute();
};

Segmenter.prototype.abort = function() {
  if (this.ctx_) {
    this.ctx_.abort();
  }
};
