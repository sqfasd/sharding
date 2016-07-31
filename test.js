var assert = require('assert');
var sharding = require('./index');

function createSharding(node_num) {
  var arr = [];
  for (var i = 0; i < node_num; ++i) {
    arr.push(i);
  }
  return new sharding.Sharding(arr);
}

// normal usage
var nodes = ['shard1', 'shard2', 'shard3'];
var s0 = new sharding.Sharding(nodes);
console.log('shard for user1 is [' + s0.shard('user1') + ']');
console.log('shard for user2 is [' + s0.shard('user2') + ']');
console.log('shard for user3 is [' + s0.shard('user3') + ']');
console.log('shard for user4 is [' + s0.shard('user4') + ']');
console.log('shard for user5 is [' + s0.shard('user5') + ']');

// test sharding option usage
s0 = new sharding.Sharding(nodes, {vnode_num: 500, seed: 20150503});
console.log('after change vnode_num and seed: ');
console.log('shard for user1 is [' + s0.shard('user1') + ']');
console.log('shard for user2 is [' + s0.shard('user2') + ']');
console.log('shard for user3 is [' + s0.shard('user3') + ']');
console.log('shard for user4 is [' + s0.shard('user4') + ']');
console.log('shard for user5 is [' + s0.shard('user5') + ']');

// test hash miss rate after cluster expansion
var test_key_num = 100;

var previous_shards_num = 10;
var s1 = createSharding(previous_shards_num);
var shards1 = {};
for (var i = 0; i < test_key_num; ++i) {
  var user = 'user' + i;
  shards1[user] = s1.shard(user);
}

var current_shards_num = 11;
var s2 = createSharding(current_shards_num);
var shards2 = {};
for (var i = 0; i < test_key_num; ++i) {
  var user = 'user' + i;
  shards2[user] = s2.shard(user);
}

var total_miss = 0;
for (var i in shards2) {
  if (shards1[i] != shards2[i]) {
    ++total_miss;
  }
}

console.log('previous shards num = ' + previous_shards_num);
console.log('current shards num = ' + current_shards_num);
console.log('test key num = ' + test_key_num);
console.log('after cluster expansion, missed hash num = ' + total_miss);
var missed_percent = total_miss / test_key_num;
var expanded_percent = (current_shards_num - previous_shards_num) /
                       previous_shards_num;
assert(missed_percent < expanded_percent * 1.1, 'too many hash miss');
