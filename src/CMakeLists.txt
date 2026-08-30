add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
        headers/strings.h
        headers/strings.c

        headers/conversions.h
        headers/conversions.c

        headers/run.h
        run.c
        main.c
)