/**
 * @file sldefs.h
 *
 * Defines the library's implementor interface.
 */

#ifndef SLDEFS_H_
#define SLDEFS_H_


#include "sl.h"

/*
 * [ [ [ TYPE DEFINITIONS ] ] ] ================================================
 */


/* Nature-oriented API types */
typedef struct slstream_a   slstream_a; /**< Mandatory API definition for all streams. */
typedef struct slreader_a   slreader_a; /**< API definition for readable streams. */
typedef struct slwriter_a   slwriter_a; /**< API definition for writable streams. */
typedef struct sleventh_a   sleventh_a; /**< API definition for handles accepting event handlers. */
typedef struct sllocale_a   sllocale_a; /**< API definition for locale-aware streams. */
typedef struct slproxy_a    slproxy_a;  /**< API definition for proxy */

/* Instance-oriented API types */
typedef struct slpos_a      slpos_a;    /**< API definition for a position or iterator instance. */
typedef struct slcconv_a    slcconv_a;  /**< API definition for a encoding converter instance. */
typedef struct slwalker_a   slwalker_a; /**< API definition for an encoding walker instance. */
typedef struct slfilter_a   slfilter_a; /**< API definition for a stream filter instance. */
typedef struct slbuffer_a   slbuffer_a; /**< API definition for a stream buffer instance. */
typedef struct slfile_a     slfile_a;   /**< API definition for a file stream instance. */
typedef struct slstring_a   slstring_a; /**< API definition for a string stream instance. */

/* Delegates and signatures */
typedef void*             (*slgetapi_f) (slhnd_t* hnd, int htype);


/*
 * [ [ [ FRONT-END STRUCTURES AND UNIONS ] ] ] =================================
 */


struct slhnd_t {
    slgetapi_f getapi;
	slecode_e  ecode;
};


struct slapi_t {
    slhtype_e  htype;
	slgetapi_f getapi;
};


struct slenc_t {
    const slenc_t*    base;       /**< Encoding used to provide fallback functionality and derivation capabilities to encodings. Sub-encodings should compare equivalent to their super-encodings. */
    const char*       name;       /**< The name of the encoding, or NULL if the encoding is unnamed or if the name is not needed. Should be set for each concrete encoding or sufficiently different sub-encoding. */
	slenum            deriv;      /**< Denotes a specific format of encoding (e.g. normalization, STDIO transformation). */
	size_t            elemsize;   /**< Informs the size of the type used to store characters in the encoding. */
	size_t            minelems;   /**< The minimum number of elements used to represent a single character point in the encoding. If `minsize != maxsize` and neither @c minsize nor @c maxsize equals 0, the encoding is considered to be "multi-element". A multi-element encoding is considered multi-byte if `elemsize == 1` (to enforce a logical difference between byte-oriented and wide-oriented streams). */
	size_t            maxelems;   /**< The maximum number of elements used to represent a single character point in the encoding. */
	size_t            maxchars;   /**< The maximum number of character points used to represent a single character in the encoding. This is provided for compatibility with encodings that support character point composition, such as Unicode. Clients can then allocate buffers big enough to allocate any possible character in the encoding. */
	const slcpoint_t* eos;        /**< */
	
	size_t          (*listderiv) (int (*visitor)(void* state, const slenc_t* enc), void* visitorp);
	size_t          (*listalias) (int (*visitor)(void* state, const char* alias), void* visitorp);
	slhnd_t*        (*cconv)     (const char* toencname, const slenc_t* toenc, const slopts_t* opts);
	slhnd_t*        (*walker)    (const char* buf, size_t bytesz);
};


struct slloc_t {
	const slloc_t*      base;
	const char*         name;
	const slfacet_t**   facets;
	
	slloc_t*          (*copy)(slloc_t* loc);
};


/*
 * [ [ [ BACK-END STRUCTURES AND UNIONS ] ] ] ==================================
 */


struct slcconv_t {
	slenc_t*    fromenc;
	slenc_t*    toenc;
	const char* fromname;
	const char* toname;
	const char* frombuf;
	char*       tobuf;
	char*       frompos;
	char*       topos;
	size_t      fromsz;
	size_t      tosz;

	slecode_e (*step)    (slcconv_t* conv, size_t nchars);
	slecode_e (*dispose) (slcconv_t* conv);
};


/*
 * [ [ [ API STRUCTURES AND UNIONS ] ] ] =======================================
 */

struct sltype_a {
    slhtype_e     type;
	const char*   apiname;
	slhnd_t*    (*proxied) (slhnd_t* hnd);
	slecode_e   (*recover) (slhnd_t* hnd); /**< Tries to recover from a previous error. By default, sets ecode to sl_OK, but may require additional actions depending on the stream type. */
	void        (*dispose) (slhnd_t* hnd); /**< Disposes */
};



#endif /* SLDEFS_H_ */
