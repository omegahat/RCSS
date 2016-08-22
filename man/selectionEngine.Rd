\name{selectionEngine}
\alias{selectionEngine}
\title{Create an instance of the CSS Selection Engine}
\description{
  A selection engine is used in \code{\link{getCSSRules}}
  to match a node to the CSS rules.
  That function creates its own selection engine if none
  is provided. However, if one is calling
  \code{getCSSRules} many times for the same CSS document,
  it is more efficient to creat the engine just once and
  pass it in each call to \code{getCSSRules}.
  This \code{selectionEngine} function creates a selection engine
  for this purpose.
}
\usage{
selectionEngine()
}
\value{
 A reference to the C-level selection engine instance.
}
\author{
Duncan Temple Lang
}

\seealso{
\code{\link{getCSSRules}}
}
\examples{
\dontrun{
eng = selectionEngine()
lapply(nodes, getCSSRules, eng = eng)
}
}
\keyword{IO}
\keyword{programming}

