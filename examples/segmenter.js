#!/usr/bin/env node

var livestreaming = require('livestreaming');

var opts = require('tav').set({
  duration: {
    note: 'Segment duration (seconds)',
    value: 10
  },
  output_path: {
    note: 'Output path',
    value: '.'
  },
  output_name: {
    note: 'Output name',
    value: 'output'
  }
});

var segmenter = livestreaming.createSegmenter({
  duration: parseInt(opts['duration'])
});

for (var n = 0; n < opts.args.length; n++) {
  var arg = opts.args[n];
  segmenter.addSource(arg);
}

segmenter.on('segment', function(segment) {
  console.log('segment: ' + segment);
});

segmenter.on('end', function() {
  console.log('completed!');
  process.exit();
});

segmenter.segmentToPath(opts['output_path'], opts['output_name']);
