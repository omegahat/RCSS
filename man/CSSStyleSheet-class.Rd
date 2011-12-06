\name{CSSStyleSheet-class}
\docType{class}
\alias{CSSStyleSheet-class}
\alias{[,CSSStyleSheet,ANY-method}
\alias{[[,CSSStyleSheet,numeric-method}
\alias{[,CSSStyleSheet,character-method}
\alias{[[,CSSStyleSheet,character-method}
\alias{length,CSSStyleSheet-method}
\alias{names,CSSStyleSheet-method}
\alias{lapply,CSSStyleSheet-method}
\alias{sapply,CSSStyleSheet-method}

\title{Representation of a parsed Cascading Style Shetet}
\description{
  This is the basic class for representing a parsed Cascading Style Sheet
  in \code{RCSS}. It is a reference to a C-level object which
  holds the parsed information.
  We can treate the style sheet like a list, asking for its length,
  its names, individual elements or subsets of elements,
  and using \code{lapply} and \code{sapply} to invoke a function on each
  of the elements.

  The CSS is actually a tree and we can access the sub-elements by
  converting the top-level elements to R objects using
  \code{\link{asCSSObject}}.
  Some classes of elements within the style sheet have methods for
  accessing the contents directly from the C-level object, e.g.
  \code{url}, \code{sheet} and \code{media_list} in an \code{IMPORT_RULE} object.
}
\section{Objects from the Class}{
  Objects of these classes are produced implicitly
  via \code{\link{readCSS}}.
}
\section{Slots}{
  \describe{
    \item{\code{docName}:}{character.  The name of the document (file or
  URL) from which this CSS content was read. This is used to identify
  the origin of the CSS content and when resolving relative imports.}
    \item{\code{ref}:}{Object of class \code{"externalptr"} which is the
            reference to the C-level object. }
  }
}
\section{Extends}{
Class \code{"ExternalRef"}, directly.
}
\section{Methods}{
  \describe{
    \item{[}{\code{signature(x = "CSSStyleSheet")}: retrieve a subset of
  the top-level elements in the style sheet. }
    \item{[[}{\code{signature(x = "CSSStyleSheet")}: retrieve an
  individual top-level element of the style sheet.}
    \item{length}{\code{signature(x = "CSSStyleSheet")}: return the
  number of top-level elements in the style sheet. Note that this
  ignores page and import nodes.
  }
    \item{names}{\code{signature(x = "CSSStyleSheet")}: get (heuristic)
  names of the top-level names in the style sheet }
	 }
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
length(css)
css[]
sapply(css, class)
css[[3]]
css[[3]]
}
\keyword{classes}
\keyword{IO}
\keyword{programming}