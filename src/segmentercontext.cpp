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
    executeCompleted(false) {
  HandleScope scope;

  this->target = Persistent<Object>(target);
  this->options = Persistent<Object>(options);
}

SegmenterContext::~SegmenterContext() {
}

static uv_async_t async1_handle;
static int async1_closed = 0;

Handle<Value> SegmenterContext::Execute(const Arguments& args) {
  HandleScope scope;
  SegmenterContext* context = ObjectWrap::Unwrap<SegmenterContext>(args.This());

  //
  context->executeHandle.data = context;
  uv_async_init(uv_default_loop(),
      &context->executeHandle, ExecuteCallback);

  uv_async_send(&context->executeHandle);

  return scope.Close(Undefined());
}

void SegmenterContext::ExecuteCallback(uv_async_t* handle, int status) {
  assert(status == 0);

  SegmenterContext* context = static_cast<SegmenterContext*>(handle->data);
  if (context->executeCompleted) {
    // May happen - sometimes the callback will be called twice...
    return;
  }

  context->PrepareInput();

  context->executeCompleted = true;
  uv_close((uv_handle_t*)handle, ExecuteHandleClose);
}

void SegmenterContext::ExecuteHandleClose(uv_handle_t* handle) {
  printf("handle cleanup %p\n", handle->data);
  handle->data = NULL;
}

Handle<Value> SegmenterContext::Abort(const Arguments& args) {
  HandleScope scope;
  SegmenterContext* context = ObjectWrap::Unwrap<SegmenterContext>(args.This());

  //
  if (!context->executeCompleted) {
    //uv_async_send(&context->executeHandle);
  }

  return scope.Close(Undefined());
}

Handle<String> SegmenterContext::ErrorToString(int errnum) {
  HandleScope scope;
  char buffer[256];
  if (av_strerror(errnum, buffer, sizeof(buffer)) == 0) {
    return scope.Close(String::New(buffer));
  } else {
    return scope.Close(String::New("Unknown error"));
  }
}

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

