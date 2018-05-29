/**
 * @file Provides hardware-based CRC-32C calculation with software fallback
 *
 * @author Anand Suresh <anand.suresh@gmail.com>
 * @copyright Copyright (C) 2018-present Anand Suresh. All rights reserved.
 * @license MIT
 */

const Crc32C = require('bindings')('crc32c')
const stream = require('stream')
const util = require('util')

/**
 * Calculates the CRC32C for a stream
 *
 * @param {Number} [crc] The initial CRC, if any
 * @constructor
 */
function Crc32CStream (crc) {
  if (!(this instanceof Crc32CStream)) {
    return new Crc32CStream(crc)
  }

  if (crc == null) {
    this.crc = 0
  } else if (typeof crc !== 'number') {
    throw new TypeError(`crc MUST be a number; got ${typeof crc}`)
  } else {
    this.crc = crc
  }

  Crc32CStream.super_.call(this, {
    write: function (chunk, encoding, next) {
      var err

      try {
        this.crc = module.exports.calculate(chunk, this.crc)
      } catch (e) {
        err = e
      } finally {
        next(err, chunk)
      }
    }
  })
}
util.inherits(Crc32CStream, stream.Writable)

/**
 * Export the interface
 * @type {Object}
 */
module.exports = {
  fromStream: function (stream, crc) { return stream.pipe(new Crc32CStream(crc)) },
  calculate: Crc32C.hardware_support ? Crc32C.sse42_crc : Crc32C.table_crc
}

// for debugging/benchmarks
if (process.NODE_ENV !== 'production') {
  module.exports.hardware_support = Crc32C.hardware_support
  module.exports.sse42_crc = Crc32C.sse42_crc
  module.exports.table_crc = Crc32C.table_crc
}
