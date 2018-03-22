add_library(tablestore_util STATIC
    arithmetic.cpp
    arithmetic.hpp
    assert.cpp
    assert.hpp
    assert.ipp
    foreach.hpp
    iterator.hpp
    iterator.ipp
    logger.cpp
    logger.hpp
    logging.cpp
    logging.hpp
    logging.ipp
    mempiece.cpp
    mempiece.hpp
    mempiece.ipp
    mempool.cpp
    mempool.hpp
    metaprogramming.hpp
    move.hpp
    move.ipp
    optional.hpp
    prettyprint.cpp
    prettyprint.hpp
    prettyprint.ipp
    random.cpp
    random.hpp
    result.hpp
    security.cpp
    security.hpp
    seq_gen.hpp
    threading.cpp
    threading.hpp
    timestamp.cpp
    timestamp.hpp
    try.hpp
)
target_include_directories(tablestore_util
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
    PUBLIC "${PROJECT_SOURCE_DIR}/src"
)
target_link_libraries(tablestore_util PUBLIC
    Boost::system Boost::chrono Boost::thread crypto
)
