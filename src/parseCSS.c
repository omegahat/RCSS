#include "RCSS.h"


void
R_freeStyleSheet(SEXP r_obj)
{
    CRStyleSheet *sheet;
    sheet = (CRStyleSheet *)R_ExternalPtrAddr(r_obj);
    if(sheet) {
#if 0
	if(sheet->ref_count == 1) {
	    fprintf(stderr, "About to free stylesheet %p\n", sheet);fflush(stderr);
	}
#endif
	cr_stylesheet_unref(sheet);
    }
}

SEXP
R_MakeStyleSheetRef(const CRStyleSheet *sheet)
{
    SEXP ans;
    if(sheet)
	cr_stylesheet_ref(sheet);
    PROTECT(ans = R_MakeRefObject(sheet, "CSSStyleSheet", "CRStyleSheet"));
    R_RegisterCFinalizer(GET_SLOT(ans, Rf_install("ref")), R_freeStyleSheet);
    UNPROTECT(1);
    return(ans);
}

CRStyleSheet *
R_GetStyleSheetRef(SEXP r_val)
{

    CRStyleSheet *sheet;
	/* Check the tag. */
    SEXP val = GET_SLOT(r_val, Rf_install("ref"));
    sheet = (CRStyleSheet* ) R_ExternalPtrAddr(val);

    return(sheet);
}

const char * StatementTypeNames[] = {
    "AT_RULE",
    "RULESET",
    "IMPORT_RULE",
    "MEDIA_RULE",
    "PAGE_RULE",
    "CHARSET_RULE",
    "FONT_FACE_RULE"
};

void
R_freeStatementRef(SEXP r_obj)
{
    CRStatement *val;
    val = (CRStatement *) R_ExternalPtrAddr(r_obj);
    if(val->parent_sheet) {
#if 0
	fprintf(stderr, "%p sheet count %d\n", val->parent_sheet, (int) val->parent_sheet->ref_count);
#endif
	cr_stylesheet_unref(val->parent_sheet);	
    }
}

SEXP
R_MakeStatementRef(const CRStatement *val)
{
    SEXP ans;
    PROTECT(ans = R_MakeRefObject(val, StatementTypeNames[val->type], "CRStatement"));
    if(val->parent_sheet) 
	cr_stylesheet_ref(val->parent_sheet);	
    R_RegisterCFinalizer(GET_SLOT(ans, Rf_install("ref")), R_freeStatementRef);
    UNPROTECT(1);
    return(ans);
}

CRStatement *
R_GetStatementRef(SEXP r_val)
{

    CRStatement *val;
	/* Check the tag. */
    val = (CRStatement* ) R_ExternalPtrAddr(GET_SLOT(r_val, Rf_install("ref")));

    return(val);
}




SEXP
R_parseCSS(SEXP r_input, SEXP r_isFile)
{
    CRStyleSheet *stylesheet = NULL;
    enum CREncoding encoding = CR_ASCII;
    const char * const input = CHAR(STRING_ELT(r_input, 0));
    enum CRStatus status;

    if(LOGICAL(r_isFile)[0]) {
/*	cr_input_new_from_uri(encoding); */
	status = cr_om_parser_simply_parse_file(input, encoding, &stylesheet);
    } else
	status = cr_om_parser_simply_parse_buf(input, strlen(input), encoding, &stylesheet);	

    if(status != CR_OK) {
	return(ScalarInteger(status));
    }
    
    return(R_MakeStyleSheetRef(stylesheet));
}


SEXP
R_getStylesheetLength(SEXP r_sty)
{
    CRStyleSheet *sty;

    sty = R_GetStyleSheetRef(r_sty);
    if(!sty->statements)
	return(ScalarInteger(0));

    return(ScalarInteger(cr_stylesheet_nr_rules(sty)));
}

#if 0
/*XXX Finish off */
SEXP
R_getStatementName(CRStatement *stmt)
{
   unsigned int len = 0;
   SEXP ans;
   char buf[1000];

   switch(stmt->type) {
   case RULESET_STMT:
       ans = copySelectorList(stmt->kind.ruleset->sel_list);
       break;
   case AT_IMPORT_RULE_STMT:
       sprintf(buf, "@import(%s)", cr_string_peek_raw_str(stmt->kind.import_rule->url));
       ans = mkString(buf);
       break;
   }

   return(ans);
}



/*XXX not finished. */
SEXP
R_getCSSNames(SEXP r_sty)
{
    int i, len = 0;
    SEXP ans;
    CRStyleSheet *sty;
    CRStatement *stmts, *tmp;
    sty = R_GetStyleSheetRef(r_sty);
    len = cr_stylesheet_nr_rules(sty);

    PROTECT(ans = allocVector(VECSXP, len));
    for(i = 0, tmp = sty->statements; i < len; i++, tmp = tmp->next) {
	SET_VECTOR_ELT(ans, i, R_getStatementName(tmp));
    }
    UNPROTECT(1);
    return(ans);
}
#endif


SEXP
R_getStylesheetItem(SEXP r_sty, SEXP which)
{
    CRStyleSheet *sty;
    CRStatement *stmt;

    sty = R_GetStyleSheetRef(r_sty);
    stmt = cr_stylesheet_statement_get_from_list(sty, INTEGER(which)[0]);

    return(R_MakeStatementRef(stmt));
}

SEXP
R_importRuleGetUrl(SEXP r_stmt)
{
    CRStatement *stmt;

    stmt = R_GetStatementRef(r_stmt);
    
    return(mkString(cr_string_peek_raw_str(stmt->kind.import_rule->url)));
}

SEXP
R_importRuleGetSheet(SEXP r_stmt)
{
    CRStatement *stmt;

    stmt = R_GetStatementRef(r_stmt);
    
    return(R_MakeStyleSheetRef(stmt->kind.import_rule->sheet));
}


SEXP
R_makeParseLocation(CRParsingLocation *loc)
{
    SEXP ans;
    ans = allocVector(REALSXP, 3);
    REAL(ans)[0] = loc->line;
    REAL(ans)[1] = loc->column;
    REAL(ans)[2] = loc->byte_offset;
    return(ans);

}

SEXP
R_getParseLocation(SEXP r_stmt)
{
   CRStatement *stmt;
   CRParsingLocation *pos;

    stmt = R_GetStatementRef(r_stmt);
    pos = &(stmt->location);
    if(pos->line == 0) {
	
	switch(stmt->type) {
	case RULESET_STMT:
	    pos = &(stmt->kind.ruleset->sel_list->location);
	    break;
	case AT_IMPORT_RULE_STMT:
	    break;
	case AT_MEDIA_RULE_STMT:
	    pos = &(stmt->kind.media_rule->rulesets->location);
	    break;
	case AT_PAGE_RULE_STMT:
	    pos = &(stmt->kind.page_rule->decl_list->location);
	    break;
	case AT_FONT_FACE_RULE_STMT:
	    pos = &(stmt->kind.font_face_rule->decl_list->location);
	    break;
	default:
	    break;
	}
    }
    return(R_makeParseLocation(pos));
}
