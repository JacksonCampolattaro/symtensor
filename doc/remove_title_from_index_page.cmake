# This is a terrible hack

# Remove header from homepage
file(READ ${CMAKE_CURRENT_BINARY_DIR}/html/index.html FILE_CONTENTS)
string(
        REGEX REPLACE
        "<h1>.*</h1>"
        ""
        FILE_CONTENTS
        "${FILE_CONTENTS}"
)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/html/index.html "${FILE_CONTENTS}")
