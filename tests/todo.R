library(RCSS)
todo = readCSS(system.file("samples", "Todo.css", package = "RCSS"))
asCSSObject(todo[[27]])
