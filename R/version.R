#' OpenPBR Material Model Version
#'
#' Return the material model version supported by the bundled OpenPBR headers
#' and verify the native interface with a diffuse scattering evaluation.
#'
#' @md
#' @return A character scalar containing the OpenPBR material model version.
#'   This is distinct from the R package version reported by
#'   `utils::packageVersion("openpbr")`.
#' @export
#' @examples
#' # Verify the bundled model version and its diffuse scattering evaluation.
#' stopifnot(identical(openpbr_version(), "1.1.1"))
openpbr_version = function() {
  .Call(C_openpbr_version)
}
