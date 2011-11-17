#include <node.h>
#include <v8.h>

using namespace v8;

namespace livestreaming {

Handle<Value> Hello(const Arguments& args) {
  HandleScope scope;
  return scope.Close(String::New("world"));
}

}

extern "C" void livestreaming_init(Handle<Object> target) {
  NODE_SET_METHOD(target, "hello", livestreaming::Hello);
}

NODE_MODULE(livestreaming, livestreaming_init);
