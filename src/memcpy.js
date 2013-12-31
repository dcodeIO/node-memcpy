/*
 Copyright 2013 Daniel Wirtz <dcode@dcode.io>

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/**
 * @license memcpy (c) 2013 Daniel Wirtz <dcode@dcode.io>
 * Released under the Apache License, Version 2.0
 * see: https://github.com/dcodeIO/memcpy for details
 */ //
(function(module) {
    "use strict";

    var path = require("path"),
        binding = null;
    try { binding = require('bindings')('memcpy.node'); } catch (e) {}

    /**
     * @type {number}
     * @inner
     */
    var i = 0;

    /**
     * @type {number}
     * @inner
     */
    var j = 0;

    /**
     * @type {?Uint8Array}
     * @inner
     */
    var k = null;

    /**
     * @type {?Uint8Array}
     * @inner
     */
    var l = null;

    /**
     * Copies data between Buffers and/or ArrayBuffers in a uniform way.
     * @exports memcpy
     * @function
     * @name memcpy
     * @param {!(Buffer|ArrayBuffer)} target Destination
     * @param {number|!(Buffer|ArrayBuffer)} targetStart Destination start, defaults to 0.
     * @param {(!(Buffer|ArrayBuffer)|number)=} source Source
     * @param {number=} sourceStart Source start, defaults to 0.
     * @param {number=} sourceEnd Source end, defaults to capacity.
     * @throws {Error} If any index is out of bounds
     */
    function memcpy(target, targetStart, source, sourceStart, sourceEnd) {
        if (typeof targetStart === 'object') {
            sourceEnd = sourceStart;
            sourceStart = source;
            source = targetStart;
            targetStart = 0;
        }
        sourceStart = sourceStart || 0;
        if (target instanceof Buffer) {

            // Buffer source -> Buffer target (the binding is a tiny bit faster)
            if (source instanceof Buffer) {
                sourceEnd = sourceEnd || source.length;
                if (targetStart+sourceEnd-sourceStart > target.length) {
                    throw(new Error("Buffer overrun"));
                }
                source.copy(target, targetStart, sourceStart, sourceEnd);

                // Slower:
                // for (i=sourceStart, j=targetStart; i<sourceEnd; ++i, ++j) target[j] = source[i];

            // ArrayBuffer source -> Buffer target (the binding is about 45 times faster)
            } else {
                sourceEnd = sourceEnd || source.byteLength;
                if (targetStart+sourceEnd-sourceStart > target.length) {
                    throw(new Error("Buffer overrun"));
                }
                for (i=sourceStart, j=targetStart, k=new Uint8Array(source); i<sourceEnd; ++i, ++j) target[j] = k[i];
                k = null;
            }

        } else {

            // Buffer source -> ArrayBuffer target (the binding is about 45 times faster)
            if (source instanceof Buffer) {
                sourceEnd = sourceEnd || source.length;
                if (targetStart+sourceEnd-sourceStart > target.byteLength) {
                    throw(new Error("Buffer overrun"));
                }
                for (i=sourceStart, j=targetStart, k=new Uint8Array(target); i<sourceEnd; ++i, ++j) k[j] = source[i];
                k = null;

            // ArrayBuffer source -> ArrayBuffer target (the binding is about 75 times faster)
            } else {
                sourceEnd = sourceEnd || source.byteLength;
                if (targetStart+sourceEnd-sourceStart > target.byteLength) {
                    throw(new Error("Buffer overrun"));
                }
                for (i=sourceStart, j=targetStart, k=new Uint8Array(target), l=new Uint8Array(source); i<sourceEnd; ++i, ++j) k[j] = l[i];
                k = l = null;
            }

        }
    }

    if (binding) {
        module.exports = binding.memcpy;
        module.exports.binding = binding.memcpy;
        module.exports.native = memcpy;
    } else {
        module.exports = memcpy;
        module.exports.binding = null;
        module.exports.native = memcpy;
    }
})(module);