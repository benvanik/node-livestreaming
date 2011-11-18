import sys
import os

import json
package = json.load(open('package.json'))
NAME = package['name']
APPNAME = 'node-' + NAME
VERSION = package['version']

srcdir = 'src'
blddir = 'build'

def set_options(opt):
  opt.tool_options('compiler_cxx')
  opt.tool_options('node_addon')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')

  conf.check(header_name='libavformat/avformat.h', mandatory=True)
  conf.check(lib='avutil', uselib_store='LIBAVUTIL')
  conf.check(lib='avformat', uselib_store='LIBAVFORMAT')
  conf.check(lib='avcodec', uselib_store='LIBAVCODEC')

def build(bld):
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.target = 'livestreaming'
  obj.cxxflags = []
  obj.uselib = ['LIBAVUTIL', 'LIBAVFORMAT', 'LIBAVCODEC']
  obj.source = [
    'src/binding.cpp',
    'src/segmentercontext.cpp'
  ]
