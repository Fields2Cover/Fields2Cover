
# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
  # set input and output files
  set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doc_doxygen/xml)
  file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  # request to configure the file
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  message("Doxygen build started")

  # note the option ALL which allows to build the docs together with the application
  add_custom_target(doc_doxygen ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )


  find_package(Sphinx)
  if (SPHINX_FOUND)
    set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
    set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/docs/sphinx)
    set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)


    add_custom_command(
      OUTPUT
        ${SPHINX_INDEX_FILE}
      COMMAND
        ${SPHINX_EXECUTABLE} -b html
          -Dbreathe_projects.Fields2Cover=${DOXYGEN_OUTPUT_DIR}
          ${SPHINX_SOURCE} ${SPHINX_BUILD}
      WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
      DEPENDS
        # Other docs files you want to track should go here (or in some variable)
        ${SPHINX_SOURCE}/index.rst
        ${DOXYGEN_INDEX_FILE}
      MAIN_DEPENDENCY
        ${SPHINX_SOURCE}/conf.py
      COMMENT
        "Generating documentation with Sphinx"
    )

    # Nice named target so we can run the job easily
    add_custom_target(Sphinx ALL DEPENDS ${SPHINX_INDEX_FILE})

    include(GNUInstallDirs)
    install(
      DIRECTORY ${SPHINX_BUILD}
      DESTINATION ${CMAKE_INSTALL_DOCDIR}
    )
  else (SPHINX_FOUND)
    message("Sphinx not installed")
  endif (SPHINX_FOUND)
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)







