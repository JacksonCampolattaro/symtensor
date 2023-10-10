DOXYFILE = 'Doxyfile-mcss'

STYLESHEETS = [
    'https://fonts.googleapis.com/css?family=Inter:400,400i,700,700i|JetBrains+Mono:400,400i,600',
    '../css/m-theme-light.css',
    '../css/m-layout.css',
    '@CMAKE_CURRENT_SOURCE_DIR@/../doc/style.css',
    '../css/m-grid.css',
    '../css/m-components.css',
    '../css/pygments-console.css',
    '../css/m-documentation.css',
]

FAVICON = '@CMAKE_CURRENT_SOURCE_DIR@/../doc/symtensor-logo.svg'

LINKS_NAVBAR1 = [
    ("Pages", 'pages', []),
    ("Namespaces", 'namespaces', [])
]
LINKS_NAVBAR2 = [
    ("Classes", 'annotated', []),
    ("Files", 'files', []),
    ("<a href=\"https://github.com/JacksonCampolattaro/symtensor\"><img style=\"max-height:1.2em;position:relative;top:0.15em;\" src=\"@CMAKE_CURRENT_SOURCE_DIR@/../doc/github-mark.svg\"/></a>", [])
]
