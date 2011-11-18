#include "segmentercontext.h"

using namespace v8;

static Persistent<FunctionTemplate> SegmenterContext_ctor;
void livestreaming::SegmenterContext::Init(Handle<Object> target) {
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

Handle<Value> livestreaming::SegmenterContext::New(const Arguments& args)
{
  HandleScope scope;
  Local<Object> target = args[0]->ToObject();
  Local<Object> options = args[1]->ToObject();
  SegmenterContext* context = new SegmenterContext(target, options);
  context->Wrap(args.This());
  return args.This();
}

livestreaming::SegmenterContext::SegmenterContext(Handle<Object> target,
    Handle<Object> options) {
  HandleScope scope;

  this->target = Persistent<Object>(target);
  this->options = Persistent<Object>(options);
}

livestreaming::SegmenterContext::~SegmenterContext() {
}

static uv_async_t async1_handle;
static int async1_closed = 0;
static void close_cb(uv_handle_t* handle) {
  printf("handle cleanup %p\n", handle->data);
  handle->data = NULL;
}
static void async1_cb(uv_async_t* handle, int status) {
  //ASSERT(status == 0);
  if (async1_closed) {
    return;
  }

  printf("data: %p\n", handle->data);
  printf("async1_cb #%d\n", async1_cb_called);

  async1_closed = 1;
  uv_close((uv_handle_t*)handle, close_cb);
}

Handle<Value> livestreaming::SegmenterContext::Execute(const Arguments& args) {
  HandleScope scope;
  SegmenterContext* context = ObjectWrap::Unwrap<SegmenterContext>(args.This());

  //
  async1_handle.data = (void*)123;
  int r = uv_async_init(uv_default_loop(), &async1_handle, async1_cb);

  return scope.Close(Undefined());
}

Handle<Value> livestreaming::SegmenterContext::Abort(const Arguments& args) {
  HandleScope scope;
  SegmenterContext* context = ObjectWrap::Unwrap<SegmenterContext>(args.This());

  //
  uv_async_send(&async1_handle);

  return scope.Close(Undefined());
}
