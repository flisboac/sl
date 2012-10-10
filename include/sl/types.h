/**
 * @file include/sl/types.h
 */

#ifndef SL_TYPES_H_
#define SL_TYPES_H_

#include <stdio.h>
#include <stddef.h>
#include "sldefs.h"

#define sl_INVALIDSIZE ((size_t)-1)
#define sl_INVALIDPOINT ((long int)0xffffffffL)

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

typedef enum sl_EEcode {
	  sl_EOS = -1
	, sl_OK
	, sl_ERROR
	, sl_ERRSUP
	, sl_ERRMEM
	, sl_ERRSYN
	, sl_ERRCNV
	, sl_ERRENC
} sl_EEcode;

typedef enum sl_EStreamType_t {

	  sl_CUSTOM = 0
	, sl_BUFFER
	, sl_STRING
	, sl_FILE
} sl_EStreamType;

typedef unsigned int sl_Ctypes;       /**< An integer with flags describing a character's types. */
typedef long int sl_Cpoint;           /**< An encoding's code point. */
typedef struct sl_Ctrait_t sl_Ctrait; /**< The encoding itself. */
typedef struct sl_Env_t sl_Env;       /**< The stream environment, to avoid global states. */
typedef struct sl_Cconv_t sl_Cconv;   /**< A type that can transcode streams. */
typedef struct sl_Stream_t sl_Stream; /**< The base stream object. */
typedef struct sl_Regex_t sl_Regex;   /**< A pattern matching object. */
typedef struct sl_Pos_t sl_Pos;       /**< An object representing a position inside a stream. */
typedef struct sl_StreamApi_t sl_StreamApi;
typedef ptrdiff_t sl_Off;             /**< A type representing an offset starting from a position. */

typedef void* (*sl_FAlloc)(void* state, void* ptr, size_t sz);

struct sl_Stream_t {

	sl_EStreamType type;
	sl_Ctrait *enc;
	const sl_StreamApi *api;
};

struct sl_Pos_t {

    sl_Stream *S;
    int aligned; /* If true, the position is on a character boundary. */
};

struct sl_Cconv_t {

	sl_Stream *from;
	sl_Stream *to;

	sl_EEcode (*step)(sl_Cconv *C, size_t len);
};

/*
 * All functions in a character trait should be reentrant, and do what they can
 * to not introduce side-effects. A character trait is about how the binary
 * stream is organized, it should not use any global state or call
 * non-reentrant functions.
 */
struct sl_Ctrait_t {

    const char* name;
    const size_t minsize;
    const size_t maxsize;
    const sl_Cpoint eof;
    
    /* Seeks `off` characters in a buffer `s` of size `sz`,
     * starting from the position `pos`. If pos is null, it's assumed to 
     * start from the beginning of the buffer, and if off is negative in this
     * case, the search is backwards and starts from the end of the stream.
     * At the end of the process, the number of bytes for the character found
     * will be written in sz.
     * - The first character could be written as `seekchar(s, sz, 0, NULL)`
     * - The next character could be written as `seekchar(s, sz, 1, NULL)`
     * - The last character could be written as `seekchar(s, sz, -1, NULL)`
     * - validchar check could be written as `seekchar(s, maxsize, 0, NULL) != NULL`
     *   (considering s has maxsize bytes)
     */
    const char* (*seekchar)(char *s, size_t *sz, long int off, char *pos);

    /* Checks the length of a stream, in characters.
     * Returns the size, or 0 if an error occurs.
     * If end is given, a pointer to the last byte of the
     * last valid character will be put there.
     * The entire stream, from s to end, will be composed of valid
     * characters (normalized or not). end will be null if the function returns
     * 0. */
    size_t (*strlen)(char *s, size_t sz, char **end);

    /* Gets the equivalent cpoint for the next character.
     * Returns sl_INVALIDPOINT on error. */
    sl_Cpoint (*cpoint)(char *s, size_t sz);

    /* Gets all valid ctypes for the character point passed. */
    sl_Ctypes (*ctypes)(sl_Cpoint cpoint);

    /* Transforms the code point into a string.
	 * s must be a pre-allocated buffer with at least minsize bytes.
     * Returns sl_INVALIDSIZE in case of error.
     * If s is NULL, the function simply returns the number of bytes
     * required for storing the character. */
    size_t (*tostr)(sl_Cpoint cpoint, char *s, size_t sz);

    /* tolower, toupper, swapcase... */
    sl_Cpoint (*tolower)(sl_Cpoint cpoint);
    sl_Cpoint (*toupper)(sl_Cpoint cpoint);
    sl_Cpoint (*swapcase)(sl_Cpoint cpoint);

};

#endif
