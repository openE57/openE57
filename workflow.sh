#!/usr/bin/env bash

#conan source . --source-folder=tmp/source
#conan install . --install-folder=tmp/build -o openE57:with_examples=True -o openE57:with_tools=True 
#conan build . --source-folder=tmp/source --build-folder=tmp/build
#conan package . --source-folder=tmp/source --build-folder=tmp/build --package-folder=tmp/package
#conan export-pkg . openE57/stable --source-folder=tmp/source --build-folder=tmp/build --force
#
#conan test test_package openE57/1.6.0@openE57/stable

conan create . openE57/stable -o openE57:with_examples=True -o openE57:with_tools=True 