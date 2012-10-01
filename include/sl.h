#ifndef SL_H_
#define SL_H_

/*

General considerations:

- SIZE (size) is the actual size, in bytes, of the stream. This may not be known
  in advance, but will almost be for strings.
- CAPACITY (cap) is size, in bytes, of the amount of bytes the stream can
  accomodate. Generally, SIZE == CAPACITY unless stated otherwise. As with
  size, it may not be known in advance.
- LENGTH (len) is the size, in characters (or character points), in a stream.

The following assumptions can be made of strings:
- DATA: Stream data is represented by `char*`. This does not imply byte or
  multi-byte strings. A stream data can have fixed increment steps to retrieve a
  element (typecast to a wchar_t to perform pointer arithmetic, or do increments
  6-bytes at a time, for example), or do an unknwon way to retrieve characters.
- CHARACTER POINT: A `long` represents a point, a value that uniquely represents
  a character in its traits environment (in other words, in the encoding
  represented by the trait). The library makes the assumption that long is at
  least 32-bits wide. For Unicode, for example, this means a grapheme.
  If long is not enough to uniquely represent a character, i guess you have a
  rather big environment to deal with...
- CHARACTER TYPES: Character types are represented by flags in an `unsigned int`
  value. The library makes the assumption that ints are at least 16-bits wide.
  If not, well, sorry...
- CAPACITY AND SIZE: Capacity and size are given in `size_t`.
- POSITION: Due to the nature of this arrangement, character positions in the
  strings are also given in `char*`.
- ABSOLUTE INDEXING: Absolute indexing of string contents, be it in bytes or
  in characters, are given in `size_t`. Indexing, in this context, means
  accessing string data through array indices, like in `str[0]`.
- RELATIVE POSITIONING: Relative positioning, be it in bytes or in characters,
  is given in `ptrdiff_t`, and always parts from a pointer position. 

*/

#include "sldefs.h"
#include "sl/types.h"
#include "sl/util.h"
#include "sl/env.h"
#include "sl/stream.h"
#include "sl/string.h"
#include "sl/file.h"

#endif
