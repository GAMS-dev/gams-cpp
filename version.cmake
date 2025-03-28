function(read_from_file_and_set FILEPATH)
    file(STRINGS ${FILEPATH} FileContents)
    foreach(NameAndValue ${FileContents})

        # Strip leading spaces
        string(REGEX REPLACE "^[ ]+" "" NameAndValue ${NameAndValue})

        # ignore comments
        if(${NameAndValue} MATCHES "^#")
            continue()
        endif()

        # Find variable name
        string(REGEX MATCH "^[^=]+" Name ${NameAndValue})
        # Find the value
        string(REPLACE "${Name}=" "" Value ${NameAndValue})
        # Set the variable
        set(${Name} "${Value}" PARENT_SCOPE)
    endforeach()
endfunction()

read_from_file_and_set(${CMAKE_SOURCE_DIR}/version)

string(CONCAT VERSION_STR
  "${GAMSCPP_MAJOR_VERSION}."
  "${GAMSCPP_MINOR_VERSION}."
  "${GAMSCPP_PATCH_LEVEL}")
set(VERSION ${VERSION_STR})
set(CMAKE_PROJECT_VERSION ${VERSION})
set(CMAKE_PROJECT_VERSION_MAJOR ${GAMSCPP_MAJOR_VERSION})
set(CMAKE_PROJECT_VERSION_MINOR ${GAMSCPP_MINOR_VERSION})
set(CMAKE_PROJECT_VERSION_PATCH ${GAMSCPP_PATCH_LEVEL})
