\name{getCSSRules}
\alias{getCSSRules}
\title{Find the CSS element that applies to an XML/HTML node}
\description{
  This is the function that allows us to lookup/resolve
  the CSS element that would apply to a particular XML/HTML
  element.  We can take XML nodes created using the
  \code{XML} package.
}
\usage{
getCSSRules(nodes, sheet, eng = NULL)
}
\arguments{
  \item{nodes}{an object of class \code{XMLInternalNode} for which we
    want to find the applicable CSS element(s).}
  \item{sheet}{the \code{\link{CSSStyleSheet-class}} in which to resolve
  the CSS element(s) for the given node.}  
  \item{eng}{the CSS selection engine. This can be created once and used
  in multiple, separate calls to this function. This will improve
  performance marginally.}
}
\value{
  A list of \code{CSSStatement} objects.
}

\references{
  libcroco at \url{http://www.freespiders.org/projects/libcroco}
  Cascading Style Sheets \url{http://www.w3.org/Style/CSS/}
}
\author{
Duncan Temple Lang
}

\seealso{
  \code{\link{readCSS}}
  \code{\link[XML]{xmlParse}},  \code{\link[XML]{getNodeSet}},   \code{\link[XML]{newXMLNode}}
  \code{\link{asCSSObject}}
}
\examples{
css = readCSS( system.file("samples", "OmegaTech.css", package =  "RCSS"))

if(require(XML)) {
 b = newXMLNode("BODY")
 code = newXMLNode("code", attrs = c(class = "r"))
 rules = getCSSRules(b, css)

 x = asCSSObject(rules[[1]])
}

if(require(XML)) {
  css = readCSS( system.file("samples", "descendant.css", package =  "RCSS"))

  node = newXMLNode("a", newXMLNode("b", newXMLNode("c", "some text")))
     # nothing matches the top-level node "a"
  getCSSRules(node, css)
     # for b we get the match a > b
  r = getCSSRules(node[[1]], css)
  asCSSObject(r[[1]])

     # a c
  r = getCSSRules(node[[1]][[1]], css)
  asCSSObject(r[[1]])

   # All the nodes in a document.
  doc = newXMLDoc(node = node, addFinalizer = FALSE)
  allNodes = getNodeSet(doc, "//*")
  lapply(allNodes, getCSSRules, css)

  # We can create the selection engine just once

  eng = .Call("R_createSelectionEngine")
  lapply(allNodes, getCSSRules, css, eng)
}
}
\keyword{IO}
\keyword{programming}

