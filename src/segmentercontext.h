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

  static void PrepareTask(uv_async_t* handle, int status);
  static void PumpTask(uv_async_t* handle, int status);
  static void CompleteTask(uv_async_t* handle, int status);
  static void TaskHandleClose(uv_handle_t* handle);

  Handle<String> ErrorToString(int errnum);

private:
  Persistent<Object>    target;
  Persistent<Object>    options;

  Persistent<String>    sourcePath;
  Persistent<String>    outputPath;
  Persistent<String>    outputName;

  double                segmentDuration;

  Persistent<Function>  segmentFunc;
  Persistent<Function>  finishedFunc;

  bool                  taskCompleted;
  bool                  taskAborted;
};

}; // livestreaming

#endif // NODE_LIVESTREAMING_SEGMENTERCONTEXT
