#include "sl.h"

static size_t     sl_ctrait_ascii_seek    (sl_Ctrait* T, char *s, size_t sz, sl_Off off, char **pos);
static sl_Cpoint  sl_ctrait_ascii_cpoint  (sl_Ctrait* T, char *s, size_t sz);
static size_t     sl_ctrait_ascii_tostr   (sl_Ctrait* T, sl_Cpoint cpoint, char *s, size_t sz);
static sl_Ctypes  sl_ctrait_ascii_ctypes  (sl_Ctrait* T, sl_Cpoint cpoint);
static sl_Cpoint  sl_ctrait_ascii_tolower (sl_Ctrait* T, sl_Cpoint cpoint);
static sl_Cpoint  sl_ctrait_ascii_toupper (sl_Ctrait* T, sl_Cpoint cpoint);
static int        sl_ctrait_ascii_cmp     (sl_Ctrait* T, const char* a, size_t a_sz, const char* b, size_t b_sz);
static size_t     sl_ctrait_ascii_xfrm    (sl_Ctrait* T, const char* from, size_t from_sz, char* to, size_t to_sz);


static sl_Ctrait sl_ctrait_ascii_i = {
	/* const sl_Ctrait* base    */ NULL
	/* const char*      name    */ , "C"
    /* const size_t     minsize */ , sizeof(char)
    /* const size_t     maxsize */ , sizeof(char)
    /* const sl_Cpoint  eof     */ , (-1)

	/* size_t    (*seek)     */ , sl_ctrait_ascii_seek
	/* sl_Cpoint (*cpoint)   */ , sl_ctrait_ascii_cpoint
	/* size_t    (*tostr)    */ , sl_ctrait_ascii_tostr
	/* sl_Ctypes (*ctypes)   */ , sl_ctrait_ascii_ctypes
	/* sl_Cpoint (*tolower)  */ , sl_ctrait_ascii_tolower
	/* sl_Cpoint (*toupper)  */ , sl_ctrait_ascii_toupper
	/* int       (*cmp)      */ , sl_ctrait_ascii_cmp
	/* size_t    (*xfrm)     */ , sl_ctrait_ascii_xfrm
};

const sl_Ctrait* const sl_ctrait_ascii = &sl_ctrait_ascii_i;


/* TODO Implementation */
size_t    
sl_ctrait_ascii_seek    (
	sl_Ctrait* T, char *s, size_t sz, sl_Off off, char **pos
) {
	
	size_t ret = (size_t)0;
	return ret;
}


/* TODO Implementation */
sl_Cpoint 
sl_ctrait_ascii_cpoint  (
	sl_Ctrait* T, char *s, size_t sz
) {
	
	sl_Cpoint ret = (sl_Cpoint )0;
	return ret;
}


/* TODO Implementation */
size_t    
sl_ctrait_ascii_tostr   (
	sl_Ctrait* T, sl_Cpoint cpoint, char *s, size_t sz
) {
	
	size_t ret = (size_t)0;
	return ret;
}


/* TODO Implementation */
sl_Ctypes 
sl_ctrait_ascii_ctypes  (
	sl_Ctrait* T, sl_Cpoint cpoint
) {
	
	sl_Ctypes ret = (sl_Ctypes)0;
	return ret;
}


/* TODO Implementation */
sl_Cpoint 
sl_ctrait_ascii_tolower (
	sl_Ctrait* T, sl_Cpoint cpoint
) {
	
	sl_Cpoint ret = (sl_Cpoint)0;
	return ret;
}


/* TODO Implementation */
sl_Cpoint 
sl_ctrait_ascii_toupper (
	sl_Ctrait* T, sl_Cpoint cpoint
) {
	
	sl_Cpoint ret = (sl_Cpoint)0;
	return ret;
}


/* TODO Implementation */
int       
sl_ctrait_ascii_cmp     (
	sl_Ctrait* T, const char* a, size_t a_sz, const char* b, size_t b_sz
) {
	
	int ret = (int)0;
	return ret;
}


/* TODO Implementation */
size_t    
sl_ctrait_ascii_xfrm    (
	sl_Ctrait* T, const char* from, size_t from_sz, char* to, size_t to_sz
) {
	
	size_t ret = (size_t)0;
	return ret;
}

