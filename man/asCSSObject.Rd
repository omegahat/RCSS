\name{asCSSObject}
\alias{asCSSObject}
\alias{asCSSObject,CSSStatement-method}
\title{Convert reference to C-level CSS object to a regular R object}
\description{
  This generic function and its methods perform the conversion
  of internal C-level objects to corresponding R objects
  with the information available directly.
}
\usage{
asCSSObject(x, ...)
}
\arguments{
  \item{x}{an object which is a sub-class of \code{\link{CSSStatement-class}}.}
  \item{\dots}{ additional parameters for methods, but not typically used.}
}
\value{
 An object of general base class \code{CSSElement}.
}
\references{
  libcroco at \url{http://www.freespiders.org/projects/libcroco}
  Cascading Style Sheets \url{http://www.w3.org/Style/CSS/}
}
\author{Duncan Temple Lang}

\seealso{
 \code{\link{readCSS}}
}
\examples{
 css = readCSS(system.file("samples", "test.css", package = "RCSS"))
 sapply(css, asCSSObject)
}
\keyword{IO}
\keyword{programming}
\concept{meta-programming}
