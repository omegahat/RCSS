\name{getDeclaration}
\alias{getDeclaration}
\alias{getDeclaration,CSSRuleset-method}
\alias{getDeclaration,RULESET-method}
\alias{getDeclaration,CSSStyleSheet-method}
\alias{getDeclaration,IMPORT_RULE-method}

\title{Get the value of a CSS declaration}
\description{
  This function gets the value associated with an individual
  declaration within a CSS rule set.
}
\usage{
getDeclaration(x, what, default = NA, simplify = TRUE)
}
\arguments{
  \item{x}{the \code{CSSRuleSet} or native \code{RULESET} object obtained from the parsed CSS document}
  \item{what}{character giving the name of the property of interest,
    e.g. "color", "background-color", "padding", "font-family"}
  \item{default}{the value to return if there is no declaration for the specified property }
  \item{simplify}{a logical value indicating whether to return the value
    as a scalar if there is just one single value and not a list of values.}
}
\value{
 Either a scalar value or a list of values, or the value of
 \code{default} if the CSS rule set does not have a declaration
 for the specified property.
}
\references{

}
\author{
Duncan Temple Lang
}

\seealso{
  \code{\link{readCSS}}
  \code{\link{asCSSObject}}
}
\examples{
  css = readCSS( system.file("samples", "OmegaTech.css", package = "RCSS"))
  sapply(css, getDeclaration, "color", NA)
}
\keyword{programming}
\concept{documentation}

