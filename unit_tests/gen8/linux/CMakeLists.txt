#
# Copyright (C) 2018 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

set(IGDRCL_SRCS_tests_gen8_linux
  ${CMAKE_CURRENT_SOURCE_DIR}/drm_mapper_tests_gen8.cpp
)
if(UNIX)
  target_sources(igdrcl_tests PRIVATE ${IGDRCL_SRCS_tests_gen8_linux})
endif()
