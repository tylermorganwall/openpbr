#' OpenPBR Material Model Version
#'
#' Exercise the installed native OpenPBR interface with a diffuse BSDF evaluation
#' and return the material model version supported by the bundled headers.
#'
#' @md
#' @return A character scalar containing the OpenPBR material model version.
#'   This is distinct from the R package version reported by
#'   `utils::packageVersion("openpbr")`.
#' @export
#' @examples
#' openpbr_version()
openpbr_version <- function() {
  .Call(C_openpbr_version)
}
