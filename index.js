var internal = require('./build/Release/sharding_');

module.exports = {
  Sharding: Sharding,
}

function isArray(a) {
  return a instanceof Array;
}

function isString(s) {
  return (typeof s === 'string') || (s instanceof string);
}

function isObject(o) {
  return typeof o === 'object';
}

function Sharding(nodes, option) {
  if (!isArray(nodes) || nodes.length === 0 ||
      (option && !isObject(option))) {
    throw new Error('invalid parameters');
  }
  this._nodes = nodes;
  this._option = option;
  this._sharding = new internal.Sharding(this._nodes.length, this._option);
}

Sharding.prototype.shard = function(key) {
  if (!isString(key)) {
    throw new Error('invalid parameters');
  }
  return this._nodes[this._sharding.shard(key)];
}

Sharding.prototype.option = function() {
  return this._option;
}
