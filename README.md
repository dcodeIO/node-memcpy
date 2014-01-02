node-memcpy
===========
Copies data between node Buffers and/or ArrayBuffers up to ~75 times faster than in pure JS.

Wait, what?
-----------
node.js utilizes a non-standard concept of I/O buffers and thus has both its Buffer as well as ArrayBuffer support.
While Buffers are nice because they are a lot faster than V8's ArrayBuffers, transferring data between those two
types can be ridiculously slow. This is where a node module like memcpy comes into play.

```
// C++ binding

 i memcpy.100k > cc Buffer -> Buffer: 22.756ms
 i memcpy.100k > cc Buffer -> ArrayBuffer: 23.861ms
 i memcpy.100k > cc Buffer -> Uint8Array: 22.953ms

 i memcpy.100k > cc ArrayBuffer -> Buffer: 22.955ms
 i memcpy.100k > cc ArrayBuffer -> ArrayBuffer: 23.273ms
 i memcpy.100k > cc ArrayBuffer -> Uint8Array: 22.685ms

 i memcpy.100k > cc Uint8Array -> Buffer: 23.472ms
 i memcpy.100k > cc Uint8Array -> ArrayBuffer: 22.975ms
 i memcpy.100k > cc Uint8Array -> Uint8Array: 22.953ms

// Native JS

 i memcpy.100k > js Buffer -> Buffer: 21.617ms
 i memcpy.100k > js Buffer -> ArrayBuffer: 993.361ms
 i memcpy.100k > js Buffer -> Uint8Array: 410.010ms

 i memcpy.100k > js ArrayBuffer -> Buffer: 940.273ms
 i memcpy.100k > js ArrayBuffer -> ArrayBuffer: 1626.182ms
 i memcpy.100k > js ArrayBuffer -> Uint8Array: 1084.790ms

 i memcpy.100k > js Uint8Array -> Buffer: 386.218ms
 i memcpy.100k > js Uint8Array -> ArrayBuffer: 1107.530ms
 i memcpy.100k > js Uint8Array -> Uint8Array: 502.653ms
```

API
---
##### memcpy(target[, targetStart=0], source[, sourceStart=0[, sourceEnd=source.length]):bytesCopied

| Argument     | Type                      | Optional  | Description
|--------------|---------------------------|-----------|------------------------------------------------------------------
| target       | Buffer &#124; ArrayBuffer &#124; Uint8Array | | Target buffer to copy to
| targetStart  | number                    | omittable | Target offset to begin copying to, defaults to `0`
| source       | Buffer &#124; ArrayBuffer &#124; Uint8Array | | Source buffer to copy from
| sourceStart  | number                    | optional  | Source offset to begin copying from, defaults to `0`
| sourceEnd    | number                    | optional  | Source offset to end copying from, defaults ot `source.length`
| **@returns** | number                    |           | Number of bytes copied

Source and target regions may overlap.

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
review the C++ code to validate that it's safe. I can't yet think of a sane use case, though, as just sticking with
Buffers on node.js and ArrayBuffers in the browser should be best practice.

**License:** [Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
