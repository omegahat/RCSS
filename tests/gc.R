library(RCSS)
f = system.file("samples", "test.css", package = "RCSS")
css = readCSS(f)
rm(css)

css = readCSS(f)
a = css
rm(css)
gc()
rm(a)  # free now
gc()  # free now

css = readCSS(f)
gctorture(TRUE)
a = css[[1]]
b = css[[2]]

rm(css)
rm(a)
rm(b)




