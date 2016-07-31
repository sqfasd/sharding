# Sharding

## Usage

```bash
$ npm install --save sharding
```

```javascript
var sharding = require('sharding');

var nodes = ['shard1', 'shard2', 'shard3'];
var s = new sharding.Sharding(nodes);
console.log('shard for user1 is [' + s.shard('user1') + ']');

// use sharding option

var option = {vnode_num: 500, seed: 20150503};
s = new sharding.Sharding(nodes, option);
console.log('shard for user1 is [' + s.shard('user1') + ']');
```

## Parameters

* default hash seed: 19861225
* default virtual nodes number: 100
* default hash algorithm: murmur2 64bit

** If your other service want to use the same sharding strategy, the
parameters above MUST be the same **

