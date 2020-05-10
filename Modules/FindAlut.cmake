FIND_PATH(ALUT_INCLUDE_DIR NAMES AL/alut.h)
MARK_AS_ADVANCED(ALUT_INCLUDE_DIR)
 
FIND_LIBRARY(ALUT_LIBRARY NAMES
  alut
  libalut)
MARK_AS_ADVANCED(ALUT_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Alut DEFAULT_MSG ALUT_LIBRARY ALUT_INCLUDE_DIR)
