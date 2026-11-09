#define R_NO_REMAP
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include <R_ext/Visibility.h>
#include <glm/glm.hpp>
#include <cassert>
#include <cmath>
#include <cstdint>

namespace reference {
#include <openpbr/openpbr.h>
}

extern "C" SEXP openpbr_version() {
  // Use the real BSDF API so package installation compiles its GLM dependency
  // and lookup tables. An index-matched diffuse surface has BRDF 0.5 / pi.
  auto inputs = reference::openpbr_make_default_resolved_inputs();
  inputs.base_color = glm::vec3(.5f);
  inputs.specular_ior = 1.f;
  inputs.geometry_basis = reference::openpbr_make_basis(
    glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), 1.f);
  inputs.geometry_coat_basis = inputs.geometry_basis;
  const auto prepared = reference::openpbr_prepare(inputs, glm::vec3(1),
    reference::OpenPBR_BaseRgbWavelengths_nm, 1.f, glm::vec3(0, 0, 1));
  const auto value = reference::openpbr_get_sum_of_diffuse_specular(
    reference::openpbr_eval(prepared, glm::vec3(0, 0, 1)));
  for (int channel = 0; channel < 3; ++channel) {
    if (!std::isfinite(value[channel]) ||
        std::abs(value[channel] - .5 / std::acos(-1.)) > 1e-6) {
      Rf_error("OpenPBR native header verification failed.");
    }
  }
  // The pinned upstream has no model-version macro; keep this synchronized with
  // inst/COPYRIGHTS when updating the bundled implementation.
  return Rf_mkString("1.1.1");
}

static const R_CallMethodDef call_methods[] = {
  {"openpbr_version", reinterpret_cast<DL_FUNC>(&openpbr_version), 0},
  {nullptr, nullptr, 0}
};

extern "C" void attribute_visible R_init_openpbr(DllInfo* dll) {
  R_registerRoutines(dll, nullptr, call_methods, nullptr, nullptr);
  R_useDynamicSymbols(dll, FALSE);
  R_forceSymbols(dll, TRUE);
}
