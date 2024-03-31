add_subdirectory(StringUtils)

if (ENABLE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if (ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()
