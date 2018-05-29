/**
 * @file Benchmarks using a single fixed-size buffer
 *
 * @author Anand Suresh <anand.suresh@gmail.com>
 * @copyright Copyright (C) 2018-present Anand Suresh. All rights reserved.
 * @license MIT
 */

const Crc32C = require('..')
const jsCrc32 = require('crc32')

/**
 * The input buffer for the CRC-benchmarks
 * @type {Buffer}
 */
const TEST_BUFFER = Buffer.from(`Lorem ipsum dolor sit amet, consectetur
adipiscing elit. Morbi mollis cursus metus vel tristique. Proin congue massa
massa, a malesuada dolor ullamcorper a. Nulla eget leo vel orci venenatis
placerat. Donec semper condimentum justo, vel sollicitudin dolor consequat id.
Nunc sed aliquet felis, eget congue nisi. Mauris eu justo suscipit, elementum
turpis ut, molestie tellus. Mauris ornare rutrum fringilla. Nulla dignissim
luctus pretium. Nullam nec eros hendrerit sapien pellentesque sollicitudin.
Integer eget ligula dui. Mauris nec cursus nibh. Nunc interdum elementum leo, eu
sagittis eros sodales nec. Duis dictum nulla sed tincidunt malesuada. Quisque in
vulputate sapien. Sed sit amet tellus a est porta rhoncus sed eu metus. Mauris
non pulvinar nisl, volutpat luctus enim. Suspendisse est nisi, sagittis at risus
quis, ultricies rhoncus sem. Donec ullamcorper purus eget sapien facilisis, eu
eleifend felis viverra. Suspendisse elit neque, semper aliquet neque sed,
egestas tempus leo. Duis condimentum turpis duis.`)

/**
 * The set of tests to run
 * @type {Array}
 */
module.exports = {
  title: 'single fixed-size buffer',
  iterations: 10000,
  tests: [{
    title: 'Native SSE 4.2 CRC-32C',
    fn: () => Crc32C.sse42_crc(TEST_BUFFER)
  }, {
    title: 'Native Table-based CRC-32C',
    fn: () => Crc32C.table_crc(TEST_BUFFER)
  }, {
    title: 'JavaScript (table-based) CRC-32C',
    fn: () => jsCrc32(TEST_BUFFER, false)
  }, {
    title: 'JavaScript (direct) CRC-32C',
    fn: () => jsCrc32(TEST_BUFFER, true)
  }]
}
