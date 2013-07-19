include(CheckCXXCompilerFlag)
include(CMakeParseArguments)

#
# selectSupportedCXXFlags(var flag1 flag2 ... [FIRST_VALID_OF flags ...])
#
# Parameters:
# var                       -   variable to store all selected flags
# flag1 flag2 ...           -   the compiler flags
# FIRST_VALID_OF flags ...  -   the compiler flags (select first valid flag from this list)
#
# Description:
# Check whether the C++ compiler supports a given flags and then store all valid
# flags to var.
#
function(selectSupportedCXXFlags output)
    cmake_parse_arguments(FLAG "" "" "FIRST_VALID_OF" ${ARGN})

    foreach(flag ${FLAG_UNPARSED_ARGUMENTS})
        check_cxx_compiler_flag(${flag} valid)
        if(valid)
            list(APPEND result ${flag})
        endif()
        unset(valid CACHE)
    endforeach()

    foreach(flag ${FLAG_FIRST_VALID_OF})
        check_cxx_compiler_flag(${flag} valid)
        if(valid)
            list(APPEND result ${flag})
            break()
        endif()
        unset(valid CACHE)
    endforeach()

    set(${output} ${result} PARENT_SCOPE)
endfunction()
