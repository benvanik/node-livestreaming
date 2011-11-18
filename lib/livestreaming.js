var Segmenter = require('./livestreaming/segmenter').Segmenter;

exports.createSegmenter = function(source, opt_options) {
  return new Segmenter(source, opt_options);
};
