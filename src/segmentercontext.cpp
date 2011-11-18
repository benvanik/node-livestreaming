/*
 * Basic flow:
 * PrepareTask():
 * - Open input
 * - Read input info
 * - Create output
 *   - Determine and add streams
 * - Create decoder
 * - PumpTask()
 * PumpTask():
 * - For each frame from decoder
 *   - Read frame
 *   - If frame timestamp over duration, allocate a new segment
 *     - Emit segment to js
 *   - Write frame to current segment
 * - If done, CompleteTask(), else PumpTask()
 * CompleteTask():
 * - Cleanup
 */

#include "segmentercontext.h"

extern "C" {
#include "libavformat/avformat.h"
}

using namespace livestreaming;
using namespace v8;

static Persistent<FunctionTemplate> SegmenterContext_ctor;
void SegmenterContext::Init(Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> ctor = FunctionTemplate::New(New);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(String::NewSymbol("SegmenterContext"));

  NODE_SET_PROTOTYPE_METHOD(ctor, "execute", Execute);
  NODE_SET_PROTOTYPE_METHOD(ctor, "abort", Abort);

  SegmenterContext_ctor = Persistent<FunctionTemplate>(ctor);
  target->Set(String::NewSymbol("SegmenterContext"),
      SegmenterContext_ctor->GetFunction());
}

Handle<Value> SegmenterContext::New(const Arguments& args)
{
  HandleScope scope;
  Local<Object> target = args[0]->ToObject();
  Local<Object> options = args[1]->ToObject();
  SegmenterContext* context = new SegmenterContext(target, options);
  context->Wrap(args.This());
  return args.This();
}

SegmenterContext::SegmenterContext(Handle<Object> target,
    Handle<Object> options) :
    taskCompleted(false), taskAborted(false) {
  HandleScope scope;

  this->target = Persistent<Object>::New(target);
  this->options = Persistent<Object>::New(options);

  this->segmentFunc = Persistent<Function>::New(
      target->Get(String::New("emitSegment_")).As<Function>());
  this->finishedFunc = Persistent<Function>::New(
      target->Get(String::New("emitFinished_")).As<Function>());
}

SegmenterContext::~SegmenterContext() {
}

#define LS_TASK_DISPATCH(context, name) \
  do { \
    uv_async_t* handle_##name = new uv_async_t; \
    handle_##name->data = context; \
    context->Ref(); \
    uv_async_init(uv_default_loop(), handle_##name, name); \
    uv_async_send(handle_##name); \
  } while(0);

Handle<Value> SegmenterContext::Execute(const Arguments& args) {
  HandleScope scope;
  SegmenterContext* context = ObjectWrap::Unwrap<SegmenterContext>(args.This());

  // Kickoff preparation stage
  context->taskCompleted = false;
  context->taskAborted = false;
  LS_TASK_DISPATCH(context, PrepareTask);

  return scope.Close(Undefined());
}

Handle<Value> SegmenterContext::Abort(const Arguments& args) {
  HandleScope scope;
  SegmenterContext* context = ObjectWrap::Unwrap<SegmenterContext>(args.This());

  if (!context->taskCompleted) {
    context->taskAborted = true;
  }

  return scope.Close(Undefined());
}

void SegmenterContext::PrepareTask(uv_async_t* handle, int status) {
  assert(status == 0);
  SegmenterContext* context = static_cast<SegmenterContext*>(handle->data);
  if (context->taskCompleted) {
    uv_close((uv_handle_t*)handle, TaskHandleClose);
    return;
  }
  if (context->taskAborted) {
    // TODO: abort
  }

  // TODO: open everything
  printf("prepare!\n");

  // Start first pump (will loop until done)
  LS_TASK_DISPATCH(context, PumpTask);

  uv_close((uv_handle_t*)handle, TaskHandleClose);
}

void SegmenterContext::PumpTask(uv_async_t* handle, int status) {
  assert(status == 0);
  SegmenterContext* context = static_cast<SegmenterContext*>(handle->data);
  if (context->taskCompleted) {
    uv_close((uv_handle_t*)handle, TaskHandleClose);
    return;
  }

  bool taskDone = false;

  // TODO: main loop (for a bit)
  do {
    if (context->taskAborted) {
      // TODO: abort
    }

    // Issue a segmentCompleted_ on the target
    context->segmentFunc->Call(context->target, 0, NULL);

    //
    printf("pump!\n");
    taskDone = true;

    break;
  } while(true);

  if (taskDone) {
    // Completed
    LS_TASK_DISPATCH(context, CompleteTask);
  } else {
    // Not done - keep pumping
    LS_TASK_DISPATCH(context, PumpTask);
  }

  uv_close((uv_handle_t*)handle, TaskHandleClose);
}

void SegmenterContext::CompleteTask(uv_async_t* handle, int status) {
  assert(status == 0);
  SegmenterContext* context = static_cast<SegmenterContext*>(handle->data);
  if (context->taskCompleted) {
    uv_close((uv_handle_t*)handle, TaskHandleClose);
    return;
  }

  // TODO: close off everything
  printf("complete!\n");

  // Issue finished_ on the target object
  context->finishedFunc->Call(context->target, 0, NULL);

  context->taskCompleted = true;
  uv_close((uv_handle_t*)handle, TaskHandleClose);
}

void SegmenterContext::TaskHandleClose(uv_handle_t* handle) {
  SegmenterContext* context = static_cast<SegmenterContext*>(handle->data);
  context->Unref();
  handle->data = NULL;
}

/*
int SegmenterContext::PrepareInput() {
  int ret = 0;
  AVFormatContext* ctx = NULL;
  AVFormatContext* octx = NULL;

  double secs;
  int bitrate;

  const char* inputPath = "/Users/noxa/test_videos/test.m4v";

  av_register_all();

  ctx = avformat_alloc_context();
  if (!ctx) {
    ret = AVERROR_NOMEM;
    goto CLEANUP;
  }

  ret = avformat_open_input(&ctx, inputPath, NULL, NULL);
  if (ret != 0) {
    goto CLEANUP;
  }

  ret = av_find_stream_info(ctx);
  if (ret != 0) {
    goto CLEANUP;
  }

  //
  printf("Hello!\n");

  av_dump_format(ctx, 0, NULL, 0);

  secs = ctx->duration / (double)AV_TIME_BASE;
  bitrate = ctx->bit_rate;
  printf("duration: %g, bitrate: %d\n", secs, bitrate);

  ret = 0;
CLEANUP:
  if (ctx) {
    avformat_free_context(ctx);
  }
  if (ret) {
    char buffer[256];
    av_strerror(ret, buffer, sizeof(buffer));
    printf("err %d: %s\b", ret, buffer);
  }
  return ret;
}
*/

Handle<String> SegmenterContext::ErrorToString(int errnum) {
  HandleScope scope;
  char buffer[256];
  if (av_strerror(errnum, buffer, sizeof(buffer)) == 0) {
    return scope.Close(String::New(buffer));
  } else {
    return scope.Close(String::New("Unknown error"));
  }
}
