# =============================================================================
# XML Backend Selection
# =============================================================================
# Supported backends: xerces, libxml2, pugixml
#
# Auto-detection order (when E57_XML_BACKEND="auto"):
#   1. Xerces-C   (find_package) — full-featured, schema validation
#   2. libxml2    (find_package) — system-provided only, never fetched
#   3. pugixml    (find_package) — lightweight alternative
#
# Fallback when no library is found on the system:
#   - If XERCES_C_DEFAULT_FETCH_TAG is set → fetch Xerces-C from source
#   - Otherwise → fetch pugixml from source

set(E57_XML_BACKEND "auto" CACHE STRING "XML parser backend (auto, xerces, libxml2, pugixml)")
set_property(CACHE E57_XML_BACKEND PROPERTY STRINGS "auto" "xerces" "libxml2" "pugixml")

list(APPEND E57_XML_BACKEND_OPTIONS "auto" "xerces" "libxml2" "pugixml")

if(NOT E57_XML_BACKEND IN_LIST E57_XML_BACKEND_OPTIONS)
    message(FATAL_ERROR "Invalid E57_XML_BACKEND: ${E57_XML_BACKEND}. "
                        "Must be one of: ${E57_XML_BACKEND_OPTIONS}")
endif()

# --- Auto-detection ---
if(E57_XML_BACKEND STREQUAL "auto")
    set(XercesC_USE_STATIC_LIBS ON)
    find_package(XercesC 3.2 QUIET)
    if(XercesC_FOUND)
        set(E57_XML_BACKEND "xerces")
    else()
        find_package(LibXml2 QUIET)
        if(LibXml2_FOUND)
            set(E57_XML_BACKEND "libxml2")
        else()
            set(E57_XML_BACKEND "pugixml")
        endif()
    endif()
    message(STATUS "Auto-detected XML Backend: ${E57_XML_BACKEND}")
endif()

message(STATUS "XML Backend: ${E57_XML_BACKEND}")

# --- Backend-specific setup ---

if(E57_XML_BACKEND STREQUAL "xerces")
    set(XercesC_USE_STATIC_LIBS ON)
    find_package(XercesC 3.2 QUIET)
    if (XercesC_FOUND)
      set(XML_LIBRARIES ${XercesC_LIBRARIES})
      set(XML_INCLUDE_DIRS ${XercesC_INCLUDE_DIRS})
      if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Apple")
        all_dynamic_libs(XercesC_LIBRARIES dyn_XercesC)
        if (${dyn_XercesC})
          message("Linking dynlib xercesc: ${XercesC_LIBRARIES}, no need for linking ICU")
        else()
          find_package(ICU REQUIRED)
          set(XML_LIBRARIES ${XML_LIBRARIES} ${ICU_LIBRARIES})
          set(XML_INCLUDE_DIRS ${XML_INCLUDE_DIRS} ${ICU_INCLUDE_DIRS})
        endif ()
      endif()
    else()
      if (NOT TARGET xerces-c)
        set(XERCES_C_REPO https://github.com/apache/xerces-c.git)
        FetchContent_Declare(
          xerces-c
          EXCLUDE_FROM_ALL
          GIT_REPOSITORY ${XERCES_C_REPO}
          GIT_TAG ${XERCES_C_DEFAULT_FETCH_TAG}
          GIT_SHALLOW TRUE
        )
        FetchContent_MakeAvailable(xerces-c)
      endif()
      set(XML_LIBRARIES "xerces-c")
      get_target_property(XercesC_INCLUDE_DIRS xerces-c INCLUDE_DIRECTORIES)
      set(XML_INCLUDE_DIRS ${XercesC_INCLUDE_DIRS})
    endif()

    if(NOT BUILD_SHARED_LIBS)
      list(APPEND compiler_definitions XERCES_STATIC_LIBRARY)
    endif()

    list(APPEND compiler_definitions E57_XML_BACKEND_XERCES)

elseif(E57_XML_BACKEND STREQUAL "libxml2")
    find_package(LibXml2 REQUIRED)

    set(XML_LIBRARIES LibXml2::LibXml2)
    set(XML_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIRS})

    list(APPEND compiler_definitions E57_XML_BACKEND_LIBXML2)

elseif(E57_XML_BACKEND STREQUAL "pugixml")
    find_package(pugixml QUIET)
    if(NOT pugixml_FOUND)
      if(NOT TARGET pugixml::pugixml)
        message(STATUS "Fetching pugixml via FetchContent")
        FetchContent_Declare(
          pugixml
          EXCLUDE_FROM_ALL
          GIT_REPOSITORY https://github.com/zeux/pugixml.git
          GIT_TAG v1.15
          GIT_SHALLOW TRUE
        )
        FetchContent_MakeAvailable(pugixml)
      endif()
    endif()

    set(XML_LIBRARIES "pugixml::pugixml")
    set(XML_INCLUDE_DIRS "")
    list(APPEND compiler_definitions E57_XML_BACKEND_PUGIXML)
endif()
