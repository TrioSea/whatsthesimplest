add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
        libs/strings.h
        libs/strings.c

        libs/conversions.h
        libs/conversions.c

        libs/run.h
        run.c
        main.c
)