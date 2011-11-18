var events = require('events');
var util = require('util');

var binding = require('../../build/Release/livestreaming.node');

function clone(obj) {
  return JSON.parse(JSON.stringify(obj));
};

var Segmenter = function(source, opt_options) {
  events.EventEmitter.call(this);

  this.source = source;

  // {
  //   allowCaching: true,
  //   streaming: false,
  //   duration: 10
  // }
  this.options = opt_options ? clone(opt_options) : {};
  if (this.options.allowCaching === undefined) {
    this.options.allowCaching = true;
  }
  if (this.options.streaming === undefined) {
    this.options.streaming = false;
  }
  if (this.options.duration === undefined) {
    this.options.duration = 10;
  }

  this.ctx_ = null;
  this.callback_ = null;
};
util.inherits(Segmenter, events.EventEmitter);
exports.Segmenter = Segmenter;

// Called from binding
Segmenter.prototype.emitSegment_ = function(id, path) {
  console.log('segment completed: ' + id + ' at ' + path);
  this.emit('segment', path);
};

// Called from binding
Segmenter.prototype.emitFinished_ = function(err) {
  if (err) {
    this.emit('error', err);
  }
  this.emit('end');

  this.ctx_ = null;

  if (this.callback_) {
    var callback = this.callback_;
    this.callback_ = null;
    callback(err);
  }
};

Segmenter.prototype.segmentToPath = function(path, name, opt_callback) {
  if (this.ctx_) {
    throw new Error('Already executing');
  }

  this.callback_ = opt_callback;

  this.ctx_ = new binding.SegmenterContext(this, {
    path: path,
    name: name,
    options: this.options_,
    sources: this.sources_
  });

  this.ctx_.execute();

  // var self = this;
  // setTimeout(function() {
  //   self.abort();
  // }, 100);
};

Segmenter.prototype.abort = function() {
  if (this.ctx_) {
    this.ctx_.abort();
  }
};
