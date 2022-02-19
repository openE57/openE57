if (BUILD_DOCS)
    # check if Doxygen is installed
    find_package(Doxygen REQUIRED)
    if (DOXYGEN_FOUND)

        set(DOXYVAR_INPUT "${PROJECT_SOURCE_DIR}/src/lib/include ${PROJECT_SOURCE_DIR}/src/lib/src")

        set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/doc/api/Doxyfile.in)
        set(DOXYGEN_OUT ${PROJECT_BINARY_DIR}/doc/api/Doxyfile)
        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        # note the option ALL which allows to build the docs together with the application
        add_custom_target( doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc/api/
            COMMENT "Generating API documentation"
            VERBATIM )

    else (DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif()