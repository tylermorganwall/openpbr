if (requireNamespace("glmheaders", quietly = TRUE)) {
  local({
    # Compile an independent consumer against the installed headers.
    source = normalizePath(file.path("cpp", "headers.cpp"))
    include_dirs = c(
      system.file("include", package = "openpbr", mustWork = TRUE),
      system.file("include", package = "glmheaders", mustWork = TRUE)
    )
    build_dir = tempfile("header-compiler-")
    dir.create(build_dir)
    old_wd = getwd()
    old_env = Sys.getenv(
      c("R_TESTS", "TMPDIR", "PKG_CPPFLAGS"),
      unset = NA_character_
    )
    on.exit({
      setwd(old_wd)
      Sys.unsetenv(names(old_env)[is.na(old_env)])
      do.call(Sys.setenv, as.list(old_env[!is.na(old_env)]))
      unlink(build_dir, recursive = TRUE)
    })
    file.copy(source, file.path(build_dir, "headers.cpp"))
    setwd(build_dir)

    # Compiler subprocesses must not inherit check's relative startup path.
    # Keep compiler caches inside the directory cleaned up after this test.
    Sys.unsetenv("R_TESTS")
    Sys.setenv(
      TMPDIR = build_dir,
      PKG_CPPFLAGS = paste(paste0("-I", shQuote(include_dirs)), collapse = " ")
    )
    library_file = paste0("headers", .Platform$dynlib.ext)
    build_log = tools::Rcmd(
      c("SHLIB", "headers.cpp", "-o", library_file),
      stdout = TRUE,
      stderr = TRUE
    )
    if (!is.null(attr(build_log, "status"))) {
      stop(paste(build_log, collapse = "\n"))
    }

    dll = dyn.load(library_file)
    # Unload before removing the build directory, including on Windows.
    on.exit(dyn.unload(dll[["path"]]), add = TRUE, after = FALSE)
    result = .Call(getDLLRegisteredRoutines(dll)$.Call$openpbr_smoke)
    names(result) = c("lambertian", "valid_samples", "max_relative_error")
    stopifnot(
      abs(result[["lambertian"]] - .5 / pi) < 1e-6,
      result[["valid_samples"]] > 300,
      result[["max_relative_error"]] < 1e-4
    )
  })
}
