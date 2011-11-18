#include <node.h>
#include <v8.h>
#include "utils.h"

#ifndef NODE_LIVESTREAMING_SEGMENTERCONTEXT
#define NODE_LIVESTREAMING_SEGMENTERCONTEXT

using namespace v8;

namespace livestreaming {

class SegmenterContext : public node::ObjectWrap {
public:
  static void Init(Handle<Object> target);
  static Handle<Value> New(const Arguments& args);

public:
  SegmenterContext(Handle<Object> target, Handle<Object> options);
  ~SegmenterContext();

private:
  static Handle<Value> Execute(const Arguments& args);
  static Handle<Value> Abort(const Arguments& args);

private:
  Persistent<Object> target;
  Persistent<Object> options;
};

}; // livestreaming

#endif // NODE_LIVESTREAMING_SEGMENTERCONTEXT
