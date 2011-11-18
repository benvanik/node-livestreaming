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

Handle<Value> livestreaming::SegmenterContext::Execute(const Arguments& args) {
  HandleScope scope;
  SegmenterContext* context = ObjectWrap::Unwrap<SegmenterContext>(args.This());

  //

  return scope.Close(Undefined());
}

Handle<Value> livestreaming::SegmenterContext::Abort(const Arguments& args) {
  HandleScope scope;
  SegmenterContext* context = ObjectWrap::Unwrap<SegmenterContext>(args.This());

  //

  return scope.Close(Undefined());
}
