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
#include <string.h>
#include <node.h>
#include <nan.h>
#include <ArrayBuffer.h>
#include "memcpy.h"

using namespace v8;

// Copies data between kExternalUnsignedByteArrays like node Buffers and ArrayBuffers.
// memcpy(target[, targetStart], source[, sourceStart[, sourceEnd]]):bytesCopied
NAN_METHOD(memcpy){
    NanScope();
    if (sizeof(unsigned char) != 1) {
        NanThrowError("sizeof(unsigned char) != 1");
        NanReturnUndefined();
    }

    unsigned char* targetData;
    size_t targetStart = 0,
           targetLength;

    unsigned char* sourceData;
    size_t sourceStart = 0,
           sourceLength,
           sourceEnd;

    // Requires at least two arguments: target and source
    if (args.Length() < 2) {
        NanThrowTypeError("illegal number of arguments");
        NanReturnUndefined();
    }
    int i = 0;

    // Target must be an object
    if (!args[i]->IsObject()) {
        NanThrowTypeError("illegal target: not an object");
        NanReturnUndefined();
    }
    Local<Object> target = args[i]->ToObject();

    // Target must reference an unsigned byte array (or be an ArrayBuffer)
    if (target->GetIndexedPropertiesExternalArrayDataType() == kExternalUnsignedByteArray) {
        targetData = static_cast<unsigned char*>(target->GetIndexedPropertiesExternalArrayData());
        targetLength = target->GetIndexedPropertiesExternalArrayDataLength();
    } else {
#if NODE_MAJOR_VERSION > 0 || NODE_MINOR_VERSION >= 12
        if (!args[i]->IsArrayBuffer()) {
            NanThrowTypeError("illegal target: not an ArrayBuffer");
            NanReturnUndefined();
        }
        node::ArrayBuffer* ab = node::ArrayBuffer::New(args[i]);
        targetData = static_cast<unsigned char*>(ab->Data());
        targetLength = ab->ByteLength();
#else
        NanThrowTypeError("illegal target: not a kExternalUnsignedByteArray");
        NanReturnUndefined();
#endif
    }
    // If specified, targetStart must be an unsigned integer in [0,targetLength]
    if (args[++i]->IsUint32()) {
        targetStart = args[i++]->ToUint32()->Value();
        if (targetStart < 0 || targetStart > targetLength) {
            NanThrowRangeError("illegal targetStart: out of bounds");
            NanReturnUndefined();
        }
    }
    // Requires at least one additional argument: source
    if (i >= args.Length()) {
        NanThrowTypeError("illegal number of arguments");
        NanReturnUndefined();
    }
    // Source must be an object
    if (!args[i]->IsObject()) {
        NanThrowTypeError("illegal source: not an object");
        NanReturnUndefined();
    }
    Local<Object> source = args[i]->ToObject();

    // Source must reference an unsigned byte array (or be an ArrayBuffer)
    if (source->GetIndexedPropertiesExternalArrayDataType() == kExternalUnsignedByteArray) {
        sourceData = static_cast<unsigned char*>(source->GetIndexedPropertiesExternalArrayData());
        sourceEnd = sourceLength = source->GetIndexedPropertiesExternalArrayDataLength();
    } else {
#if NODE_MAJOR_VERSION > 0 || NODE_MINOR_VERSION >= 12
        if (!args[i]->IsArrayBuffer()) {
            NanThrowTypeError("illegal source: not an ArrayBuffer");
            NanReturnUndefined();
        }
        node::ArrayBuffer* ab = node::ArrayBuffer::New(args[i]);
        sourceData = static_cast<unsigned char*>(ab->Data());
        sourceEnd = sourceLength = ab->ByteLength();
#else
        NanThrowTypeError("illegal target: not a kExternalUnsignedByteArray");
        NanReturnUndefined();
#endif
    }
    // If specified, sourceStart must be an unsigned integer in [0,sourceLength]
    if (++i < args.Length()) {
        if (!args[i]->IsUint32()) {
            NanThrowTypeError("illegal sourceStart: not an uint32");
            NanReturnUndefined();
        }
        sourceStart = args[i++]->ToUint32()->Value();
        if (sourceStart < 0 || sourceStart > sourceLength) {
            NanThrowTypeError("illegal sourceStart: out of bounds");
            NanReturnUndefined();
        }
    }
    // If specified, sourceEnd must be an unsigned integer in [sourceStart,sourceLength]
    if (i < args.Length()) {
        if (!args[i]->IsUint32()) {
            NanThrowTypeError("illegal sourceEnd: not an uint32");
            NanReturnUndefined();
        }
        sourceEnd = args[i++]->ToUint32()->Value();
        if (sourceEnd < sourceStart || sourceEnd > sourceLength) {
            NanThrowTypeError("illegal sourceEnd: out of bounds");
            NanReturnUndefined();
        }
    }
    // Additional arguments are invalid
    if (i < args.Length()) {
        NanThrowTypeError("illegal number of arguments");
        NanReturnUndefined();
    }
    // Determine number of bytes to copy
    int length = sourceEnd - sourceStart;
    if (length == 0) {
        NanReturnValue(NanNew<Number>(length));
    }
    // Perform sanity checks
    if (targetStart + length > targetLength) {
        NanThrowTypeError("illegal source range: target capacity overrun");
        NanReturnUndefined();
    }
    // Do the thing (memmove to be compatible with native Buffer#copy)
    memmove(
        targetData + targetStart,
        sourceData + sourceStart,
        length
    );
    NanReturnValue(NanNew<Number>(length));
}

void init(Handle<Object> exports) {
    exports->Set(NanNew<String>("memcpy"), NanNew<FunctionTemplate>(memcpy)->GetFunction());
}

NODE_MODULE(memcpy, init);
