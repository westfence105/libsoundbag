libsoundbag
====
libsoundbag is some miscellaneous utility library for C++, made for my own use.

### Contains

##### soundbag.hpp

- xor128 function to generate random 32bit int value (need to link libsoundbag.a)
- bigger and smaller inline function to get comparison result

##### bits.hpp

- isLittleEndian function to decide endian
- encodeBytes / decodeBytes function to convert multi-byte variable to std::vector<byte> data with endian
- readBytes / writeBytes function to read/write multi-byte variable to/from file with endian

### License
MIT