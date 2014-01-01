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
#include <node.h>

using namespace v8;

// Copies data between kExternalUnsignedByteArrays like node Buffers and ArrayBuffers.
// target[, targetStart], source[, sourceStart[, sourceEnd]]
Handle<Value> memcpy(const Arguments& args) {
    HandleScope scope;

    // Parse arguments
    if (args.Length() < 2) {
        ThrowException(Exception::TypeError(String::New("Illegal number of arguments")));
        return scope.Close(Undefined());
    }
    int i = 0;
    if (!args[i]->IsObject()) {
        ThrowException(Exception::TypeError(String::New("Illegal target: Not an object")));
        return scope.Close(Undefined());
    }
    Local<Object> target = args[i++]->ToObject();
    if (target->GetIndexedPropertiesExternalArrayDataType() != kExternalUnsignedByteArray) {
        ThrowException(Exception::TypeError(String::New("Illegal target: Not a valid kExternalUnsignedByteArray")));
        return scope.Close(Undefined());
    }
    int targetStart = 0;
    int targetLength = target->GetIndexedPropertiesExternalArrayDataLength();
    if (args[i]->IsUint32()) {
        targetStart = args[i++]->ToUint32()->Value();
        if (targetStart < 0 || targetStart > targetLength) {
            ThrowException(Exception::TypeError(String::New("Illegal targetStart: Less than 0 or bigger than length")));
            return scope.Close(Undefined());
        }
    }
    if (i >= args.Length()) {
        ThrowException(Exception::TypeError(String::New("Illegal number of arguments")));
        return scope.Close(Undefined());
    }
    if (!args[i]->IsObject()) {
        ThrowException(Exception::TypeError(String::New("Illegal source: Not an object")));
        return scope.Close(Undefined());
    }
    Local<Object> source = args[i++]->ToObject();
    if (source->GetIndexedPropertiesExternalArrayDataType() != kExternalUnsignedByteArray) {
        ThrowException(Exception::TypeError(String::New("Illegal source: Not a valid kExternalUnsignedByteArray")));
        return scope.Close(Undefined());
    }
    int sourceStart = 0;
    int sourceLength = source->GetIndexedPropertiesExternalArrayDataLength();
    int sourceEnd = sourceLength;
    if (i < args.Length()) {
        if (!args[i]->IsUint32()) {
            ThrowException(Exception::TypeError(String::New("Illegal sourceStart: Not an uint32")));
            return scope.Close(Undefined());
        }
        sourceStart = args[i++]->ToUint32()->Value();
        if (sourceStart < 0 || sourceStart > sourceLength) {
            ThrowException(Exception::TypeError(String::New("Illegal sourceStart: Less than 0 or bigger than length")));
            return scope.Close(Undefined());
        }
    }
    if (i < args.Length()) {
        if (!args[i]->IsUint32()) {
            ThrowException(Exception::TypeError(String::New("Illegal sourceEnd: Not an uint32")));
            return scope.Close(Undefined());
        }
        sourceEnd = args[i++]->ToUint32()->Value();
        if (sourceEnd < sourceStart || sourceEnd > sourceLength) {
            ThrowException(Exception::TypeError(String::New("Illegal sourceEnd: Less than sourceStart or bigger than length")));
            return scope.Close(Undefined());
        }
    }
    if (i /* still */ < args.Length()) {
        ThrowException(Exception::TypeError(String::New("Illegal number of arguments")));
        return scope.Close(Undefined());
    }

    // Perform sanity checks
    int len = sourceEnd - sourceStart;
    if (len == 0) {
        return scope.Close(Undefined()); // Nothing to copy
    }
    if (targetStart + len > targetLength) {
        ThrowException(Exception::TypeError(String::New("Illegal source range: Target capacity overrun")));
        return scope.Close(Undefined());
    }

    // Do the thing (memmove to be compatible to native Buffer#copy)
    memmove(
        static_cast<unsigned char*>(target->GetIndexedPropertiesExternalArrayData()) + targetStart,
        static_cast<unsigned char*>(source->GetIndexedPropertiesExternalArrayData()) + sourceStart,
        len
    );

    return scope.Close(Undefined());
}

void init(Handle<Object> exports) {
    exports->Set(String::NewSymbol("memcpy"), FunctionTemplate::New(memcpy)->GetFunction());
}

NODE_MODULE(memcpy, init);
