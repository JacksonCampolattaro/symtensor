DOXYFILE = 'Doxyfile-mcss'

STYLESHEETS = [
    'https://fonts.googleapis.com/css?family=Inter:400,400i,700,700i|JetBrains+Mono:400,400i,600',
    '../css/m-theme-light.css',
    '../css/m-layout.css',
    '../css/m-grid.css',
    '../css/m-components.css',
    '../css/pygments-console.css',
    '../css/m-documentation.css',
    '@CMAKE_CURRENT_SOURCE_DIR@/../doc/style.css',
]

FAVICON = '@CMAKE_CURRENT_SOURCE_DIR@/images/symtensor-logo.svg'

LINKS_NAVBAR1 = [
    ("Pages", 'pages', []),
    #("Namespaces", 'namespaces', [])
    ("Classes", 'annotated', []),
]
LINKS_NAVBAR2 = [
    ("Files", 'files', []),
    ("""
        <a href="https://github.com/JacksonCampolattaro/symtensor">
            <img 
                style="position:relative;top:0.25em;"
                src="https://img.shields.io/badge/GitHub-black?logo=github"
            >
        </a>
    """, [])
]

FILE_INDEX_EXPAND_LEVELS = 10
