# coverage.cmake - Code coverage configuration for openE57
#
# This module provides coverage support using gcov/lcov/genhtml
# Usage:
#   cmake -DBUILD_TESTS=ON -DBUILD_COVERAGE=ON ..
#   cmake --build .
#   ctest
#   cmake --build . --target coverage

# Only enable coverage on supported platforms
if(BUILD_COVERAGE)
  # Check for coverage tools
  find_program(LCOV_EXE lcov)
  find_program(GENHTML_EXE genhtml)
  
  if(NOT LCOV_EXE OR NOT GENHTML_EXE)
    message(WARNING "Coverage tools not found. Install lcov and genhtml to generate coverage reports.")
    message(STATUS "  Ubuntu/Debian: sudo apt-get install lcov")
    message(STATUS "  macOS: brew install lcov")
    message(STATUS "  Fedora: sudo dnf install lcov")
    set(BUILD_COVERAGE OFF CACHE BOOL "Disable coverage (tools not found)" FORCE)
  else()
    message(STATUS "Coverage enabled: ${LCOV_EXE}, ${GENHTML_EXE}")
    
    # Verify tests are enabled
    if(NOT BUILD_TESTS)
      message(WARNING "Coverage requires BUILD_TESTS=ON. Enabling automatically.")
      set(BUILD_TESTS ON CACHE BOOL "Enable tests for coverage" FORCE)
    endif()
    
    # Coverage output directory
    set(COVERAGE_DIR "${CMAKE_BINARY_DIR}/coverage")
    
    # Add custom target to generate coverage report
    add_custom_target(coverage
      # Clean previous coverage data
      COMMAND ${CMAKE_COMMAND} -E remove_directory ${COVERAGE_DIR}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_DIR}
      
      # Capture initial baseline (zero coverage) - capture all, filter later
      COMMAND ${LCOV_EXE} --capture --initial 
              --directory ${CMAKE_BINARY_DIR} 
              --output-file ${COVERAGE_DIR}/baseline.info
              --ignore-errors inconsistent,range
      
      # Run all tests
      COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
      
      # Capture coverage data after tests
      COMMAND ${LCOV_EXE} --capture 
              --directory ${CMAKE_BINARY_DIR} 
              --output-file ${COVERAGE_DIR}/coverage.info
              --ignore-errors inconsistent,corrupt,range
      
      # Combine baseline and coverage data
      COMMAND ${LCOV_EXE} --add-tracefile ${COVERAGE_DIR}/baseline.info 
              --add-tracefile ${COVERAGE_DIR}/coverage.info 
              --output-file ${COVERAGE_DIR}/total.info
              --ignore-errors inconsistent,corrupt
      
      # Remove coverage for system headers, test files, and dependencies
      COMMAND ${LCOV_EXE} --remove ${COVERAGE_DIR}/total.info 
              '/usr/*' 
              '*/test/*' 
              '*/_deps/*'
              --ignore-errors unused,inconsistent,corrupt
              --output-file ${COVERAGE_DIR}/filtered.info
      
      # Generate HTML report
      COMMAND ${GENHTML_EXE} ${COVERAGE_DIR}/filtered.info 
              --output-directory ${COVERAGE_DIR}/html
              --title "openE57 Code Coverage"
              --legend
              --show-details
      
      # Print summary
      COMMAND ${LCOV_EXE} --summary ${COVERAGE_DIR}/filtered.info
              --ignore-errors inconsistent,corrupt
      
      # Display location of report
      COMMAND ${CMAKE_COMMAND} -E echo "Coverage report generated: ${COVERAGE_DIR}/html/index.html"
      
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMENT "Generating code coverage report..."
      VERBATIM
    )
    
    # Add a target to show coverage summary only (faster)
    add_custom_target(coverage-summary
      COMMAND ${LCOV_EXE} --capture 
              --directory ${CMAKE_BINARY_DIR} 
              --output-file ${COVERAGE_DIR}/coverage.info
              --ignore-errors inconsistent,corrupt
      COMMAND ${LCOV_EXE} --remove ${COVERAGE_DIR}/coverage.info 
              '/usr/*' '*/test/*' '*/_deps/*'
              --ignore-errors unused,inconsistent,corrupt
              --output-file ${COVERAGE_DIR}/filtered.info
      COMMAND ${LCOV_EXE} --summary ${COVERAGE_DIR}/filtered.info
              --ignore-errors inconsistent,corrupt
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMENT "Showing coverage summary..."
      VERBATIM
    )
  endif()
endif()
