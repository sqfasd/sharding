#ifndef SHARDING_H_
#define SHARDING_H_

#include <stdint.h>
#include <map>

#include <nan.h>

class Sharding : public Nan::ObjectWrap {
 public:
   static NAN_MODULE_INIT(Init);
 private:
  explicit Sharding(int32_t node_num, int32_t vnode_num, int32_t seed);
  ~Sharding();

  static NAN_METHOD(New);
  static NAN_METHOD(Shard);

  static Nan::Persistent<v8::Function> constructor;
  typedef std::map<uint64_t, int32_t> Map;
  Map mapping_;
  const int32_t node_num_;
  const int32_t vnode_num_;
  const int32_t seed_;
};

#endif  // SHARDING_H_
