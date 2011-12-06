library(RCSS)
library(XML)
css = readCSS( system.file("samples", "OmegaTech.css", package = "RCSS"))
length(css)
a = css[[1]]
a$url

b = newXMLNode("BODY")
code = newXMLNode("code", attrs = c(class = "r"))
rules = getCSSRules(b, css)

x = asCSSObject(rules[[1]])




