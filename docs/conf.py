import sys
import os
import exhale
# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('../include/fields2cover'))


# -- Project information -----------------------------------------------------

project = 'Fields2Cover'
copyright = '2022, Wageningen University'
author = 'Wageningen University'

# The full version, including alpha/beta/rc tags
release = 'latest'

primary_domain = 'cpp'
highlight_language = 'cpp'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.intersphinx',
    'sphinx.ext.autosectionlabel',
    'sphinx.ext.todo',
    'sphinx.ext.coverage',
    'sphinx.ext.mathjax',
    'sphinx.ext.ifconfig',
    'sphinx.ext.viewcode',
    'sphinx_sitemap',
    'sphinx.ext.inheritance_diagram',
    "exhale",
    "breathe",
    "m2r2"
]

source_suffix = [".rst", ".md"]

breathe_projects = {"Fields2Cover": "../build/docs/doc_doxygen/xml/index.xml"}
breathe_default_project = "Fields2Cover"
#breathe_default_members = ('members', 'undoc-members')
exhale_args =  {
    "containmentFolder"    : "./api/",
    "rootFileName"         : "f2c_library.rst",
    "rootFileTitle"        : "API",
    "doxygenStripFromPath" : ".."
}

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
#html_static_path = ['_static']

html_baseurl = 'https://fields2cover.github.io/'
html_logo = "figures/logo_fields2cover.png"
html_favicon = "figures/favicon/favicon.ico"
html_theme_options = {
    'logo_only': True,
    'display_version': False,
}
github_url = 'https://github.com/Fields2Cover-group/Fields2Cover'

import time
def setup(app):
    time.sleep(3)




