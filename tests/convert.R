library(RCSS)
css = readCSS(system.file("samples", "eg.css", package = "RCSS"))
z = lapply(css[], asCSSObject)
