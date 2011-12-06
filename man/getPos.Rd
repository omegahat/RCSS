\name{getPos}
\alias{getPos}
\alias{getPos,CSSStatement-method}
\title{Get the location in the CSS file of a CSS element}
\description{
  This function gets the line and column number of a given
  CSS element within a CSS document.
}
\usage{
getPos(x, ...)
}
\arguments{
  \item{x}{the CSS element whose location is of interest. This has to be
           a reference to a C-level object at present.}
  \item{\dots}{additional parameters for methods.}
}
\value{
  A integer vector with three elements.
  These give the line number, column number (position within that line)
  and the count of the number of characters from the beginning of the
  CSS document.
}
\references{
  libcroco at \url{http://www.freespiders.org/projects/libcroco}

  Cascading Style Sheets \url{http://www.w3.org/Style/CSS/}
}
\author{
Duncan Temple Lang
}
\note{This gives the location of the end of several CSS elements, and not the beginning.}
\seealso{
  \code{\link{readCSS}}
  \code{[[} method for a \code{\link{CSSStyleSheet-class}}
}
\examples{
 f = system.file("samples", "test.css", package = "RCSS")
 css = readCSS( f )

 getPos(css[["media"]])
}
\keyword{IO}
\keyword{programming}
