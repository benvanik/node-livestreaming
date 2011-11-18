#include <node.h>
#include <v8.h>
#include "utils.h"
#include "segmentercontext.h"

using namespace v8;

extern "C" void livestreaming_init(Handle<Object> target) {
  livestreaming::SegmenterContext::Init(target);
}

NODE_MODULE(livestreaming, livestreaming_init);
