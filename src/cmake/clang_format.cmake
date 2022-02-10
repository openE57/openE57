find_program(E57_CLANG_FORMAT_BIN NAMES clang-format)

if(E57_CLANG_FORMAT_BIN)
  get_target_property(openE57_files ${PROJECT_NAME} SOURCES)
  get_target_property(openE57las_files ${PROJECT_NAME}las SOURCES)

  add_custom_target(
    format-openE57
    COMMAND clang-format --style=file -i ${openE57_files}
    COMMAND_EXPAND_LISTS VERBATIM)

  add_custom_target(
    format-openE57las
    COMMAND clang-format --style=file -i ${openE57las_files}
    COMMAND_EXPAND_LISTS VERBATIM)

  add_custom_target(
    format
    COMMENT "Running clang-format..."
    DEPENDS format-openE57 format-openE57las)
endif()
