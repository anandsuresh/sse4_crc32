/**
 * @file Benchmarks using multiple random-length buffers
 *
 * @author Anand Suresh <anand.suresh@gmail.com>
 * @copyright Copyright (C) 2018-present Anand Suresh. All rights reserved.
 * @license MIT
 */

const Crc32C = require('..')
const jsCrc32 = require('crc32')

/**
 * A list of pre-generated buffers for the benchmark
 *
 * The use of pre-generated buffers as opposed to strings is to eliminate the
 * possibility of the interference from the garbage-collector during the test.
 *
 * @type {Buffer[]}
 */
const BUFFERS = []

/**
 * Initializes a list of randomly-sized buffers
 * @param {Object} benchmark Reference to the benchmark being executed
 */
function initBuffers (benchmark) {
  for (i = 0; i < benchmark.iterations; i++) {
    const len = Math.random() * 65536
    const buf = Buffer.alloc(len)

    let j = 0
    do {
      let data = Math.random().toString(36).slice(2)
      let size = j + data.length > len ? len - j : data.length
      buf.write(data, j, size)
      j += size
    } while (j < len)

    BUFFERS.push(buf)
  }
}

/**
 * The set of tests to run
 * @type {Object}
 */
module.exports = {
  title: 'multiple random-length buffers',
  iterations: 10000,
  init: initBuffers,
  tests: [{
    title: 'Native SSE 4.2 CRC-32C',
    fn: i => Crc32C.sse42_crc(BUFFERS[i])
  }, {
    title: 'Native Table-based CRC-32C',
    fn: i => Crc32C.table_crc(BUFFERS[i])
  }, {
    title: 'JavaScript (table-based) CRC-32C',
    fn: i => jsCrc32(BUFFERS[i], false)
  }, {
    title: 'JavaScript (direct) CRC-32C',
    fn: i => jsCrc32(BUFFERS[i], true)
  }]
}
