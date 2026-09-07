add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
        headers/mymath.h

        headers/strings.h
        headers/strings.c

        headers/logic.h
        headers/logic.c

        headers/conversions.h
        headers/conversions.c

        headers/run.h
        run.c
        main.c
)

# vibe coded
target_link_libraries(witspg PRIVATE m)