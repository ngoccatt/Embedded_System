idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS ".")

# you can try enable this line to run only tryMemory.c. (remember to delete main.c)
#idf_component_register(SRCS "tryMemory.c" "main.c" INCLUDE_DIRS ".")