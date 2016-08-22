#include <libcroco/cr-utils.h>
#include "RCSS.h"

SEXP copyDeclaration(CRDeclaration *val, SEXP r_class);
SEXP makeRCRTerm(CRTerm *value);
SEXP copyDeclList(CRDeclaration *val);
SEXP copyRulesetToR(CRStatement *rule);
SEXP copySelectorList(CRSelector *rule);
SEXP convertCRRgbToR(CRRgb *rgb);
SEXP convertTermIdent(CRTerm *val);



SEXP
copyCharsetRuleToR(CRStatement *stmt)
{
    SEXP ans, klass;
    CRAtCharsetRule *r = stmt->kind.charset_rule;
    PROTECT(klass = MAKE_CLASS("CSSCharsetRule"));
    PROTECT(ans = NEW(klass));
    ans = SET_SLOT(ans, Rf_install("charset"), mkString(cr_string_peek_raw_str(r->charset)));
    UNPROTECT(2);
    return(ans);
}

SEXP
copyPageRuleToR(CRStatement *stmt)
{
    SEXP ans, klass;
    CRAtPageRule *r = stmt->kind.page_rule;
    PROTECT(klass = MAKE_CLASS("CSSPageRule"));
    PROTECT(ans = NEW(klass));
    PROTECT(ans = SET_SLOT(ans, Rf_install("name"), mkString(r->name ? cr_string_peek_raw_str(r->name) : "")));
    PROTECT(ans = SET_SLOT(ans, Rf_install("pseudo"), mkString(r->pseudo ? cr_string_peek_raw_str(r->pseudo) : "")));
    PROTECT(ans = SET_SLOT(ans, Rf_install("declarations"), copyDeclList(r->decl_list)));
    UNPROTECT(5);
    return(ans);
}


SEXP
copyFontFaceRuleToR(CRStatement *stmt)
{
    SEXP ans, klass;
    CRAtFontFaceRule *r = stmt->kind.font_face_rule;
    PROTECT(klass = MAKE_CLASS("CSSFontFaceRule"));
    PROTECT(ans = NEW(klass));
    PROTECT(ans = SET_SLOT(ans, Rf_install("declarations"), copyDeclList(r->decl_list)));
    UNPROTECT(3);
    return(ans);
}

SEXP
copyStatementRefsToList(CRStatement *els)
{
    SEXP ans;
    int len = 0, i;
    CRStatement *tmp = els;
    for( ; tmp ; len++, tmp = tmp->next) {}

    PROTECT(ans = allocVector(VECSXP, len));
    for(i = 0; i < len; i++, els = els->next) {
	SET_VECTOR_ELT(ans, i, R_MakeStatementRef(els));
    }
    UNPROTECT(1);
    return(ans);
}

SEXP
convertGListStrings(GList const *els)
{
    int i, len = 0;
    SEXP ans;
    const GList *tmp = els;
    for( ; tmp; len++, tmp = tmp->next) {}

    PROTECT(ans = allocVector(STRSXP, len));
    for(tmp = els, i = 0; i < len; i++, tmp = tmp->next) {
	CRString *str = (CRString *) tmp->data;
	SET_STRING_ELT(ans, i, mkChar(cr_string_peek_raw_str(str)));
    }
    UNPROTECT(1);
    return(ans);
}

SEXP
copyMediaRuleToR(CRStatement *stmt)
{
    SEXP ans, klass;
    CRAtMediaRule *r = stmt->kind.media_rule;
    PROTECT(klass = MAKE_CLASS("CSSMediaRule"));
    PROTECT(ans = NEW(klass));
    PROTECT(ans = SET_SLOT(ans, Rf_install("rulesets"), copyStatementRefsToList(r->rulesets)));
    PROTECT(ans = SET_SLOT(ans, Rf_install("media"), convertGListStrings(r->media_list)));

    UNPROTECT(4);
    return(ans);
}

SEXP
copyImportRuleToR(CRStatement *stmt)
{
    SEXP ans, klass;
    CRAtImportRule *r = stmt->kind.import_rule;
    PROTECT(klass = MAKE_CLASS("CSSImportRule"));
    PROTECT(ans = NEW(klass));
    PROTECT(ans = SET_SLOT(ans, Rf_install("sheet"), R_MakeStyleSheetRef(r->sheet)));
    PROTECT(ans = SET_SLOT(ans, Rf_install("url"), mkString(r->url ? cr_string_peek_raw_str(r->url) : "")));
    PROTECT(ans = SET_SLOT(ans, Rf_install("media"), convertGListStrings(r->media_list)));

    UNPROTECT(5);
    return(ans);
}



SEXP
R_convertStatement(SEXP r_stmt)
{
    CRStatement *stmt;
    SEXP ans = R_NilValue;
    stmt = R_GetStatementRef(r_stmt);
    switch(stmt->type) {
    case RULESET_STMT:
	ans = copyRulesetToR(stmt);
	break;
    case AT_RULE_STMT:
//XXXX
	break;
    case AT_IMPORT_RULE_STMT:
	ans = copyImportRuleToR(stmt);
	break;
    case AT_MEDIA_RULE_STMT:
	ans = copyMediaRuleToR(stmt);
	break;
    case AT_PAGE_RULE_STMT:
	ans = copyPageRuleToR(stmt);
	break;
    case AT_CHARSET_RULE_STMT:
	ans = copyCharsetRuleToR(stmt);
	break;
    case AT_FONT_FACE_RULE_STMT:
	ans = copyFontFaceRuleToR(stmt);
	break;
    
    }
    return(ans);
}

SEXP
copyRulesetToR(CRStatement *rule)
{
    SEXP klass, ans;

    PROTECT(klass = MAKE_CLASS("CSSRuleset"));
    PROTECT(ans = NEW(klass));
    PROTECT(ans = SET_SLOT(ans, Rf_install("declarations"), copyDeclList(rule->kind.ruleset->decl_list)));
    PROTECT(SET_SLOT(ans, Rf_install("selectors"), copySelectorList(rule->kind.ruleset->sel_list)));

    UNPROTECT(4);

    return(ans);
}

SEXP
copyDeclList(CRDeclaration *val)
{
    int i, len;
    SEXP ans, klass, names;
    CRDeclaration *tmp = val;

    for(len = 0; tmp ; tmp = tmp->next, len++) {}

    PROTECT(ans = allocVector(VECSXP, len));
    PROTECT(klass = MAKE_CLASS("CSSDeclaration"));
    PROTECT(names = allocVector(STRSXP, len));
    for(i = 0; i < len; i++, val = val->next) {
	SET_VECTOR_ELT(ans, i, copyDeclaration(val, klass));
	SET_STRING_ELT(names, i, mkChar(cr_string_peek_raw_str(val->property)));
    }

    SET_NAMES(ans, names);
    UNPROTECT(3);
    return(ans);
}


/* Generated from 
  tu = parseTU("libcroco.c.t00.tu")
  e = getEnumerations(tu)
  num = resolveType(e[["CRNumType"]], tu)
  cat(paste(dQuote(gsub("^NUM_", "", names(num@values))), collapse = ",\n"))
 */
const char * const CRNumTypes [] = {
"AUTO",
"GENERIC",
"LENGTH_EM",
"LENGTH_EX",
"LENGTH_PX",
"LENGTH_IN",
"LENGTH_CM",
"LENGTH_MM",
"LENGTH_PT",
"LENGTH_PC",
"ANGLE_DEG",
"ANGLE_RAD",
"ANGLE_GRAD",
"TIME_MS",
"TIME_S",
"FREQ_HZ",
"FREQ_KHZ",
"PERCENTAGE",
"INHERIT",
"UNKNOWN_TYPE",
"NB_NUM_TYPE"
};

SEXP
makeRCRTerm(CRTerm *val)
{
    SEXP ans, arr;
    int numProtect = 0;
    CRTerm *tmp = val;
    int i, len = 0;

    while(tmp) { len++; tmp = tmp->next;}

    PROTECT(arr = allocVector(VECSXP, len));

    for(i = 0 ; i < len; i++,  val = val->next) {
	numProtect = 0;
	switch(val->type) {
	case TERM_NUMBER:
	    PROTECT(ans = ScalarReal(val->content.num->val));
	    SET_NAMES(ans, mkString(CRNumTypes[val->content.num->type]));
	    numProtect++;
	    break;
	case TERM_RGB:
	    ans = convertCRRgbToR(val->content.rgb);
	    break;
	case TERM_STRING:
	case TERM_URI:
	    ans = mkString(cr_string_peek_raw_str(val->content.str));
	    break;
	case TERM_IDENT: 
	    ans = convertTermIdent(val);
	    break;
	case TERM_HASH:
	    PROTECT(ans = mkString(cr_string_peek_raw_str(val->content.str)));
	    numProtect++;
	    SET_CLASS(ans, mkString("HexadecimalString"));
	    break;
	case TERM_FUNCTION:
	case TERM_UNICODERANGE: {
	    guchar *str = cr_term_to_string(val);
//XXX convert guchar to R string.
	    PROTECT(ans = ScalarString(str ? mkChar((char *) str) : NA_STRING));
	    numProtect++;
	    SET_CLASS(ans, ScalarString(mkChar( val->type == TERM_FUNCTION ? "TermFunction" : "UnicodeRange")));
	    free(str);
	    break;
	}
	default:
	    PROBLEM "Unhandled conversion %d\n", val->type
		WARN;
//	    REprintf("Unhandled conversion %d\n", val->type);
	    ans = R_NilValue;
	}

	SET_VECTOR_ELT(arr, i, ans);
	UNPROTECT(numProtect);
    }
    UNPROTECT(1);
    return(arr);
}

SEXP
convertTermIdent(CRTerm *val)
{
//    int i, len = 0;
    CRTerm *tmp = val;
//    SEXP ans;

    return(mkString(cr_string_peek_raw_str(tmp->content.str)));
}


SEXP
copyDeclaration(CRDeclaration *val, SEXP r_class)
{
    SEXP ans;
   PROTECT(ans = NEW(r_class));  
   PROTECT(ans = SET_SLOT(ans, Rf_install("important"), ScalarLogical(val->important)));
   PROTECT(ans = SET_SLOT(ans, Rf_install("property"), mkString(cr_string_peek_raw_str(val->property))));
   PROTECT(ans = SET_SLOT(ans, Rf_install("value"), makeRCRTerm(val->value)));

   UNPROTECT(4);
   return(ans);
}

/*XXX What about the next and prev */
SEXP
convertAttrSel(CRAttrSel *val)
{
    SEXP ans, klass;
    PROTECT(klass = MAKE_CLASS("CSSAttributeSelector"));
    PROTECT(ans = NEW(klass));
    PROTECT(ans = SET_SLOT(ans, Rf_install("name"), mkString(cr_string_peek_raw_str(val->name))));
    PROTECT(ans = SET_SLOT(ans, Rf_install("value"), mkString(cr_string_peek_raw_str(val->value))));
    PROTECT(ans = SET_SLOT(ans, Rf_install("match_way"), ScalarInteger(val->match_way)));

    UNPROTECT(5);
    return(ans);
}

SEXP
convertPseudo(CRPseudo *val)
{
    SEXP ans, klass;
    int i = 0;
    PROTECT(klass = MAKE_CLASS("CSSPseudoSelector"));i++;
    PROTECT(ans = NEW(klass));i++;
    PROTECT(ans = SET_SLOT(ans, Rf_install("type"), ScalarInteger(val->type)));i++;
    PROTECT(ans = SET_SLOT(ans, Rf_install("name"), mkString(cr_string_peek_raw_str(val->name))));i++;
    if(val->extra && cr_string_peek_raw_str(val->extra)) {
	PROTECT(ans = SET_SLOT(ans, Rf_install("extra"), mkString(cr_string_peek_raw_str(val->extra))));i++;
    }
    UNPROTECT(i);

    return(ans);
}

SEXP
convertCRAdditionalSelectorContent(CRAdditionalSel *val)
{
    SEXP ans, klass;
    int i = 0;

    switch(val->type) {
	case CLASS_ADD_SELECTOR:
	    PROTECT(klass = MAKE_CLASS("CSSClassSelector"));i++;
	    PROTECT(ans = NEW(klass)); i++;
	    ans = SET_SLOT(ans, Rf_install("name"), mkString(cr_string_peek_raw_str(val->content.class_name)));
	    break;
	case PSEUDO_CLASS_ADD_SELECTOR:
	    ans = convertPseudo(val->content.pseudo);
	    break;
	case ID_ADD_SELECTOR:
	    PROTECT(klass = MAKE_CLASS("CSSIDSelector"));i++;
	    PROTECT(ans = NEW(klass)); i++;
	    ans = SET_SLOT(ans, Rf_install("name"), mkString(cr_string_peek_raw_str(val->content.id_name)));
	    break;
	case ATTRIBUTE_ADD_SELECTOR:
	    ans = convertAttrSel(val->content.attr_sel);
	    break;

        default:
	    ans = R_NilValue;
    }

    if(i)
	UNPROTECT(i);

    return(ans);
}

const  char * const SelectorCombinatorOps[] = {
    "",
    " ",
    "+",
    ">"
};

SEXP
convertSimpleSelector(CRSimpleSel *p, SEXP klass, int recursive)
{
    int i = 0;
    SEXP ans;

    if(p->add_sel || p->next) {
	PROTECT(klass = MAKE_CLASS("CSSComplexSelector")); i++;
    }

    PROTECT(ans = NEW(klass));i++;

    PROTECT(ans = SET_SLOT(ans, Rf_install("is_case_sensitive"), ScalarLogical(p->is_case_sentive)));i++;
    if(p->name) {
       PROTECT(ans = SET_SLOT(ans, Rf_install("name"), mkString(cr_string_peek_raw_str(p->name))));
       i++;
    }
    PROTECT(ans = SET_SLOT(ans, Rf_install("specificity"), ScalarInteger(p->specificity)));i++;

    if(p->add_sel) {
	int ctr = 0;
	SEXP tmp;
	CRAdditionalSel *ptr = p->add_sel;
	for(; ptr; ptr = ptr->next, ctr++) {}

	PROTECT(tmp = allocVector(VECSXP, ctr));i++;
	for(ctr = 0, ptr = p->add_sel; ptr; ptr = ptr->next, ctr++)
	    SET_VECTOR_ELT(tmp, ctr, convertCRAdditionalSelectorContent(ptr));

	PROTECT(ans = SET_SLOT(ans, Rf_install("selectors"), tmp));i++;
    }

    if(recursive && p->next) {
	int ctr = 0;
	SEXP tmp, ops;
	CRSimpleSel *ptr = p->next;
	for( ; ptr ; ptr = ptr->next, ctr++) {}

	PROTECT(tmp = allocVector(VECSXP, ctr));i++;
	PROTECT(ops = allocVector(STRSXP, ctr)); i++;
	for(ctr = 0, ptr = p->next; ptr; ptr = ptr->next, ctr++) {
	    SET_VECTOR_ELT(tmp, ctr, convertSimpleSelector(ptr, klass, 0));
	    SET_STRING_ELT(ops, ctr, mkChar(SelectorCombinatorOps[ptr->combinator]));
	}
	Rf_setAttrib(tmp, Rf_install("ops"), ops);
	PROTECT(ans = SET_SLOT(ans, Rf_install("selectors"), tmp)); i++;
    }

    UNPROTECT(i);
    return(ans);
}

SEXP
convertSelector(CRSelector *sel, SEXP klass)
{
    CRSimpleSel *p = sel->simple_sel;
    return(convertSimpleSelector(p, klass, 1));
}

SEXP 
copySelectorList(CRSelector *rule)
{
    int i, len = 0;
    SEXP ans, klass;
    CRSelector *tmp = rule;

    while(tmp) { len++; tmp = tmp->next;}

    
    PROTECT(ans = allocVector(VECSXP, len));
    PROTECT(klass = MAKE_CLASS("CSSSimpleSelector"));
    for(i = 0; i < len; i++, rule = rule->next) {
	SET_VECTOR_ELT(ans, i, convertSelector(rule, klass));
    }
    UNPROTECT(2);

    return(ans);
}


SEXP
convertCRRgbToR(CRRgb *rgb)
{
    SEXP klass, ans;
    double *els;
    PROTECT(klass = MAKE_CLASS("CSSRGB"));
    PROTECT(ans = NEW(klass));
    PROTECT(ans = SET_SLOT(ans, Rf_install("is_percentage"), ScalarLogical(rgb->is_percentage)));
    PROTECT(ans = SET_SLOT(ans, Rf_install("inherit"), ScalarLogical(rgb->inherit)));
    PROTECT(ans = SET_SLOT(ans, Rf_install("is_transparent"), ScalarLogical(rgb->is_transparent)));
    
    els = REAL(GET_SLOT(ans, Rf_install("values")));
    els[0] = rgb->red;
    els[1] = rgb->green;
    els[2] = rgb->blue;

    UNPROTECT(5);
    return(ans);
}
