library(RCSS)
f = system.file("samples", "test.css", package = "RCSS")
css = readCSS( f )
length(css)

# Missing the @page
sapply(css, class)
css[]

names(css)

#  
asCSSObject(css[[3]])

 # media
asCSSObject(css[["media"]])
asCSSObject(asCSSObject(css[["media"]])@rulesets[[1]])
