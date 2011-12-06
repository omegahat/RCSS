library(XML)
library(RCSS)
f = system.file("samples", "OmegaTech.css", package = "RCSS")
css = readCSS( f )


b = newXMLNode("BODY")
code = newXMLNode("code", attrs = c(class = "r"))
rules = getCSSRules(b, css)

x = .Call("R_convertStatement", rules[[1]])

x = .Call("R_convertStatement", css[[18]])

