#
# Copyright (C) 2018 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

set(IGDRCL_SRCS_tests_mocks
  ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_32bitAllocator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_async_event_handler.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_async_event_handler.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_aub_csr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_aub_file_stream.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_aub_stream.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_aub_subcapture_manager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_block_kernel_manager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_buffer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_builtins.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_cif.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_cif.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_command_queue.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_compilers.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_compilers.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_context.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_context.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_csr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_csr.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_deferrable_deletion.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_deferrable_deletion.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_deferred_deleter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_deferred_deleter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_device_queue.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_device.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_device.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_event.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_experimental_command_buffer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_gmm.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_gmm_client_context_base.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_gmm_client_context_base.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_gmm_resource_info.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_gmm_resource_info.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_graphics_allocation.h
  ${CMAKE_CURRENT_SOURCE_DIR}${IGDRCL__INSTRUMENTATION_DIR_SUFFIX}/mock_instrumentation.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}${BRANCH_DIR_SUFFIX}/mock_gmm_client_context.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}${BRANCH_DIR_SUFFIX}/mock_gmm_client_context.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_host_ptr_manager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_internal_allocation_storage.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_image.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_kernel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_kernel.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_memory_manager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_memory_manager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_ostime.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_physical_address_allocator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_program.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_program.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_sampler.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_sip.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_sip.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_source_level_debugger.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_submissions_aggregator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_tbx_stream.h
)

if(WIN32)
  file(GLOB IGDRCL_SRC_tests_mock_wddm "${CMAKE_CURRENT_SOURCE_DIR}/mock_wddm2[0-9]\.*")
  list(APPEND IGDRCL_SRCS_tests_mocks
    ${CMAKE_CURRENT_SOURCE_DIR}/mock_d3d_objects.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mock_ostime_win.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mock_gmm_memory_base.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mock_gmm_memory_base.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gmm_memory${BRANCH_DIR_SUFFIX}/mock_gmm_memory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mock_gmm_page_table_mngr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mock_gmm_page_table_mngr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mock_wddm_interface23.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mock_wddm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mock_wddm.cpp
    ${IGDRCL_SRC_tests_mock_wddm}
  )
else()
  list(APPEND IGDRCL_SRCS_tests_mocks
    ${CMAKE_CURRENT_SOURCE_DIR}/linux/mock_drm_memory_manager.h
  )
endif()

add_subdirectories()

add_library(igdrcl_mocks STATIC ${IGDRCL_SRCS_tests_mocks})

if(WIN32)
  target_include_directories(igdrcl_mocks PUBLIC
    ${IGDRCL_SOURCE_DIR}/unit_tests/mocks/gmm_memory${BRANCH_DIR_SUFFIX}
  )
endif()

# add_dependencies(igdrcl_mocks gmock-gtest)

target_include_directories(igdrcl_mocks PRIVATE 
    $<TARGET_PROPERTY:gmock-gtest,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:${NEO_STATIC_LIB_NAME},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(igdrcl_mocks PRIVATE MOCKABLE_VIRTUAL=virtual $<TARGET_PROPERTY:${NEO_STATIC_LIB_NAME},INTERFACE_COMPILE_DEFINITIONS>)

set_target_properties(igdrcl_mocks PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(igdrcl_mocks PROPERTIES FOLDER "test mocks")
create_project_source_tree(igdrcl_mocks)
