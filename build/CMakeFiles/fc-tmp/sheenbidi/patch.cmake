cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

message(VERBOSE "Executing patch step for sheenbidi")

block(SCOPE_FOR VARIABLES)

execute_process(
  WORKING_DIRECTORY "/home/alex/Documents/coding/aiTest/build/_deps/sheenbidi-src"
  COMMAND_ERROR_IS_FATAL LAST
  COMMAND  [====[/usr/bin/cmake]====] [====[-DSHEENBIDI_DIR=/home/alex/Documents/coding/aiTest/build/_deps/sheenbidi-src]====] [====[-P]====] [====[/home/alex/Documents/coding/aiTest/deps/SFML/tools/sheenbidi/PatchSheenBidi.cmake]====]
)

endblock()
