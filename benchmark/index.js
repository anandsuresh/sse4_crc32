/**
 * @file Runs the benchmarks and reports results
 *
 * @author Anand Suresh <anand.suresh@gmail.com>
 * @copyright Copyright (C) 2018-present Anand Suresh. All rights reserved.
 * @license MIT
 */

/**
 * Records and returns the run-time of the specified function
 *
 * @param {Function} fn The function to execute
 * @param {Number} count The number of iterations to run the function
 * @returns {Number} The run-time of the function (in ms)
 */
function run (fn, count) {
  const startTime = process.hrtime()
  for (let i = 0; i < count; i++) fn(i)
  const endTime = process.hrtime(startTime)

  return (endTime[0] * 1e9 + endTime[1]) / 1000000
}

/**
 * Runs the benchmarks
 */
[
  require('./single_fixed_size_buffer.benchmark'),
  require('./multiple_random_size_buffers.benchmark')
].forEach(benchmark => {
  // init the benchmark, if needed
  if (typeof benchmark.init === 'function') {
    benchmark.init(benchmark)
  }

  // run the tests
  console.log(`${benchmark.title}, ${benchmark.iterations} iterations:`)
  benchmark.tests
    .map(test => Object.assign(test, {time: run(test.fn, benchmark.iterations)}))
    .forEach(test => console.log(`- ${test.title}: %d ms`, test.time))
  console.log('\n')
})
