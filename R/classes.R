setClass("ExternalRef", representation(ref = "externalptr"))

setClass("CSSSelectionEngine", contains = "ExternalRef")

setClass("CSSStyleSheet", representation(docName = "character"), contains = "ExternalRef",
            prototype = list(docName = as.character(NA)))

setClass("CSSStatement", contains = "ExternalRef")
setClass("AT_RULE", contains = "CSSStatement")
setClass("RULESET", contains = "CSSStatement")
setClass("IMPORT_RULE", contains = "CSSStatement")
setClass("MEDIA_RULE", contains = "CSSStatement")
setClass("PAGE_RULE", contains = "CSSStatement")
setClass("CHARSET_RULE", contains = "CSSStatement")
setClass("FONT_FACE_RULE", contains = "CSSStatement")

# R-level objects
setClass("CSSElement")  # generic abstract class

setClass("CSSRuleset", representation(declarations = "list",
                                      selectors = "list" ), 
                 contains = "CSSElement")


setClass("CSSImportRule", contains = "CSSElement",
          representation(url = "character",
                         media = "character",
                         sheet = "CSSStyleSheet"))


setClass("CSSMediaRule", contains = "CSSElement",
           representation(media = "character",
                          rulesets = "list"))

setClass("CSSPageRule", representation(declarations = "list",
                                       name = "character",
                                       pseudo = "character"), 
                 contains = "CSSElement")


setClass("CSSCharsetRule", 
          representation(charset = "character"),
          contains = "CSSElement")

setClass("CSSFontFaceRule", 
          representation(declarations = "list"),
          contains = "CSSElement")


##################################

setClass("CSSDeclaration",
           representation(important = "logical",
                          property = "character",
                          value = "ANY")) # XXX for now. Could restrict to set.

setClass("CSSRGB",
         representation(values = "numeric",
                        is_percentage = "logical",
                        is_transparent = "logical",
                        inherit = "logical"),
           prototype = list(values = numeric(3), is_percentage = FALSE, is_transparent = FALSE, inherit = FALSE))

setClass("CSSSimpleSelector",
          representation(is_case_sensitive = "logical",
                         name = "character",
                         specificity = "integer"))

setClass("CSSComplexSelector",
          contains = "CSSSimpleSelector",
          representation(selectors = "list"))

setClass("CSSQualifiedSelector")
setClass("CSSIDSelector", representation(name = "character"), contains = "CSSQualifiedSelector")
setClass("CSSClassSelector", representation(name = "character"), contains = "CSSQualifiedSelector")
setClass("CSSPseudoSelector", representation(name = "character", extra = "character", type = "integer"), contains = "CSSQualifiedSelector")

setClass("AttributeMatchWay", contains = "integer", prototype = structure(0L, names = "NO_MATCH"))
setValidity("AttributeMatchWay", function(object) {
                                    if(object < 0 || object > 4)
                                      "value out of range (0 through 4)"
                                    else
                                      TRUE
                                 })


  # There can be a list of these within the CRAdditionalSelectorContent
setClass("CSSAttributeSelector", 
            representation(name = "character", 
                           value = "character", 
                           match_way = "AttributeMatchWay"))
