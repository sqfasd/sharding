#include "sharding.h"

#include <stdio.h>
#include <assert.h>

#include "hash.h"

using namespace v8;
using namespace node;

static const char* CLASS_NAME = "Sharding";
static const int VIRTUAL_NODE_NUM = 100;
static const uint32_t HASH_SEED = 19861225;

Nan::Persistent<Function> Sharding::constructor;

Sharding::Sharding(int32_t node_num, int32_t vnode_num, int32_t seed)
    : node_num_(node_num),
      vnode_num_(vnode_num),
      seed_(seed) {
  assert(node_num_ > 0);
  assert(vnode_num_ > 0);
  assert(seed_ > 0);
  for (int i = 0; i < node_num_; ++i) {
    for (int j = 0; j < vnode_num_; ++j) {
      char buf[20] = {0};
      int n = ::sprintf(buf, "%d:%d", i, j);
      mapping_[MurmurHash2_64(buf, n, seed_)] = i;
    }
  }
}

Sharding::~Sharding() {
}

NAN_MODULE_INIT(Sharding::Init) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New(CLASS_NAME).ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "shard", Shard);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New(CLASS_NAME).ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

NAN_METHOD(Sharding::New) {
  assert(info.IsConstructCall());
  assert(info.Length() >= 1);
  assert(info[0]->IsInt32());
  int node_num = info[0]->Int32Value();
  int vnode_num = VIRTUAL_NODE_NUM;
  int seed = HASH_SEED;
  if (info.Length() > 1 && !info[1]->IsUndefined()) {
    assert(info[1]->IsObject());
    Object* option = Object::Cast(*info[1]);

    Local<String> vnode_num_key = Nan::New("vnode_num").ToLocalChecked();
    if (option->Has(vnode_num_key)) {
      assert(option->Get(vnode_num_key)->IsInt32());
      vnode_num = option->Get(vnode_num_key)->Int32Value();
    }

    Local<String> seed_key = Nan::New("seed").ToLocalChecked();
    if (option->Has(seed_key)) {
      assert(option->Get(seed_key)->IsInt32());
      seed = option->Get(seed_key)->Int32Value();
    }
  }
  Sharding* obj = new Sharding(node_num, vnode_num, seed);
  obj->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(Sharding::Shard) {
  assert(info.Length() == 1);
  assert(info[0]->IsString());
  Sharding* obj = ObjectWrap::Unwrap<Sharding>(info.Holder());
  String::Utf8Value utf8_str(info[0]);
  uint64_t hash = MurmurHash2_64(*utf8_str, utf8_str.length(), obj->seed_);
  Map::const_iterator iter = obj->mapping_.lower_bound(hash);
  assert(iter != obj->mapping_.end());
  info.GetReturnValue().Set(Nan::New<Number>(iter->second));
}

void Initialize(Handle<Object> exports) {
  Sharding::Init(exports);
}

NODE_MODULE(sharding_, Initialize)
