# openpbr

Adobe's OpenPBR BSDF headers packaged for R under Apache-2.0. The pinned
implementation targets the OpenPBR 1.1.1 model. The upstream shader remains
header-only. A small registered native interface exercises its BSDF and reports
the supported model version:

```r
openpbr::openpbr_version()
# [1] "1.1.1"
```

This is the material model version; `packageVersion("openpbr")` reports the R
package version. Package versions use `x.y.z-n`, with `x.y.z` taken from upstream:
the current version is `1.1.1-2`, where `2` is the packaging revision. Increment
that suffix for packaging changes that retain the same upstream version.
The function checks an index-matched diffuse evaluation against
the expected Lambertian response. Renderers supply geometry, lighting, and
transport; this package does not provide an R shader constructor.

Install glmheaders first, then openpbr. A consuming package should declare both:

```
LinkingTo: openpbr (>= 1.1.1-1), glmheaders (>= 1.0.1-1)
```

The C++ entry point requires GLM to be included first:

```cpp
#include <glm/glm.hpp>
#include <openpbr/openpbr.h>
```

R supplies the include paths when installing the consumer. No custom linker
flags or linkage to this package's verification DLL are needed. To isolate the upstream API, include
GLM first, then include `openpbr/openpbr.h` inside a namespace. Configuration
macros must be set before including OpenPBR. Upstream settings remain unchanged.

Keep R's standard `-DNDEBUG` compilation flag to disable upstream assertions
that could otherwise terminate the R process. Custom build systems must also
define `NDEBUG`.

For standalone tools, locate headers with
`system.file("include", package = "openpbr")` and the corresponding glmheaders
directory. Code compiled during tests or examples should also declare its header
dependencies in Suggests, as LinkingTo is an installation-time dependency.

The pinned upstream revision and copyright attributions are recorded in
`COPYRIGHTS`. API documentation is available in the
[upstream repository](https://github.com/adobe/openpbr-bsdf).
Compile-and-run tests use R's native interface to exercise the installed
headers, BSDF evaluation, sampling, and embedded lookup tables.
