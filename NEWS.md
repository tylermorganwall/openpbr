# openpbr 1.1.1-0

- Adds `openpbr_version()`, a registered native interface that verifies a diffuse
  BSDF evaluation and returns the supported OpenPBR model version. Installation
  now compiles against `glmheaders` rather than leaving `LinkingTo` unused.

- Provides the pinned Adobe OpenPBR BSDF headers and lookup tables under
  Apache-2.0, with GLM supplied through `LinkingTo: glmheaders`.
