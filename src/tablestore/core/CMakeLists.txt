﻿add_library(tablestore_core STATIC
    client.cpp
    client.hpp
    error.cpp
    error.hpp
    http/asio.cpp
    http/asio.hpp
    http/asio_impl.cpp
    http/asio_impl.hpp
    http/bookmark_input_stream.cpp
    http/bookmark_input_stream.hpp
    http/client.cpp
    http/client.hpp
    http/connection_impl.cpp
    http/connection_impl.hpp
    http/response_reader.cpp
    http/response_reader.hpp
    http/timer_impl.cpp
    http/timer_impl.hpp
    http/types.cpp
    http/types.hpp
    impl/api_traits.cpp
    impl/api_traits.hpp
    impl/async_client.cpp
    impl/async_client.hpp
    impl/async_client_base.cpp
    impl/async_client_base.hpp
    impl/buildinfo.cpp.in
    impl/ots_constants.cpp
    impl/ots_constants.hpp
    impl/ots_exception.cpp
    impl/ots_exception.hpp
    impl/ots_helper.cpp
    impl/ots_helper.hpp
    impl/serde.cpp
    impl/serde.hpp
    impl/sync_client.cpp
    impl/sync_client.hpp
    plainbuffer/plain_buffer_builder.cpp
    plainbuffer/plain_buffer_builder.hpp
    plainbuffer/plain_buffer_coded_stream.cpp
    plainbuffer/plain_buffer_coded_stream.hpp
    plainbuffer/plain_buffer_consts.hpp
    plainbuffer/plain_buffer_crc8.cpp
    plainbuffer/plain_buffer_crc8.hpp
    plainbuffer/plain_buffer_stream.cpp
    plainbuffer/plain_buffer_stream.hpp
    protocol/table_store.proto
    protocol/table_store_filter.proto
    protocol/zero_copy_stream.cpp
    protocol/zero_copy_stream.hpp
    range_iterator.cpp
    range_iterator.hpp
    retry.cpp
    retry.hpp
    types.cpp
    types.hpp
)
protobuf_generate_cpp(proto_srcs proto_hdrs protocol/table_store.proto protocol/table_store_filter.proto)

set(major "4")
set(minor "5")
set(revision "1")
set(language "cpp98")
set(platform "${CMAKE_SYSTEM}")
set(architecture "${CMAKE_SYSTEM_PROCESSOR}")
configure_file(impl/buildinfo.cpp.cmake.in buildinfo.cpp @ONLY)

target_sources(tablestore_core PRIVATE ${proto_srcs} ${proto_hdrs} "${CMAKE_CURRENT_BINARY_DIR}/buildinfo.cpp")
set_source_files_properties(${proto_srcs} ${proto_hdrs} "${CMAKE_CURRENT_BINARY_DIR}/buildinfo.cpp"
    PROPERTIES SKIP_AUTOGEN ON
)
target_compile_definitions(tablestore_core PUBLIC BOOST_ASIO_ENABLE_OLD_SERVICES)
target_include_directories(tablestore_core
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
    PRIVATE "${Protobuf_INCLUDE_DIRS}" "${CMAKE_CURRENT_BINARY_DIR}"
)
target_link_libraries(tablestore_core PUBLIC tablestore_util
    protobuf::libprotobuf decrepit
)
if(WIN32)
    target_link_libraries(tablestore_core PUBLIC rpcrt4.lib)
endif()
