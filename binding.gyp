{
  "targets": [
    {
      "target_name": "memcpy",
      "sources": [ "src/memcpy.cc" ],
      "include_dirs" : [
          "<!(node -e \"require('nan')\")",
          "<!(node -e \"require('node-arraybuffer')\")"
      ]
    }
  ]
}