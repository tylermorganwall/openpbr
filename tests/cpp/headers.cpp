#define R_NO_REMAP
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include <R_ext/Visibility.h>
#include <glm/glm.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
namespace reference {
#include <openpbr/openpbr.h>
}

extern "C" SEXP openpbr_smoke() {
  namespace ref = reference;
  auto inputs = ref::openpbr_make_default_resolved_inputs();
  inputs.base_color = glm::vec3(.5f);
  inputs.specular_ior = 1.f;
  inputs.geometry_basis = ref::openpbr_make_basis(glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), 1.f);
  inputs.geometry_coat_basis = inputs.geometry_basis;
  const auto diffuse = ref::openpbr_prepare(inputs, glm::vec3(1),
    ref::OpenPBR_BaseRgbWavelengths_nm, 1.f, glm::vec3(0, 0, 1));
  const auto lambertian = ref::openpbr_get_sum_of_diffuse_specular(
    ref::openpbr_eval(diffuse, glm::vec3(0, 0, 1)));
  double max_error = 0;
  int valid = 0;
  // Exercise sampling for dielectric, metal, coat, fuzz, transmission, and film.
  for (int variant = 0; variant < 6; ++variant) {
    auto p = inputs;
    p.specular_ior = 1.5f;
    p.specular_roughness = .4f;
    if (variant == 1) p.base_metalness = 1.f;
    if (variant == 2) { p.coat_weight = .7f; p.coat_roughness = .3f; }
    if (variant == 3) p.fuzz_weight = .6f;
    if (variant == 4) p.transmission_weight = .8f;
    if (variant == 5) p.thin_film_weight = 1.f;
    const auto prepared = ref::openpbr_prepare(p, glm::vec3(1),
      ref::OpenPBR_BaseRgbWavelengths_nm, 1.f, glm::normalize(glm::vec3(.2f, .1f, 1.f)));
    for (int i = 0; i < 128; ++i) {
      // Fixed inputs make the sample/evaluation comparison reproducible.
      const glm::vec3 sample_uniforms(std::fmod(.17f + i * .61803399f, 1.f),
                                     std::fmod(.31f + i * .41421356f, 1.f),
                                     std::fmod(.53f + i * .73205081f, 1.f));
      glm::vec3 direction;
      ref::OpenPBR_DiffuseSpecular weight;
      float pdf;
      std::uint32_t type;
      ref::openpbr_sample(prepared, sample_uniforms, direction, weight, pdf, type);
      if (!std::isfinite(pdf) || pdf < 0) Rf_error("Invalid sample PDF");
      if (pdf == 0) continue;
      ++valid;
      const auto w = ref::openpbr_get_sum_of_diffuse_specular(weight);
      const auto evaluated = ref::openpbr_get_sum_of_diffuse_specular(ref::openpbr_eval(prepared, direction));
      const float density = ref::openpbr_pdf(prepared, direction);
      if (!std::isfinite(density) || density < 0) Rf_error("Invalid evaluated PDF");
      max_error = std::max(max_error, double(std::abs(density / pdf - 1)));
      for (int c = 0; c < 3; ++c) {
        if (!std::isfinite(w[c]) || w[c] < 0) Rf_error("Invalid sample weight");
        if (!std::isfinite(evaluated[c]) || evaluated[c] < 0) Rf_error("Invalid evaluated weight");
        max_error = std::max(max_error, double(std::abs(evaluated[c] / pdf - w[c]) / std::max(1.f, w[c])));
      }
    }
  }
  SEXP result = PROTECT(Rf_allocVector(REALSXP, 3));
  REAL(result)[0] = lambertian.x;
  REAL(result)[1] = valid;
  REAL(result)[2] = max_error;
  UNPROTECT(1);
  return result;
}

static const R_CallMethodDef call_methods[] = {
  {"openpbr_smoke", reinterpret_cast<DL_FUNC>(&openpbr_smoke), 0},
  {nullptr, nullptr, 0}
};

extern "C" void attribute_visible R_init_headers(DllInfo* dll) {
  R_registerRoutines(dll, nullptr, call_methods, nullptr, nullptr);
  R_useDynamicSymbols(dll, FALSE);
  R_forceSymbols(dll, TRUE);
}
