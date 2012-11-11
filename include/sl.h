/**
 * @file include/sl.h
 */

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

Immediate types:
	- OFF:    Represents an offset
	- CPOINT: Represents a character point in a specific encoding.
	          It's equivalent to int for char, or wint_t for wchar_t.
	- CTYPES: An integer with flags describing character types for a character.

Base types:
	- STREAM: Basic type that represents a stream of data.
	- NATURE: Represents a specific nature of a stream.
	          Natures are not objects per se, they're interfaces present on
	          the stream's API.
	- POS:    Represents a position inside a stream.
	- LOCALE: Represents cultural-dependent options.
	- LFACET: Represents a specific aspect of a locale.
	          These can be either of:
	          - collate:
	          - ctrait:
	          - monetary:
	          - messages:
	          - numeric:
	          - time:

	- CTRAIT: An encoding, a way to tell how to interpret data.
	- CCONV:  Converts from one encoding to another.

Extra types:
	- REGEX:  A basic regular expression engine.

- Locale will be left for inclusion in later revisions.

*/

#include <stddef.h>

/*##############################################################################
 * [[[   CONFIGURATION   ]]]
 */


#define sl_NAME "sl"/*@@APPNAME@@*/
#define sl_RELEASETYPE "beta"/*@@RELEASETYPE@@*/
#define sl_MAJORVERSION (0)/*@@MAJORVERSION@@*/
#define sl_MINORVERSION (1)/*@@MINORVERSION@@*/
#define sl_PATCHVERSION (0)/*@@PATCHVERSION@@*/


/*##############################################################################
 * [[[   DEFINES   ]]]
 */


#define sl_STRQT(s) #s
#define sl_STRFY(s) um_STRQT(s)

#define sl_NOSIZE ((size_t)-1)
#define sl_NOPOINT ((long int)0xffffffffL)
#define sl_NOCMP (-2)

#define sl_MODEMASK (0x000F)
#define sl_READ     (0x0001)
#define sl_WRITE    (0x0002)
#define sl_BINARY   (0x0004)
#define sl_PREPEND  (0x0008)

#define sl_CTYPE (0x2fff) /**< The valid range of ctype flags. */
#define sl_ALPHA (0x0001) /**< An alphanumeric character. */
#define sl_DIGIT (0x0002) /**< A digit. */
#define sl_XDIGT (0x0004) /**< A hexadecimal digit. */
#define sl_CNTRL (0x0008) /**< A control character. */
#define sl_GRAPH (0x0010) /**< A graphical character. */
#define sl_PRINT (0x0020) /**< A printable character. */
#define sl_PUNCT (0x0040) /**< A punctuation character. */
#define sl_SPACE (0x0080) /**< A space character, including newlines and tabs. */
#define sl_NEWLN (0x0100) /**< A newline character. */
#define sl_UPPER (0x0200) /**< An uppercase letter. */
#define sl_LOWER (0x0400) /**< A lowercase letter. */
#define sl_EOS   (0x0800) /**< End-of-stream (file, buffer, string, etc). */
#define sl_VALID (0x1000) /**< If a character is valid, this MAY BE set if the character is of no other ctype. */
#define sl_SLENC (0x2000) /**< A valid "SL" locale character. */

#define sl_ISCTYPE(t, x) ( ((x) & (t)) != 0 )
#define sl_ISALPHA(x)    ( sl_ISCTYPE(sl_ALPHA, x) )
#define sl_ISDIGIT(x)    ( sl_ISCTYPE(sl_DIGIT, x) )
#define sl_ISXDIGT(x)    ( sl_ISCTYPE(sl_XDIGT, x) ) /**< Implies @c sl_ISALNUM(x) */
#define sl_ISALNUM(x)    ( sl_ISALPHA(x) && sl_ISDIGIT(x) )
#define sl_ISCNTRL(x)    ( sl_ISCTYPE(sl_CNTRL, x) ) /**< Implies @code !sl_ISALNUM(x) && !sl_ISPUNCT(x) @endcode */
#define sl_ISGRAPH(x)    ( sl_ISCTYPE(sl_GRAPH, x) )
#define sl_ISPRINT(x)    ( sl_ISCTYPE(sl_PRINT, x) )
#define sl_ISPUNCT(x)    ( sl_ISCTYPE(sl_PUNCT, x) ) /**< Implies @c !sl_ISALNUM(x) */
#define sl_ISSPACE(x)    ( sl_ISCTYPE(sl_SPACE, x) )
#define sl_ISNEWLN(x)    ( sl_ISCTYPE(sl_NEWLN, x) ) /**< Implies @c sl_ISSPACE(x) */
#define sl_ISUPPER(x)    ( sl_ISCTYPE(sl_UPPER, x) ) /**< Implies @c sl_ISALPHA(x) */
#define sl_ISLOWER(x)    ( sl_ISCTYPE(sl_LOWER, x) ) /**< Implies @c sl_ISALPHA(x) */
#define sl_ISVALID(x)    ( sl_ISANY(x) || sl_ISCTYPE(sl_VALID, x) )
#define sl_ISSLENC(x)    ( sl_ISCTYPE(sl_SLENC, x) )
#define sl_ISEOS(x)      ( sl_ISCTYPE(sl_EOS,   x) )
#define sl_ISANY(x)      ( sl_ISCTYPE(sl_CTYPE, x) )
#define sl_ISNONE(x)     !sl_ISANY(x)


/* Do not change! */
#define sl_RELEASENAME sl_NAME \
	" " sl_STRFY(sl_MAJORVERSION) \
	"." sl_STRFY(sl_MINORVERSION) \
	"." sl_STRFY(sl_PATCHVERSION) \
	"-" sl_RELEASETYPE


#if !defined(sl_STATIC) && defined(_WIN32)
#   define sl_FIMPORT __declspec(dllimport)
#   define sl_FEXPORT __declspec(dllexport)
#   define sl_DIMPORT sl_FIMPORT
#   define sl_DEXPORT sl_FEXPORT
#else
#   define sl_FIMPORT extern
#   define sl_FEXPORT
#   define sl_DIMPORT sl_FIMPORT
#   define sl_DEXPORT
#endif


#if defined(sl_BUILDING)
#   define sl_API sl_FEXPORT
#   define sl_DATA sl_DEXPORT
#   if defined(sl_STATIC)
#       define sl_IAPI static
#       define sl_IDATA
#   else
#       if defined(__GNUC__) && ((__GNUC__*100 + __GNUC_MINOR__) >= 302) && \
                defined(__ELF__)
#           define sl_IAPI __attribute__((visibility("hidden"))) extern
#           define sl_IDATA sl_IAPI
#       else
#           define sl_IAPI extern
#           define sl_IDATA extern
#       endif
#   endif
#else
#   define sl_API sl_FIMPORT
#   define sl_DATA sl_DIMPORT
#endif


/*##############################################################################
 * [[[   ENUMERATIONS   ]]]
 */


typedef enum sl_EEcode {
	  sl_ERRPART = -2
	, sl_ERREOS = -1
	, sl_OK
	, sl_ERROR
	, sl_ERRSUP
	, sl_ERRMEM
	, sl_ERRSYN
	, sl_ERRCNV
	, sl_ERRENC
} sl_EEcode;


typedef enum sl_ENature_ {

	  sl_NAT_NONE   /**< The stream has no specific nature. */
	, sl_NAT_FILTER /**< The stream is a converter between traits. */
	, sl_NAT_BUFFER /**< The stream is buffered. */
	, sl_NAT_FILE   /**< The stream is a file handle. */
	, sl_NAT_STRING /**< The stream implements a string object. */

	/* ... */

	, sl_NAT_MAX
} sl_ENature;


/*##############################################################################
 * [[[   TYPE DEFINITIONS   ]]]
 */

/* Immediate types */
typedef unsigned int sl_Ctypes;      /**< An integer with flags describing a character's types. */
typedef long int     sl_Cpoint;      /**< An encoding's code point. */
typedef long int     sl_Off;         /**< A type representing an offset starting from a position. */

/* Base types */
typedef struct sl_Ctrait_ sl_Ctrait; /**< The encoding itself. */
typedef struct sl_Cconv_  sl_Cconv;  /**< An encoding conversion object. */

/* Environment */
typedef struct sl_Stream_ sl_Stream; /**< The base stream object. */
typedef struct sl_Pos_    sl_Pos;    /**< An object representing a position inside a stream. */

/* Streams */
typedef struct sl_StreamApi_ sl_StreamApi;
typedef struct sl_State_     sl_State;
typedef struct sl_Opts_      sl_Opts;

/* Natures */
typedef struct sl_FilterState_  sl_FilterState;
typedef struct sl_FilterNature_ sl_FilterNature;
typedef struct sl_BufferState_  sl_BufferState;
typedef struct sl_BufferNature_ sl_BufferNature;
typedef struct sl_FileState_    sl_FileState;
typedef struct sl_FileNature_   sl_FileNature;
typedef struct sl_StringState_  sl_StringState;
typedef struct sl_StringNature_ sl_StringNature;

/* Options */
typedef struct sl_StringOpts_   sl_StringOpts;
typedef struct sl_StdfileOpts_  sl_StdfileOpts;
typedef struct sl_FileOpts_     sl_FileOpts;

/* Function signatures */
typedef int       (*sl_FEvent)  (void* state, sl_EEcode ecode, sl_Stream* S, sl_Pos* P);
typedef void*     (*sl_FAlloc)  (void* allocp, void* ptr, size_t sz, size_t align);
typedef sl_Cconv* (*sl_FCcopen) (sl_Ctrait* from, sl_Ctrait* to, sl_FAlloc allocf, void* allocp);


/*##############################################################################
 * [[[   BASIC STRUCTS AND UNIONS   ]]]
 */


struct sl_Cconv_ {

	sl_Ctrait*  from_enc;
	sl_Ctrait*  to_enc;
	const char* from;
	char*       to;
	char*       from_pos;
	char*       to_pos;
	size_t      from_sz;
	size_t      to_sz;

	sl_EEcode (*step)    (sl_Cconv* conv, size_t step);
	sl_EEcode (*dispose) (sl_Cconv* conv);
};


struct sl_Ctrait_t {

	const sl_Ctrait* base;
	const char*      name;
    const size_t     minsize;
    const size_t     maxsize;
    const sl_Cpoint  eof;

    size_t    (*seek)    (sl_Ctrait* T, char *s, size_t sz, sl_Off off, char **pos);
    sl_Cpoint (*cpoint)  (sl_Ctrait* T, char *s, size_t sz);
    size_t    (*tostr)   (sl_Ctrait* T, sl_Cpoint cpoint, char *s, size_t sz);
    sl_Ctypes (*ctypes)  (sl_Ctrait* T, sl_Cpoint cpoint);
    sl_Cpoint (*tolower) (sl_Ctrait* T, sl_Cpoint cpoint);
    sl_Cpoint (*toupper) (sl_Ctrait* T, sl_Cpoint cpoint);
    int       (*cmp)     (sl_Ctrait* T, const char* a, size_t a_sz, const char* b, size_t b_sz);
    size_t    (*xfrm)    (sl_Ctrait* T, const char* from, size_t from_sz, char* to, size_t to_sz);
};


struct sl_Stream_ {

	sl_StreamApi* api;
	sl_Ctrait*    enc;
};


struct sl_Pos_ {

    sl_Stream* stream;
    int        aligned;

    void (*dispose) (sl_Pos* pos);
};


struct sl_Opts_ {

	sl_FAlloc    allocf;
	void*        allocp;
	sl_FEvent    eventf;
	void*        eventp;
	unsigned int mode;
};


struct sl_State_ {

	unsigned int mode;
};


struct sl_StreamApi_ {

	sl_StreamApi* base;

	const char*       (*getimplname)   (sl_StreamApi* A);
	const sl_ENature* (*getnatures)    (sl_StreamApi* A);
	void**            (*getnatureapis) (sl_StreamApi* A);

	sl_Stream* (*openwith) (sl_StreamApi* A, sl_Opts* opts);
	sl_Stream* (*reopen)   (sl_StreamApi* A, sl_Stream* S, sl_Opts* opts);
	sl_EEcode  (*close)    (sl_StreamApi* A, sl_Stream* S);
	sl_State*  (*getstate) (sl_StreamApi* A, sl_Stream* S, sl_State* state);

	size_t     (*read)     (sl_StreamApi* A, sl_Stream* S, char* buf, size_t sz, size_t nchars);
	size_t     (*write)    (sl_StreamApi* A, sl_Stream* S, char* buf, size_t sz, size_t nchars);
	int        (*flush)    (sl_StreamApi* A, sl_Stream* S);

	sl_Pos*    (*getpos)   (sl_StreamApi* A, sl_Stream* S);
	sl_EEcode  (*setpos)   (sl_StreamApi* A, sl_Stream* S, sl_Pos* pos);
	sl_EEcode  (*tell)     (sl_StreamApi* A, sl_Stream* S, sl_Off* off);
	sl_EEcode  (*seek)     (sl_StreamApi* A, sl_Stream* S, sl_Off off, int where);
};


/*##############################################################################
 * [[[   FILTER STRUCTS AND UNIONS   ]]]
 */


struct sl_FilterState_ {

	sl_State state;
	sl_Stream* base;
};


struct sl_FilterNature_ {

	sl_FilterState*  (*getstate)    (sl_FilterNature* A, sl_Stream* S, sl_FilterState* state);
};


sl_API sl_FilterState* sl_getfilterstate(sl_Stream* S, sl_FilterState* state);


/*##############################################################################
 * [[[   BUFFER STRUCTS AND UNIONS   ]]]
 */


struct sl_BufferState_ {

	sl_State state;
	size_t sz;
	size_t cap;
};


struct sl_BufferNature_ {

	sl_BufferState*  (*getstate)    (sl_BufferNature* A, sl_Stream* S, sl_BufferState* state);
};


sl_API sl_BufferState* sl_getbufferstate(sl_Stream* S, sl_BufferState* state);


/*##############################################################################
 * [[[   FILE STRUCTS AND UNIONS   ]]]
 */


struct sl_FileState_ {

	sl_State       state;
	const char*    path;
	const wchar_t* wpath;
	long int       fhandle;
	void*          fptr;
};


struct sl_FileNature_ {

	sl_FileState*  (*getstate)    (sl_FileNature* A, sl_Stream* S, sl_FileState* state);
};


sl_API sl_FileState* sl_getfilestate(sl_Stream* S, sl_FileState* state);


/*##############################################################################
 * [[[   STRING STRUCTS, UNIONS AND APIS   ]]]
 */


struct sl_StringState_ {

	sl_Opts opts;
	size_t sz;
	size_t cap;
};


struct sl_StringNature_ {

	sl_StringState*  (*getstate)    (sl_StringNature* A, sl_Stream* S, sl_StringState* state);
};


sl_API sl_StringState* sl_getstringstate(sl_Stream* S, sl_StringState* state);


/*##############################################################################
 * [[[   EXTERNAL DATA   ]]]
 */


struct sl_StringOpts_ {

	sl_Opts opts;
	size_t  cap;
};


struct sl_StdfileOpts_ {

	sl_Opts     opts;
	const char* path;
};


struct sl_FileOpts_ {

	sl_Opts opts;
	const char*    path;
	const wchar_t* wpath;
};


sl_DATA const sl_StreamApi* const sl_stdfile;
sl_DATA const sl_StreamApi* const sl_cstring;
sl_DATA const sl_StreamApi* const sl_file;

sl_DATA const sl_Ctrait* const sl_ctrait_ascii;
sl_DATA const sl_Ctrait* const sl_ctrait_slenc;
sl_DATA const sl_Ctrait* const sl_ctrait_char;
sl_DATA const sl_Ctrait* const sl_ctrait_wchar;


/*##############################################################################
 * [[[   UTILITY API   ]]]
 */


sl_API sl_FAlloc sl_getalloc(void **allocf);


#endif
