#include "RCSS.h"

SEXP
R_MakeRefObject(void *val, const char * const className, const char * const tag)
{
    SEXP ans, klass, e;
    PROTECT(klass = MAKE_CLASS(className));
    PROTECT(ans = NEW(klass));
    PROTECT(e = R_MakeExternalPtr(val, Rf_install(tag), R_NilValue));
    ans = SET_SLOT(ans, Rf_install("ref"), e);
    UNPROTECT(3);
    return(ans);
}

void *
R_GetRefValue(SEXP r_val, const char * const tag)
{
    void *p;
    SEXP ref;
    ref =  GET_SLOT(r_val, Rf_install("ref"));
    p = R_ExternalPtrAddr(ref);
    return(p);
}
