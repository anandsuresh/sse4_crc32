/**
 * @file Unit tests for the library
 *
 * @author Anand Suresh <anand.suresh@voxer.com>
 * @copyright Copyright (C) 2007-2015 Voxer LLC. All rights reserved.
 */

var expect = require('chai').expect;

var Sse4Crc32 = require('../sse4_crc32');



describe('Sse4Crc32', function() {
    describe('.calculate', function() {
        var testCases = {
            'string': {
                input : 'SSE4-CRC32: A hardware accelerated CRC32 implementation for node.js',
                output: 3039989317
            },
            'buffer': {
                input : new Buffer('SSE4-CRC32: A hardware accelerated CRC32 implementation for node.js'),
                output: 3039989317
            }
        };

        Object.keys(testCases).forEach(function(inputType) {
            it('should handle input of type ' + inputType, function() {
                expect(Sse4Crc32.calculate(testCases[inputType].input)).to.equal(testCases[inputType].output);
            });
        });
    });



    describe('CRC32', function() {
        it('should be callable', function() {
            expect(Sse4Crc32.CRC32).to.be.a('function');
        });


        it('should not throw an error upon instantiation', function() {
            expect(function() { return new Sse4Crc32.CRC32(); }).to.not.throw(Error);
        });


        it('should default the initial CRC to 0', function() {
            var input = null,
                crc = new Sse4Crc32.CRC32(input);

            expect(crc.crc32c).to.equal(0);
        });


        it('should accept an initial CRC value', function() {
            var input = null,
                initialCrc = 10,
                crc = new Sse4Crc32.CRC32(input, initialCrc);

            expect(crc.crc32c).to.equal(initialCrc);
        });


        it('should immediately calculate the value of the passed input', function() {
            var input = 'input',
                crc = new Sse4Crc32.CRC32(input);

            expect(crc.crc()).to.equal(3633258352);
        });


        it('should progressively calculate CRC for additional input', function() {
            var input = 'input',
                additionalInput = ' progressive',
                crc = new Sse4Crc32.CRC32(input);

            expect(crc.crc()).to.equal(3633258352);

            crc.update(additionalInput);
            expect(crc.crc()).to.equal(1233760415);
        });
    });
});
