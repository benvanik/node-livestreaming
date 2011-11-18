var Segmenter = require('./livestreaming/segmenter').Segmenter;

exports.createSegmenter = function(options) {
  return new Segmenter(options);
};
