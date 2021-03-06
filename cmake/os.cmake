if (WIN32)
    set(TENSERFLOW_OS_WIN ON)
elseif (APPLE)
    set(TENSERFLOW_OS_APPLE ON)

    if (IOS OR CMAKE_SYSTEM_NAME STREQUAL iOS)
        set(TENSERFLOW_OS_IOS ON)
    else()
        set(TENSERFLOW_OS_MACOS ON)
    endif()
else()
    set(TENSERFLOW_OS_UNIX ON)

    if (ANDROID OR CMAKE_SYSTEM_NAME MATCHES "Android")
        set(TENSERFLOW_OS_ANDROID ON)
    elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
        set(TENSERFLOW_OS_LINUX ON)
    elseif(CMAKE_SYSTEM_NAME STREQUAL FreeBSD)
        set(TENSERFLOW_OS_FREEBSD ON)
    endif()
endif()


if (TENSERFLOW_OS_WIN)
    add_definitions(/DWIN32)
    add_definitions(/DTENSERFLOW_OS_WIN)
elseif(TENSERFLOW_OS_APPLE)
    add_definitions(/DTENSERFLOW_OS_APPLE)

    if (TENSERFLOW_OS_IOS)
        add_definitions(/DTENSERFLOW_OS_IOS)
    else()
        add_definitions(/DTENSERFLOW_OS_MACOS)
    endif()
elseif(TENSERFLOW_OS_UNIX)
    add_definitions(/DTENSERFLOW_OS_UNIX)

    if (TENSERFLOW_OS_ANDROID)
        add_definitions(/DTENSERFLOW_OS_ANDROID)
    elseif (TENSERFLOW_OS_LINUX)
        add_definitions(/DTENSERFLOW_OS_LINUX)
    elseif (TENSERFLOW_OS_FREEBSD)
        add_definitions(/DTENSERFLOW_OS_FREEBSD)
    endif()
endif()
