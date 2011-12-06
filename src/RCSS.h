#include <libcroco/cr-om-parser.h>
#include <libcroco/cr-utils.h>
#include <Rinternals.h>
#include <Rdefines.h>

SEXP R_MakeRefObject(void *val, const char * const className, const char * const tag);
void *R_GetRefValue(SEXP r_val, const char * const tag);

SEXP R_MakeStatementRef(const CRStatement *val);
CRStatement *R_GetStatementRef(SEXP r_val);
SEXP R_MakeStyleSheetRef(const CRStyleSheet *sheet);
CRStyleSheet *R_GetStyleSheetRef(SEXP r_val);
SEXP copySelectorList(CRSelector *rule);

