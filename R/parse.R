
readCSS =
function(src, asText = inherits(src, "AsIs"))
{
   if(!asText) {
     src = path.expand(src)

     if(!file.exists(src))
       stop("no such CSS file. If this is CSS text, use asText = TRUE")
   }
   
   ans = .Call("R_parseCSS", src, !asText)
   if(is.character(src) && !asText)
     ans@docName = src
   
   ans
}

setMethod("length", "CSSStyleSheet",
           function(x) {
             .Call("R_getStylesheetLength", x)
           })

setMethod("[[", c("CSSStyleSheet", "character"),
          function(x, i, j, ...) {
             k = pmatch(i, names(x))
             if(is.na(k))
               return(NULL)
             
              x[[k]]             
          })


setMethod("[[", c("CSSStyleSheet", "numeric"),
           function(x, i, j, ...) {
	      if(i == 0)
                return(NULL)
	     if(i < 1 || i > length(x))
                 stop("index out of range")

             .Call("R_getStylesheetItem", x, as.integer(i - 1))
           })

setMethod("[", "CSSStyleSheet",
           function(x, i, j, ..., drop = NA) {
              if(missing(i) && missing(j)) {
               if(length(x) == 0)
                  return(list())
               return(lapply(seq(length = length(x)), function(i) x[[i]]))
              }

              if(!missing(j))
                i = c(i, j)

             lapply(i, function(i) x[[i]])               
           })


setMethod("[", c("CSSStyleSheet", "character"),
          function(x, i, j, ..., drop = NA) {
             if(!missing(j))
               i = c(i, j, unlist(list(...)))
             
             k = pmatch(i, names(x))
             if(all(is.na(k)))
               return(NULL)
              structure(x[k], names = names(x)[k])
          })


IMPORT_RULE_ELEMENTS = c("url", "sheet", "media_list")

setMethod("$", "IMPORT_RULE",
           function(x, name) {
              i = pmatch(name, IMPORT_RULE_ELEMENTS)
              if(is.na(i))
                 stop("No such field in ", class(x))

              switch(i,
                     .Call("R_importRuleGetUrl", x),                    
                     .Call("R_importRuleGetSheet", x),
                     .Call("R_importRuleGetMediaList", x))
           })

setMethod("names", "CSSStyleSheet",
            function(x){
              if(length(x) == 0)
                 return(NULL)

              els = lapply(x[], asCSSObject)
              sapply(els, asCSSName)
            })

setGeneric("asCSSName", function(x) standardGeneric("asCSSName"))
setMethod("asCSSName", "CSSCharsetRule", function(x) "charset")

setMethod("asCSSName", "CSSQualifiedSelector", function(x) x@name)

setMethod("asCSSName", "CSSElement", function(x) tolower(gsub("(CSS|Statement|Rule)", "", class(x))))

setMethod("asCSSName", "CSSRuleset",
                function(x) paste(sapply(x@selectors, asCSSName), collapse = ", "))

setMethod("asCSSName", "CSSSimpleSelector",
                function(x) x@name)
setMethod("asCSSName", "CSSPseudoSelector",
                function(x) { # paste(x@name, paste(x@extra, collapse = " "), sep = ":")
                      x@name
          })

setMethod("asCSSName", "CSSComplexSelector",
                function(x) {
                   ops = attr(x@selectors, "ops")
	           if(length(ops) == 0)
                      ops = rep(".", length(x@selectors))

                   isPseudo = sapply(x@selectors, is, "CSSPseudoSelector")
                   ids = sapply(x@selectors, asCSSName)
                   if(any(isPseudo))
                     ids[isPseudo] = paste(":", ids[isPseudo], sep = "")
                   paste(x@name, paste(ops, ids, collapse = " "))
               })


setMethod("lapply", "CSSStyleSheet",
           function(X, FUN, ...) {
             lapply(X[], FUN, ...)
           })

setMethod("sapply", "CSSStyleSheet",
           function(X, FUN, ..., simplify = TRUE, USE.NAMES = TRUE) {
             sapply(X[], FUN, ..., simplify = simplify, USE.NAMES = USE.NAMES)
           })

##############

setGeneric("asCSSObject",
           function(x, ...) {
             standardGeneric("asCSSObject")
           })

setMethod("asCSSObject", "CSSStatement",
           function(x, ...) {
             .Call("R_convertStatement", x)
           })


###################

setGeneric("getPos", function(x, ...) standardGeneric("getPos"))

setMethod('getPos', 'CSSStatement',
            function(x, ...)  {
              .Call("R_getParseLocation", x)
            })
  


############################

setGeneric("getDeclaration",
            function(x, what, default = NA, simplify = TRUE)
               standardGeneric("getDeclaration"))

setMethod("getDeclaration",
           "RULESET",
             function(x, what, default = NA, simplify = TRUE) {
                getDeclaration(asCSSObject(x), what, default, simplify)
             })

setMethod("getDeclaration",
           "IMPORT_RULE",
             function(x, what, default = NA, simplify = TRUE) {
                default
             })

setMethod("getDeclaration",
           "CSSRuleset",          
           function(x, what, default = NA, simplify = TRUE)  {

             val = x@declarations[[what]]
             if(is.null(val))
               default
             else {
               val = val@value
               if(length(val) == 1)
                 val[[1]]
               else
                 val
             }
           })

setMethod("getDeclaration",
           "CSSStyleSheet",
             function(x, what, default = NA, simplify = TRUE) {
                sapply(x, getDeclaration, what, default, simplify)
             })
