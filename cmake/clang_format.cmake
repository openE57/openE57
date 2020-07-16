# Copyright 2020 Michele Adduci <adduci@tutanota.com>

find_program(E57_CLANG_FORMAT_BIN NAMES clang-format)

if(E57_CLANG_FORMAT_BIN)
  get_target_property(openE57_files ${PROJECT_NAME} SOURCES)
  get_target_property(LASReader_files LASReader SOURCES)
  get_target_property(time_conversion_files time_conversion SOURCES)

  add_custom_target(
    format-openE57
    COMMAND clang-format --style=file -i ${openE57_files}
    COMMAND_EXPAND_LISTS VERBATIM)

  add_custom_target(
    format-lasreader
    COMMAND clang-format --style=file -i ${LASReader_files}
    COMMAND_EXPAND_LISTS VERBATIM)

  add_custom_target(
    format-timeconv
    COMMAND clang-format --style=file -i ${time_conversion_files}
    COMMAND_EXPAND_LISTS VERBATIM)

  add_custom_target(
    format
    COMMENT "Running clang-format..."
    DEPENDS format-openE57 format-lasreader format-timeconv)
endif()
