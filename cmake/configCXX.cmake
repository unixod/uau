include(selectSupportedCXXFlags)

selectSupportedCXXFlags(LIBUAU_WALL_FLAG
    -Wall)

selectSupportedCXXFlags(LIBUAU_CXX11_FLAG
    FIRST_VALID_OF
        -std=c++11
        -std=c++0x)

