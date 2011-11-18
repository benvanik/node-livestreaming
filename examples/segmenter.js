#!/usr/bin/env node

var fs = require('fs');
var livestreaming = require('livestreaming');
var path = require('path');

var opts = require('tav').set({
  duration: {
    note: 'Segment duration (seconds)',
    value: 10
  }
});

if (opts.args.length < 1) {
  console.log('no input file specified');
  return;
}
if (opts.args.length < 2) {
  console.log('no output path specified');
  return;
}

var inputFile = path.normalize(opts.args[0]);
if (!path.existsSync(inputFile)) {
  console.log('input file not found');
  return;
}

var outputPath = path.normalize(opts.args[1]);
if (!path.existsSync(outputPath)) {
  fs.mkdirSync(outputPath);
}
var outputName = path.basename(inputFile, path.extname(inputFile));
var outputFile = path.normalize(path.join(outputPath, outputName + '.m3u8'));

var segmenter = livestreaming.createSegmenter(inputFile, {
  duration: parseInt(opts['duration'])
});

segmenter.on('segment', function(id, path) {
  console.log('- segment(' + id + '): ' + path);
});

segmenter.on('error', function(err) {
  console.log('- error: ', err);
});

segmenter.on('end', function() {
  console.log('- ended');
});

console.log('segmenting input ' + inputFile + ' to ' + outputFile);

segmenter.segmentToPath(outputPath, outputName, function(err) {
  console.log('completed! err: ', err);
});
