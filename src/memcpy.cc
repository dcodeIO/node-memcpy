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

using namespace v8;

// Copies data between kExternalUnsignedByteArrays like node Buffers and ArrayBuffers.
// memcpy(target[, targetStart], source[, sourceStart[, sourceEnd]]):bytesCopied
NAN_METHOD(memcpy){
    NanScope();

    // Parse arguments
    if (args.Length() < 2) {
        NanThrowTypeError("Illegal number of arguments");
        NanReturnUndefined();
    }
    int i = 0;
    if (!args[i]->IsObject()) {
        NanThrowTypeError("Illegal target: Not an object");
        NanReturnUndefined();
    }
    Local<Object> target = args[i++]->ToObject();
    if (target->GetIndexedPropertiesExternalArrayDataType() != kExternalUnsignedByteArray) {
        NanThrowTypeError("Illegal target: Not a valid kExternalUnsignedByteArray");
        NanReturnUndefined();
    }
    int targetStart = 0;
    int targetLength = target->GetIndexedPropertiesExternalArrayDataLength();
    if (args[i]->IsUint32()) {
        targetStart = args[i++]->ToUint32()->Value();
        if (targetStart < 0 || targetStart > targetLength) {
            NanThrowTypeError("Illegal targetStart: Less than 0 or bigger than length");
            NanReturnUndefined();
        }
    }
    if (i >= args.Length()) {
        NanThrowTypeError("Illegal number of arguments");
        NanReturnUndefined();
    }
    if (!args[i]->IsObject()) {
        NanThrowTypeError("Illegal source: Not an object");
        NanReturnUndefined();
    }
    Local<Object> source = args[i++]->ToObject();
    if (source->GetIndexedPropertiesExternalArrayDataType() != kExternalUnsignedByteArray) {
        NanThrowTypeError("Illegal source: Not a valid kExternalUnsignedByteArray");
        NanReturnUndefined();
    }
    int sourceStart = 0;
    int sourceLength = source->GetIndexedPropertiesExternalArrayDataLength();
    int sourceEnd = sourceLength;
    if (i < args.Length()) {
        if (!args[i]->IsUint32()) {
            NanThrowTypeError("Illegal sourceStart: Not an uint32");
            NanReturnUndefined();
        }
        sourceStart = args[i++]->ToUint32()->Value();
        if (sourceStart < 0 || sourceStart > sourceLength) {
            NanThrowTypeError("Illegal sourceStart: Less than 0 or bigger than length");
            NanReturnUndefined();
        }
    }
    if (i < args.Length()) {
        if (!args[i]->IsUint32()) {
            NanThrowTypeError("Illegal sourceEnd: Not an uint32");
            NanReturnUndefined();
        }
        sourceEnd = args[i++]->ToUint32()->Value();
        if (sourceEnd < sourceStart || sourceEnd > sourceLength) {
            NanThrowTypeError("Illegal sourceEnd: Less than sourceStart or bigger than length");
            NanReturnUndefined();
        }
    }
    if (i /* still */ < args.Length()) {
        NanThrowTypeError("Illegal number of arguments");
        NanReturnUndefined();
    }

    // Perform sanity checks
    int len = sourceEnd - sourceStart;
    if (len == 0) {
        NanReturnUndefined();
    }
    if (targetStart + len > targetLength) {
        NanThrowTypeError("Illegal source range: Target capacity overrun");
        NanReturnUndefined();
    }
    if (sizeof(unsigned char) != 1) {
        NanThrowTypeError("sizeof(unsigned char) != 1");
        NanReturnUndefined();
    }

    // Do the thing (memmove to be compatible with native Buffer#copy)
    memmove(
        static_cast<unsigned char*>(target->GetIndexedPropertiesExternalArrayData()) + targetStart,
        static_cast<unsigned char*>(source->GetIndexedPropertiesExternalArrayData()) + sourceStart,
        len
    );

    NanReturnValue(NanNew<Number>(len));
}

void init(Handle<Object> exports) {
    exports->Set(NanNew<String>("memcpy"), NanNew<FunctionTemplate>(memcpy)->GetFunction());
}

NODE_MODULE(memcpy, init);
