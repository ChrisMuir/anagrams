# Unload the DLL related to c++ files if the anagrams package is unloaded.
.onUnload <- function (libpath) {
  library.dynam.unload("anagrams", libpath)
}
