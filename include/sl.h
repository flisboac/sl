/**
 * @file sl.h
 * 
 * Defines the library's client interface.
 */

#ifndef SL_H_
#define SL_H_

#include <stddef.h>
#include <stdarg.h>

/*
 * [ [ [   CONFIGURATION   ] ] ] ===============================================
 */


#define sl_NAME "sl"/*@@APPNAME@@*/
#define sl_RELEASETYPE "beta"/*@@RELEASETYPE@@*/
#define sl_MAJORVERSION (0)/*@@MAJORVERSION@@*/
#define sl_MINORVERSION (1)/*@@MINORVERSION@@*/
#define sl_PATCHVERSION (0)/*@@PATCHVERSION@@*/

/* Do not change! */
#define sl_STRQT(s) #s
#define sl_STRFY(s) um_STRQT(s)
#define sl_RELEASENAME sl_NAME \
	" " sl_STRFY(sl_MAJORVERSION) \
	"." sl_STRFY(sl_MINORVERSION) \
	"." sl_STRFY(sl_PATCHVERSION) \
	"-" sl_RELEASETYPE


/*
 * [ [ [ MACROS AND DEFINITIONS ] ] ] ==========================================
 */


/* Generic macros */
#define sl_ISFLAG(t, x) ( ((x) & (t)) != 0 )

/* Tri-boolean states */
#define sl_yes             (1)
#define sl_maybe           (0)
#define sl_no              (-1)
#define sl_true            (1)
#define sl_false           (0)

/* Tri-boolean testing (NOT supposed to be used slbool) */
#define sl_YES(x)          ((x) > 0)
#define sl_NO(x)           ((x) < 0)
#define sl_TRUE            sl_YES
#define sl_FALSE(x)        ((x) <= 0)
#define sl_MAYBE(x)        ((x) == 0)
#define sl_SURE(x)         ((x) != 0)

/* Error code testing */
#define sl_OK(c)           ((c) == sl_E_OK)
#define sl_ERANGE(c)       ((c) >= sl_E_MIN && (c) <= sl_E_MAX)
#define sl_VALID(c)        ((c) <= sl_E_OK)
#define sl_WARN(c)         ((c) < sl_E_OK)
#define sl_ERROR(c)        ((c) > sl_E_OK)
#define sl_EOS(c)          ((c) == sl_E_END)
#define sl_EOF(c)          sl_eos(c)

/* Iteration modes */
#define sl_W_IS(t, x)      sl_ISFLAG((t), (x) & sl_W_MASK)
#define sl_W_MASK          (0x000F)
#define sl_W_PERMASK       (0x0007) /**< Mask for the iteration mode flags. */
#define sl_W_PERCODE       (0x0001) /**< Iteration is done per code point. This is generally the default mode. */
#define sl_W_PERCHAR       (0x0002) /**< Iteration is done per character. This means that the walker may return more than one codepoint per iteration, so long as it forms a complete character with the combination of all returned codepoints. */
#define sl_W_PERUNIT       (0x0003) /**< Iteration is done per code unit (e.g. char, wchar_t), disregarding codepoint completeness. */
#define sl_W_REVERSE       (0x0008) /**< Iteration is done in reverse order, from last to first. */

/* Stream opening modes */
#define sl_M_IS(t, x)      sl_ISFLAG((t), (x) & sl_M_MASK)
#define sl_M_MASK          (0x00FF)
#define sl_M_RWMASK        (0x0003)
#define sl_M_READ          (0x0001)
#define sl_M_WRITE         (0x0002)
#define sl_M_RW            (sl_M_READ | sl_M_WRITE)
#define sl_M_APPEND        (0x0004)
#define sl_M_BINARY        (0x0008)
#define sl_M_UPDATE        (0x0010)
#define sl_M_CREATE        (0x0020)
#define sl_M_ORIENTEDMASK  (0x00C0)
#define sl_M_CHARORIENTED  (0x0040)
#define sl_M_WIDEORIENTED  (0x0080)
#define sl_M_ENCORIENTED   (0x00C0)

/* Buffering modes */
#define sl_B_IS(t, x)  sl_ISFLAG((t), (x) & sl_B_MASK)
#define sl_B_MASK          (0x000F)
#define sl_B_PERFLAGS      (0x0007)
#define sl_B_NOBUF         (0x0001) /**< Deactivates buffering. */
#define sl_B_NATIVE        (0x0002) /**< Enforces default buffering behaviour from the underlying library used to implement the stream. The user has no control over the buffering mechanisms through the library's API. */
#define sl_B_FULL          (0x0003)
#define sl_B_PERLINE       (0x0004)
#define sl_B_PERCHAR       (0x0005)

/* Char types */
#define sl_C_MASK          (0x1fff) /**< The valid range of ctype flags. */
#define sl_C_SPACE         (0x0080) /**< A space character, including newlines and tabs. */
#define sl_C_CNTRL         (0x0008) /**< A control character. */
#define sl_C_PRINT         (0x0020) /**< A printable character. */
#define sl_C_UPPER         (0x0200) /**< An uppercase letter. */
#define sl_C_LOWER         (0x0400) /**< A lowercase letter. */
#define sl_C_ALPHA         (0x0001) /**< An alphanumeric character. */
#define sl_C_DIGIT         (0x0002) /**< A digit. */
#define sl_C_PUNCT         (0x0040) /**< A punctuation character. */
#define sl_C_XDIGT         (0x0004) /**< A hexadecimal digit. */
#define sl_C_NEWLN         (0x0100) /**< A newline character. Usage may vary depending on locale and stream type. */
#define sl_C_EOS           (0x0800) /**< End-of-stream (file, buffer, string, etc). */
#define sl_C_VALID         (0x1000) /**< If a character is valid, this MAY BE set if the character is of no other ctype. */
#define sl_C_ALNUM         (sl_C_ALPHA | sl_C_DIGIT)
#define sl_C_GRAPH         (sl_C_ALNUM | sl_C_PUNCT) /**< A graphical character. */

/* Character type testing */
#define sl_C_IS(t, x)  sl_C_ISFLAG((t), (x) & sl_C_MASK)
#define sl_C_ISALPHA(x)    ( sl_C_IS(sl_C_ALPHA, x) )
#define sl_C_ISDIGIT(x)    ( sl_C_IS(sl_C_DIGIT, x) )
#define sl_C_ISXDIGT(x)    ( sl_C_IS(sl_C_XDIGT, x) )
#define sl_C_ISALNUM(x)    ( sl_C_ISALPHA(x) && sl_C_ISDIGIT(x) )
#define sl_C_ISCNTRL(x)    ( sl_C_IS(sl_C_CNTRL, x) )
#define sl_C_ISGRAPH(x)    ( sl_C_IS(sl_C_GRAPH, x) )
#define sl_C_ISPRINT(x)    ( sl_C_IS(sl_C_PRINT, x) )
#define sl_C_ISPUNCT(x)    ( sl_C_IS(sl_C_PUNCT, x) )
#define sl_C_ISSPACE(x)    ( sl_C_IS(sl_C_SPACE, x) )
#define sl_C_ISNEWLN(x)    ( sl_C_IS(sl_C_NEWLN, x) )
#define sl_C_ISUPPER(x)    ( sl_C_IS(sl_C_UPPER, x) )
#define sl_C_ISLOWER(x)    ( sl_C_IS(sl_C_LOWER, x) )
#define sl_C_ISVALID(x)    ( sl_C_ISANY(x) || sl_C_IS(sl_C_VALID, x) )
#define sl_C_ISEOS(x)      ( sl_C_IS(sl_C_EOS,   x) )
#define sl_C_ISANY(x)      ( sl_C_IS(sl_C_MASK, x) )
#define sl_C_ISNONE(x)     !sl_C_ISANY(x)


/*
 * [ [ [ TYPE DEFINITIONS ] ] ] ================================================
 */


/* Aliases */
#define slenum            int         /**< Alias for an int intended to hold an enumeration value. */
#define sltbool           int         /**< Alias for an int intended to hold a tri-state boolean value. */
#define slbool            int         /**<  */
#define slecode           int         /**< Alias for an int intended to be a substitute for the @a slecode_e type. */
#define slctypes          slflags_t   /**< Alias for a slflags_t type intended to hold character type flags. */

/* Enumerations */
typedef enum slecode_e    slecode_e;  /**< Error code enumeration. Negative values are informational or warnings, while positive ones are errors. */
typedef enum slhtype_e    slhtype_e;  /**< Enumeration that lists all of the library's handle types. */

/* Immediate types */
typedef long int          slcpoint_t; /**< Type for a signed integer representing a single codepoint. Equivalent to the usage of int in C-style string functions. Depending on the encoding, a character may be composed of more than just one codepoint. This type has at least 32 bits. */
typedef long int          sloff_t;    /**< Type for positional offsets. Though it may not be big enough to represent an integer capable of instructing operations to seek from beginning to end, for most applications it may suffice, and when not, two or more consecutive seeks can be performed to reach the desired position. */
typedef unsigned int      slflags_t;  /**< A simple formalism for an integer supposed to hold bitwise flags. It is guaranteed to hold at least 16 bits. */

/* Essential types */ 
typedef struct slapi_t    slapi_t;    /**< Base for all other API definitions. */
typedef struct slhnd_t    slhnd_t;    /**< A handle that (semi-)opaquely represents any library-supported object. */
typedef struct slopts_t   slopts_t;   /**< Simple POD structure used to pass standardized options on handle creation. */
typedef struct slenc_t    slenc_t;    /**< Represents and implements an encoding (e.g. codepage, etc). */
typedef struct slhandle_a slhandle_a; /**< An API capable of generating handles. */

/* Delegates and signatures */
typedef slecode_e (*slevent_f)  (void* eventp, int what, slhnd_t* who, const char* where, void* ctx); /* Return: <0 - partially recovered, but should continue; =0 - fully recovered, must continue; >0 - not recovered, must abort */
typedef slecode_e (*slfilter_f) (void* filterp, slhnd_t* filterh, size_t* nchars, const char* ibuf, size_t ibufbytesz, char* obuf, size_t* obufbytesz); /* Return: <0 - Nothing to do. =0 - OK. >0 - Error. Specific error code will be put on filterh. */
typedef void*     (*slalloc_f)  (void* allocp, void* ptr, size_t sz);


/*
 * [ [ [ ENUMERATIONS ] ] ] ====================================================
 */


/* Errors as positive numbers.
 * Warnings and events as negative numbers.
 */
enum slecode_e {
    sl_E_USERWARN=-256,/**< Starting number for user-defined warnings. */
	
    sl_E_READY = -6,    /**< Indicates that the stream is ready to be used. */
    sl_E_DATA,          /**< A code indicating presence or traffic of data in the stream. */
	sl_E_AGAIN,         /**< Resource still unavailable, try again. */
	sl_E_PASS,          /**< There is nothing to do, the operation is being ignored. */
	sl_E_UNREG,         /**< The event handler or any other similar entity is being unregistered. */
    sl_E_WARN,          /**< Generic warning indicator. May be raised after error recovery. */
    sl_E_END,            /**< Indicates end-of-stream states (EOF and all the like). */
	
    sl_E_OK,            /**< Operation successful. */
	
	sl_E_RR,            /**< Generic error indicator. */
	sl_E_IMPL,          /**< Operation not implemented or not provided. */
	sl_E_SUPP,          /**< Operation not supported. */
	sl_E_ARGS,          /**< Wrong arguments provided. */
	
	sl_E_USERERR = 256,/**< Starting number for user-defined errors. */
	
	sl_E_MIN = sl_E_DATA,
	sl_E_MAX = sl_E_ARGS
};


enum slhtype_e {
    sl_T_NONE,        /* - */ /**< The handle type is invalid, indiscernible or irrelevant. */
	
	/* (real) Types (although they're actually used as interfaces...) */
	sl_T_HANDLE,      /* _ */ /**< generic methods that must apply for all handle types. */
	sl_T_STREAM,      /* _ */ /**< handle is a stream. */
	sl_T_POS,         /* _ */ /**< handle is a stream position. */
	sl_T_WALKER,      /* c */ /**< handle is an encoding walker state. */
	sl_T_CCONV,       /* c */ /**< handle is an encoding conversion state. */
	sl_T_LOCALE,      /* l */ /**< handle is a locale. A locale has facets (that in this library are merely sub-locales). */
	sl_T_WRAPCALL,    /* w */ /**< handle is an asynchronous call handle. */
	sl_T_FILTER,      /* b */ /**< handle works as a stream filter for another stream. */
	sl_T_BUFFER,      /* b */ /**< handle works as a stream buffer, either standalone or proxied. */
	sl_T_FILE,        /* f */ /**< handle works as a file. */
	sl_T_STRING,      /* s */ /**< handle works as an in-memory string, dynamic or not. */
		
	/* Natures (in reality, just a cute term for Java-like interfaces) */
	sl_T_READABLE,    /* _ */ /**< handle is a readable stream. */
	sl_T_WRITABLE,    /* _ */ /**< handle is a writable stream. */
	sl_T_SEEKABLE,    /* _ */ /**< handle is a seek-enabled stream. */
	sl_T_ASYNC,       /* _ */ /**< handle has an asynchronous operation, or is asynchronous-aware. */
	sl_T_NOTIFIER,    /* _ */ /**< handle can emit events and register event handlers. Based on callbacks. */
	sl_T_LOCAWARE,    /* _ */ /**< handle can be localized. */
	sl_T_ENCAWARE,    /* _ */ /**< handle operates following an encoding. */
	sl_T_WRAPPABLE,   /* _ */ /**< handle is capable of wrapping calls into handlers. */
	
	/* Locale facets */
	sl_T_LCONV,       /* l */ /**< Defines the locale's default encoding and character data for the locale (lconv). Assigning an encoding different from that specified in the locale on a stream may incur an implicit conversion. */
	sl_T_LCOLLATE,
	sl_T_LMONETARY,
	sl_T_LNUMERIC,
	sl_T_LTIME,
	
	sl_T_MAX = sl_T_LTIME
};


/*
 * [ [ [ STRUCTURES AND UNIONS ] ] ] ===========================================
 */


struct slopts_t {
    slapi_t*        api;     /**< API that created the additional information stored at @c apip. */
    void*           apip;    /**< Used by APIs for holding information not contemplated by the structure. When this field is set, the user can consider that the API indicated by the @c api variable has set at least one custom option. */
	
	/* General/Reusable options */
	slhnd_t*        hnd;     /**< Handle to be used in the operation. */
	slenum          mode;    /**< Enumeration-like integer holding the mode or operation to be performed. */
	slflags_t       flags;   /**< Operation flags specifying some details. */
	slcpoint_t      cpoint;  /**< A code point to use on the operation. */
	slctypes_t      ctypes;
	size_t          size;
	const char*     name;
	const wchar_t*  wname;
	slhnd_t*        hname;
    void*           eventp;
    slevent_f       eventf;
	void*           allocp;
	slalloc_f       allocf;	
	sltbool         autoquit; /**< For proxy-type streams (e.g. filter, buffer), if @c sl_yes, their proxied streams are closed when they are. */
	sltbool         async;    /**< Option used to force synchronous or asynchronous operation on streams. Set to @c sl_yes to enable asynchronous functionality, and @c sl_false to force synchronization. */
	
	/* Stream options */
    const char*     encname;
    const slenc_t*  enc;
	const char*     locname;
	const slhnd_t*  loc;
	
	/* Filter and Converter options */
	slfilter_f      filterf;
	void*           filterp;
	slenc_t*        fromenc;
	const char*     fromencname;
	
	/* Event notification options */
	int             evtsignal;
};


/*
 * [ [ [ GLOBAL METHODS ] ] ] ==================================================
 *
 * Some utility methods used to help both users and implementors of the library.
 */


sl_API int            sl_toerrno  (int ecode);
sl_API slopts_t*      sl_initopts (slopts_t* opts, const slapi_t* api);


/*
 * [ [ [ GENERIC HANDLE METHODS ] ] ] ==========================================
 *
 * A handle is a formalism for an object with pre-defined interfaces that the
 * library calls APIs (and that the rest of the world would call classes).
 * Handles may implement any of the APIs listed in the @c slhtype_e enumeration.
 * However, handles are enforced to implement at least the @c sl_T_HANDLE
 * interface API, that provides the minimum information and methods required
 * for handle-based operations (e.g. primary type (the "constructor"), resource
 * deallocation methods).
 *
 * Handles are also required to provide an errno-like state that's query-able by
 * clients. It is hoped that this design promotes (actually, allows for)
 * reentrant handle implementations. The rationale behind these internal states
 * is given by that of @c slecode_e, and is as such:
 * + Zero indicates a valid state.
 * + Positive values indicate error states.
 * + Negative values indicate warnings or events of interest to the client.
 * + Values less than @c sl_E_USERWARN are non-standard and represent
 *   implementation-defined warnings/events.
 * + Values greater than @c sl_E_USERERR are non-standard and represent
 *   implementation-defined errors.
 * Given an error state, the user may try to recover the handle into a valid
 * state through @c sl_recover.
 */


#define               sl_ok(c)    sl_OK(sl_ecode(c))
#define               sl_valid(c) sl_VALID(sl_ecode(c))
#define               sl_warn(c)  sl_WARN(sl_ecode(c))
#define               sl_error(c) sl_ERROR(sl_ecode(c))
#define               sl_eos(hnd) sl_EOS(sl_ecode(c))
#define               sl_eof(hnd) sl_EOF(sl_ecode(c))


sl_API int            sl_htype   (slhnd_t* hnd);
sl_API int            sl_ishtype (slhnd_t* hnd, int htype);
sl_API slecode_e      sl_ecode   (slhnd_t* hnd);
sl_API slecode_e      sl_recover (slhnd_t* hnd);
sl_API void           sl_close   (slhnd_t* hnd);


/*
 * [ [ [ STREAM METHODS ] ] ] ==================================================
 *
 * Streams have a central place in the library. @c sl_T_STREAM enumeration item
 * is the API ID for the stream interface. The enumeration item itself serves to
 * identify the handle as being a stream instance. The stream interface
 * itself is very barebones, as the library considers that not every stream may
 * provide all the functionality the library makes available to the client. In
 * such cases, the library will inspect the handle for proper support.
 */


/* General functions */
sl_API slhnd_t*       sl_open      (const slhandle_a* api, const slopts_t* opts);
sl_API slecode_e      sl_flush     (slhnd_t* hnd); /**< Finishes any pending operations on stream. The most common example is buffer flushing, but the same concept can be extended to other actions (e.g. operation flushing if the stream queues operations). */

/* Encoding-aware (character type) functions */
sl_API const slenc_t* sl_getenc    (slhnd_t* hnd);
sl_API slecode_e      sl_setenc    (slhnd_t* hnd, const slenc_t* enc);
sl_API const char*    sl_getencname(slhnd_t* hnd);
sl_API slecode_e      sl_setencname(slhnd_t* hnd, const char* encname);

/* Locale-aware functions. */
sl_API const slhnd_t* sl_getloc    (slhnd_t* hnd);
sl_API slecode_e      sl_setloc    (slhnd_t* hnd, const slhnd_t* loc);
sl_API const char*    sl_getlocname(slhnd_t* hnd);
sl_API slecode_e      sl_setlocname(slhnd_t* hnd, const char* locname);

/* Output functions */
sl_API slcpoint_t     sl_ungetc    (slhnd_t* hnd, slcpoint_t ch);
sl_API slcpoint_t     sl_putc      (slhnd_t* hnd, slcpoint_t cpoint);
sl_API long           sl_puts      (slhnd_t* hnd, const char* str);
sl_API long           sl_putl      (slhnd_t* hnd, const char* str);
sl_API long           sl_putn      (slhnd_t* hnd, const char* str, size_t bytesz);
sl_API long           sl_putnl     (slhnd_t* hnd, const char* str, size_t bytesz);
sl_API long           sl_printf    (slhnd_t* hnd, const char* fmt, ...);
sl_API long           sl_vprintf   (slhnd_t* hnd, const char* fmt, va_list args);
sl_API long           sl_write     (slhnd_t* hnd, slhnd_t* fromstream, size_t nchars);
sl_API long           sl_writebuf  (slhnd_t* hnd, const char* buf, size_t sz, size_t nchars);

/* Input functions */
sl_API slecode_e      sl_pipe      (slhnd_t* readable, slhnd_t* writable); /**< Available readable data on @c readable is read and then written to @c writable as soon as it arrives. */
sl_API slecode_e      sl_tie       (slhnd_t* readable, slhnd_t* stream); /**< @c stream is flushed before any read operation on @c readable. */
sl_API slcpoint_t     sl_getc      (slhnd_t* hnd);
sl_API slecode_e      sl_gets      (slhnd_t* hnd, char* buf, size_t bytesz);
sl_API slecode_e      sl_get0      (slhnd_t* hnd, char* buf, size_t bytesz); /**< Gets a string up to the first null character found, including any character in-between. */
sl_API slecode_e      sl_scanf     (slhnd_t* hnd, const char* fmt, ...);
sl_API size_t         sl_vscanf    (slhnd_t* hnd, const char* fmt, va_list args);
sl_API slecode_e      sl_read      (slhnd_t* hnd, slhnd_t* tostream, size_t nchars);
sl_API size_t         sl_readbuf   (slhnd_t* hnd, char* buf, size_t bufsz, size_t nchars);

/* Seek and Positioning */
sl_API ssize_t        sl_seek      (slhnd_t* hnd, sloff_t off, int where);
sl_API slhnd_t*       sl_getpos    (slhnd_t* hnd);
sl_API slecode_e      sl_setpos    (slhnd_t* hnd, slhnd_t* pos);


/*
 * [ [ [ ENCODING FUNCTIONS ] ] ] ==============================================
 *
 * These are functions that operates on encodings and their instances.
 * Encoding objects are capable of 
 *
 * Walkers are encoding-based objects that walk through a buffer, codepoint by
 * codepoint. Note that, depending on the encoding, Some considerations about encoding walkers:
 * + Conceptually, walkers are not streams, and therefore, they may not be
 *   operated as such. Although implementations may return a walker handle that
 *   is streamable, it is not recommended.
 * + Walkers are primarily read-only. They should never alter the input buffer
 *   in any way -- unless the walker handle is a streamable object, it's all
 *   up to the implementation.
 * + The speed of the walking can be specified through the number of
 *   characters to walk by -- and in fact implementations are free to optimize
 *   these cases.
 * + As walkers operate with in-memory objects, they must be provided
 *   a raw byte-oriented pointer and its size. Walkers internally maintain a
 *   pointer used to hold its current position in the buffer, and the user can
 *   inspect said pointer. Walkers can be stateful, but that's not a rule.
 * + Walkers may be stateful, if the encoding is complex enough.
 * + Information a walker must provide:
 *   - initial position
 *   - current position
 *   - remaining size
 *   - current code point's size
 *
 * Code converters 
 */


/* Encoding functions */
sl_API 

/* Walker functions */
sl_API slhnd_t*       sl_cwalk     (slhnd_t* hnd, int mode, const char* buf, size_t bufsz, const slopts_t* opts);
sl_API const char*    sl_cstartpos (slhnd_t* walker); /**< Returns the starting pointer of the walker. */
sl_API ssize_t        sl_cstartlen (slhnd_t* walker); /**< Returns the full byte length of the buffer being walked on. */
sl_API const char*    sl_ccurrpos  (slhnd_t* walker); /**< Returns the current position pointer. It always points to the beginning of a valid character. */
sl_API ssize_t        sl_ccurrlen  (slhnd_t* walker); /**< Returns the byte length of the character pointed by the current walker position. */
sl_API ssize_t        sl_cremlen   (slhnd_t* walker); /**< Returns the remaining byte length of the walker buffer. */
sl_API ssize_t        sl_cseek     (slhnd_t* hnd, sloff_t* off, int where);

/* Code converter functions */
sl_API slhnd_t*       sl_cconvert  (slenc_t* from, slenc_t* to, slopts_t* opts);
sl_API ssize_t        sl_cstep     (slhnd_t* cnv, size_t nchars);


/* Walker and code converter functions */
sl_API slecode_e      sl_cinit     (slhnd_t* cnv); /**< Initializes the walker or converter state. */
sl_API slecode_e      sl_cisinit   (slhnd_t* cnv); /**< Checks if the walker or converter is in its initial state. */


/*
 * [ [ [ EVENT FUNCTIONS ] ] ] =================================================
 * 
 * Event handling functions are called when a specific event is raised during
 * any stream operation. Events can be either notifications, warnings or errors.
 * When an error is being treated, the event handler is able to tell the stream
 * whether it can skip, retry or abort the operation. Event handling happens
 * only once when the event is raised; specifically, error handling happens only
 * once so that the stream doesn't try to redo an ever-failing operation.
 *
 * If the stream supports event handling, it must support at least the "global"
 * event handler. The global handler is capable of catching any event that does
 * not have a specific handler assigned through @a sl_on or @a sl_once.
 * Providing any of the latter two functions is opt-in for the stream.
 *
 * The way event handlers are called is defined at construction time through
 * @a slopts_t options. Anyway, depending on how the stream implements its
 * event notification scheme, the user must be aware that the function may be
 * called from a different thread or from a signal handler. Therefore, if a
 * custom state is provided through @c eventp, care must be taken to ensure data
 * integrity (e.g. mutexes, atomic assignments).
 */


sl_API slecode_e      sl_every     (slhnd_t* hnd, slevent_f eventf, void* eventp); /**< Sets up the global event handler. This event handler will be called for every event raised or recovery procedure started on the stream. Contrary to per-errorcode event handlers, this handler position is ensured to be available if the stream supports event handling. */
sl_API slecode_e      sl_on        (slhnd_t* hnd, int event, slevent_f eventf, void* eventp); /**< Sets an event handler for specific error codes. As an example, by assigning a handler for @c sl_EOK, everytime the stream succeeds on some operation, the handler will be called. */
sl_API slecode_e      sl_once      (slhnd_t* hnd, int event, slevent_f eventf, void* eventp); /**< Sets a one-time event handler for specific error codes. The handler will be unregistered as soon as its call finishes. */


/*
 * [ [ [ FILE METHODS ] ] ] ====================================================
 */


#define           sl_fclose(hnd) sl_close(hnd)

sl_API slhnd_t*   sl_fopen     (const slhandle_a* api, const char* filename, slflags_t flags, const slopts_t* opts);
sl_API slhnd_t*   sl_fwopen    (const slhandle_a* api, const wchar_t* filename, slflags_t flags, const slopts_t* opts);
sl_API slhnd_t*   sl_freopen   (slhnd_t* hnd, const char* filename, slflags_t flags, const slopts_t* opts);

sl_API slflags_t  sl_fgetflags (slhnd_t* hnd);
sl_API void*      sl_fgetstdh  (slhnd_t* hnd); /**< Gets the underlying standards-compliant (ANSI C) file handle. */
sl_API int        sl_fgetstdi  (slhnd_t* hnd); /**< Gets the underlying system-dependent file identifier, if available. Useable on Linux systems or others capable of representing opened files as integer identifiers. */
sl_API void*      sl_fgethnd   (slhnd_t* hnd); /**< Gets the underlying system-dependent file handle, if available. */


/*
 * [ [ [ BUFFER AND FILTER METHODS ] ] ] =======================================
 */


sl_API slhnd_t*       sl_filter     (const slhandle_a* api, slhnd_t* hnd, slfilter_f filterf, void* filterp, slopts_t* opts);
sl_API slhnd_t*       sl_filterinto (const slhandle_a* api, slhnd_t* hnd, const char* encname, slopts_t* opts);
sl_API slhnd_t*       sl_filterenc  (const slhandle_a* api, slhnd_t* hnd, slenc_t* enc, slopts_t* opts);
sl_API slhnd_t*       sl_filtercnv  (const slhandle_a* api, slhnd_t* hnd, slhnd_t* cnv, slopts_t* opts);
sl_API slhnd_t*       sl_buffer     (const slhandle_a* api, slhnd_t* hnd, slflags_t bufmode, size_t bufsize, slcpoint_t slopts_t* opts);


/*
 * [ [ [ VARIABLES ] ] ] =======================================================
 */


sl_DATA const slloc_t const* sl_gstdc_loc;    /**< Reflects the global locale currently set through @c setlocale(). */ 
sl_DATA const slloc_t const* sl_ascii_loc;    /**< Reentrant ASCII locale, based on the STDC's C locale. */
sl_DATA const slloc_t const* sl_sl_loc;       /**< A reentrant ASCII-based locale that accepts characters greater than 127. */

sl_DATA const slenc_t const* sl_gstdc_enc;    /**< Placeholder for the codepage or encoding currently set through @c setlocale(). */
sl_DATA const slenc_t const* sl_ascii_enc;    /**< Impleemntation of the ASCII encoding. */

#endif /* SL_H_ */







#if 0

/* Some sketches... */

slopts_t opts = {0};
slhnd_t* file;
opts.filename = "filename.txt";
opts.fileflags = sl_RW | sl_BINARY;
file = sl_open(sl_stdio_api, &opts);


char str = { '\n' };
slhnd_t* file = sl_fopen("path", "opts");
if (!sl_valid(file)) {
    return 0;
}
sl_eset(file, "UTF-8");
sl_fprint(file, "hey you!");
sl_fwrite(file, str, sizeof(str));
sl_fclose(file);

#endif