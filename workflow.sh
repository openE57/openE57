#!/usr/bin/env bash

conan source . --source-folder=tmp/source
conan install . --install-folder=tmp/build
conan build . --source-folder=tmp/source --build-folder=tmp/build
conan package . --source-folder=tmp/source --build-folder=tmp/build --package-folder=tmp/package
conan export-pkg . openE57/stable --source-folder=tmp/source --build-folder=tmp/build

#conan test test_package hello/1.1@user/channel