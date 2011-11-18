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

  static void ExecuteCallback(uv_async_t* handle, int status);
  static void ExecuteHandleClose(uv_handle_t* handle);

  Handle<String> ErrorToString(int errnum);

  int PrepareInput();

private:
  Persistent<Object>  target;
  Persistent<Object>  options;

  uv_async_t          executeHandle;
  bool                executeCompleted;
};

}; // livestreaming

#endif // NODE_LIVESTREAMING_SEGMENTERCONTEXT
