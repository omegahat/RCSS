#include <libcroco/cr-sel-eng.h>
#include <libcroco/cr-utils.h>
#include "RCSS.h"


void
R_freeSelectionEngine(SEXP obj)
{
    CRSelEng *eng = (CRSelEng*)  R_ExternalPtrAddr(obj);
    if(eng) {
	cr_sel_eng_destroy(eng);	
    }
    R_ClearExternalPtr(obj);
}
SEXP
R_createSelectionEngine()
{
  CRSelEng *eng;
  SEXP ans;

  eng = cr_sel_eng_new();
  PROTECT(ans = R_MakeRefObject(eng, "CSSSelectionEngine", "CRSelEng"));
  R_RegisterCFinalizer(GET_SLOT(ans, Rf_install("ref")), R_freeSelectionEngine);
  UNPROTECT(1);
  return(ans);
}

SEXP
R_getMatchedRulesets(SEXP r_engine, SEXP r_sheet, SEXP r_node)
{
    CRSelEng *eng;
    CRStyleSheet *sheet;
    xmlNodePtr node;
    gulong len = 0;
    CRStatement **rules = NULL;
    enum CRStatus status;
    int i;
    SEXP ans;

    if(Rf_length(r_engine))
	eng = R_GetRefValue(r_engine, "CRSelEng");
    else
	eng = cr_sel_eng_new();

    sheet = R_GetStyleSheetRef(r_sheet);
    node = (xmlNodePtr) R_ExternalPtrAddr(r_node);

    status = cr_sel_eng_get_matched_rulesets(eng, sheet, node, &rules, &len);

    if(!Rf_length(r_engine)) 
	cr_sel_eng_destroy(eng);

    if(status != CR_OK) {
	PROBLEM "failed to match CSS rules to XML node"
         ERROR;
    }

    PROTECT(ans = allocVector(VECSXP, len));
    for(i = 0 ; i < len; i++) {
	SET_VECTOR_ELT(ans, i, R_MakeStatementRef(rules[i]));
    }
    UNPROTECT(1);
    free(rules);

    return(ans);
}

