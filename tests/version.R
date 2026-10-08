# This check has no optional test dependencies and exercises the installed DLL.
stopifnot(identical(openpbr::openpbr_version(), "1.1.1"))
registered <- getDLLRegisteredRoutines("openpbr")$.Call
stopifnot("openpbr_version" %in% names(registered))
