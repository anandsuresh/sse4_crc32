/**
 * @file Unit tests for the library
 *
 * @author Anand Suresh <anand.suresh@gmail.com>
 * @copyright Copyright (C) 2018-present Anand Suresh. All rights reserved.
 * @license MIT
 */

const expect = require('chai').expect
const fs = require('fs')
const path = require('path')
const Crc32C = require('..')

describe('CRC-32C', function () {
  describe('.calculate', function () {
    const input = 'A hardware-accelerated CRC-32C implementation'
    const output = 795910490
    const testCases = {
      'string': {input: input, output: output},
      'buffer': {input: Buffer.from(input), output: output}
    }

    Object.keys(testCases).forEach(function (inputType) {
      it(`should handle input of type ${inputType}`, function () {
        const input = testCases[inputType].input
        const output = testCases[inputType].output
        expect(Crc32C.calculate(input)).to.equal(output)
      })
    })
  })

  describe('.fromStream', function () {
    it('should correctly calculate the CRC-32C of the stream', function (done) {
      const input = fs.createReadStream(path.join(__dirname, 'fixture.txt'))
      const output = Crc32C.fromStream(input)

      output.on('finish', function () {
        expect(output.crc).to.equal(1240019588)
        done()
      })
    })
  })
})
