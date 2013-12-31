node-memcpy
===========
Copies data between node Buffers and/or ArrayBuffers up to ~75 times faster than in pure JS.

Wait, what?
-----------
node.js utilizes a non-standard concept of I/O buffers and thus has both its Buffer as well as ArrayBuffer support.
While Buffers are nice because they are a lot faster than V8's ArrayBuffers, transferring data between those two
types can be ridiculously slow. This is where a node module like memcpy comes into play.

```
 |_ _ _|_
 |_(-_)|_                                                       test.js v1.0.0

 i memcpy.interop > cc Buffer -> Buffer
 i memcpy.interop > cc Buffer -> ArrayBuffer
 i memcpy.interop > cc ArrayBuffer -> Buffer
 i memcpy.interop > cc ArrayBuffer -> ArrayBuffer
 i memcpy.interop > js Buffer -> Buffer
 i memcpy.interop > js Buffer -> ArrayBuffer
 i memcpy.interop > js ArrayBuffer -> Buffer
 i memcpy.interop > js ArrayBuffer -> ArrayBuffer
 + memcpy.interop                                   5.430 ms     48 assertions
 i memcpy.100k > cc Buffer -> Buffer: 22.222ms
 i memcpy.100k > cc Buffer -> ArrayBuffer: 23.548ms
 i memcpy.100k > cc ArrayBuffer -> Buffer: 22.108ms
 i memcpy.100k > cc ArrayBuffer -> ArrayBuffer: 21.778ms
 i memcpy.100k > js Buffer -> Buffer: 23.163ms
 i memcpy.100k > js Buffer -> ArrayBuffer: 1043.713ms
 i memcpy.100k > js ArrayBuffer -> Buffer: 1003.351ms
 i memcpy.100k > js ArrayBuffer -> ArrayBuffer: 1607.978ms
 + memcpy.100k                                   3773.234 ms      0 assertions

 test OK 2 tests (3780.560 ms, 48 assertions)
```

Breakdown
---------
* `Buffer` to `Buffer` is about as fast as using `Buffer#copy`
* `Buffer` to `ArrayBuffer` is about **45 times faster**
* `ArrayBuffer` to `Buffer` is about **45 times faster**
* `ArrayBuffer` to `ArrayBuffer` is about **75 times faster**

Usage
-----
* `npm install memcpy`
* `var memcpy = require("memcpy");`
* `memcpy(target[, targetStart=0], source[, sourceStart=0[, sourceEnd=source.length])`

Please keep in mind that - besides the nice numbers - this is still to be considered experimental. I'd love if you'd
review the C++ code to validate that it's safe.

**License:** [Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
