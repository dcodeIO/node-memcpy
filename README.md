node-memcpy
===========
Copies data between node Buffers and/or ArrayBuffers up to ~75 times faster than in pure JS.

Wait, what?
-----------
node.js utilizes a non-standard concept of I/O buffers and thus has both its Buffer as well as ArrayBuffer support.
While Buffers are nice because they are a lot faster than V8's ArrayBuffers, transferring data between those two
types can be ridiculously slow. This is where a node module like memcpy comes into play.

```
 i memcpy.100k > cc Buffer -> Buffer: 22.222ms
 i memcpy.100k > cc Buffer -> ArrayBuffer: 23.548ms
 i memcpy.100k > cc ArrayBuffer -> Buffer: 22.108ms
 i memcpy.100k > cc ArrayBuffer -> ArrayBuffer: 21.778ms

 i memcpy.100k > js Buffer -> Buffer: 23.163ms
 i memcpy.100k > js Buffer -> ArrayBuffer: 1043.713ms
 i memcpy.100k > js ArrayBuffer -> Buffer: 1003.351ms
 i memcpy.100k > js ArrayBuffer -> ArrayBuffer: 1607.978ms
```

Breakdown
---------
* `Buffer` to `Buffer` is about as fast as using `Buffer#copy`
* `Buffer` to `ArrayBuffer` is about **45 times faster**
* `ArrayBuffer` to `Buffer` is about **45 times faster**
* `ArrayBuffer` to `ArrayBuffer` is about **75 times faster**

API
---
#### memcpy(target[, targetStart=0], source[, sourceStart=0[, sourceEnd=source.length]):bytesCopied`

| Argument    | Type               | Optional  | Description
|-------------|--------------------|-----------|------------------------------------
| target      | Buffer|ArrayBuffer |           | Target buffer to copy to
| targetStart | number             | omittable | Target offset to begin copying to
| source      | Buffer|ArrayBuffer |           | Source buffer to copy from
| sourceStart | number             | optional  | Source offset to begin copying from
| sourceEnd   | number             | optional  | Source offset to end copying from
|-------------|--------------------|-----------|------------------------------------
| @returns    | number             |           | Number of bytes copied

Usage
-----
* `npm install memcpy`

```js
var memcpy = require("memcpy"), // C++ binding if available, else native JS
    memcpy_binding = memcpy.binding, // C++ binding or NULL if not available
    memcpy_native = memcpy.native; // Native JS
...
```

Please keep in mind that - besides the nice numbers - this is still to be considered experimental. I'd love if you'd
review the C++ code to validate that it's safe. I can't yet think about a sane use case, though, as just sticking with
Buffers on node.js and ArrayBuffers in the browser should be best practice.

**License:** [Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
