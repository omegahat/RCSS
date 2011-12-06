\name{readCSS}
%\Rdversion{1.1}
\alias{readCSS}
\title{Read/parse a Cascading Style Sheet}
\description{
  This reads a CSS document into R.
 The document can be the name of a file or 
 as the text/content of a CSS.
}
\usage{
readCSS(src, asText = inherits(src, "AsIs"))
}
\arguments{
  \item{src}{the name of the CSS file or the CSS content}
  \item{asText}{a logical value indicating whether \code{src} is
    the actual content of the CSS or (\code{FALSE}) is the name of the file}
}
\details{
 This uses libcroco to parse the CSS
}
\value{
 An object of class CSSStyleSheet
}
\references{
  libcroco at \url{http://www.freespiders.org/projects/libcroco}

  Cascading Style Sheets \url{http://www.w3.org/Style/CSS/}
}
\author{
Duncan Temple Lang
}

\seealso{

}
\examples{

 f = system.file("samples", "eg.css", package = "RCSS")
 css = readCSS(f)

 if(require(RCurl)) {
    txt = getURL("http://www.omegahat.org/OmegaTech.css")
    css = readCSS(txt, TRUE)
    css = readCSS(I(txt))
 }


 if(require(XML)) {
   f = system.file("samples", "bob.svg", package = "RCSS")
   doc = xmlParse(f)
   node = getNodeSet(doc, "//x:style[@type='text/css']", "x")[[1]]
   css = readCSS(I(xmlValue(node)))
 }
}
\keyword{IO}

