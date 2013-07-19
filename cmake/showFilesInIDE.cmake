#
# showFilesInIDE(file_paths...)
#
# Parameters:
# file_paths... -   files that shall be accessible from IDE
#
# Description:
# Include directory content for convenience in editing in IDE
#
function(showFilesInIDE)
    string(MD5 TARGET_NAME ${CMAKE_CURRENT_LIST_FILE})

    add_custom_target(${TARGET_NAME} ALL SOURCES ${ARGN})
endfunction()
