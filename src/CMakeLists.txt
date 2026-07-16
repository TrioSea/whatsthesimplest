add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
        libs/run.h
        run.c
        main.c
)