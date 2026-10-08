\name{openpbr-package}
\alias{openpbr}
\alias{openpbr-package}
\docType{package}
\title{OpenPBR Surface Shading Headers}
\description{
Provides Adobe's header-only implementation of the OpenPBR 1.1.1 material model
for use in C++ code. A small native interface verifies a diffuse BSDF evaluation
and reports the model version through \code{\link{openpbr_version}}.
}
\details{
Add both \code{openpbr} and \code{glmheaders} to a consuming package's
\code{LinkingTo} field. Include \code{<glm/glm.hpp>} before
\code{<openpbr/openpbr.h>} in C++ source. The upstream API evaluates and samples
a layered bidirectional scattering distribution. Geometry, light sampling,
and volume integration are supplied by the consuming renderer.

Headers and embedded lookup tables are installed under
\code{system.file("include", package = "openpbr")}. See the installed
\file{UPSTREAM.md} and \file{UPSTREAM-README.md} for provenance and API details.
}
\references{
\url{https://github.com/adobe/openpbr-bsdf}
\url{https://academysoftwarefoundation.github.io/OpenPBR/}
}
\author{Adobe (BSDF implementation); Tyler Morgan-Wall (R packaging).}
\keyword{package}
