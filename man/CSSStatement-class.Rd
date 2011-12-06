\name{CSSStatement-class}
\docType{class}
\alias{CSSStatement-class}
\alias{IMPORT_RULE-class}
\alias{AT_RULE-class}
\alias{RULESET-class}
\alias{MEDIA_RULE-class}
\alias{PAGE_RULE-class}
\alias{CHARSET_RULE-class}
\alias{FONT_FACE_RULE-class}

\alias{$,IMPORT_RULE-method}


\title{Class "CSSStatement"}
\description{This is the basic class for representing an internal
  C-level CSS element.
  We can query certain elements within such objects directly
  or we can convert such an object to an S4 object that is equivalent.
  We use \code{\link{asCSSObject}} to perform this C-to-R conversion.
}
\section{Objects from the Class}{
  Objects of this and related sub-classes are created implicitly
  via accessing elements of a \code{\link{CSSStyleSheet-class}}
  object, e.g. using \code{[[} and \code{[} methods.
}
\section{Slots}{
	 \describe{
    \item{\code{ref}:}{Object of class \code{"externalptr"} which holds
      the reference to the C object. }
  }
}
\section{Extends}{
Class \code{"ExternalRef"}, directly.
}

\references{
  libcroco at \url{http://www.freespiders.org/projects/libcroco}
  Cascading Style Sheets \url{http://www.w3.org/Style/CSS/}
}
\author{
Duncan Temple Lang
}



\seealso{
 \code{\link{CSSStyleSheet-class}}
}
\examples{
showClass("CSSStatement")
}
\keyword{classes}
