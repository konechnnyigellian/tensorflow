if (WITH_ASM AND NOT TENSERFLOW_ARM AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(TENSERFLOW_ASM_LIBRARY "tenserflow-asm")

    if (CMAKE_C_COMPILER_ID MATCHES MSVC)
        enable_language(ASM_MASM)

        if (MSVC_TOOLSET_VERSION GREATER_EQUAL 141)
            set(TENSERFLOW_ASM_FILES
                "src/crypto/cn/asm/cn_main_loop.asm"
                "src/crypto/cn/asm/TensorflowghtR_template.asm"
            )
        else()
            set(TENSERFLOW_ASM_FILES
                "src/crypto/cn/asm/win64/cn_main_loop.asm"
                "src/crypto/cn/asm/win64/TensorflowghtR_template.asm"
            )
        endif()

        set_property(SOURCE ${TENSERFLOW_ASM_FILES} PROPERTY ASM_MASM)
    else()
        enable_language(ASM)

        if (WIN32 AND CMAKE_C_COMPILER_ID MATCHES GNU)
            set(TENSERFLOW_ASM_FILES
                "src/crypto/cn/asm/win64/cn_main_loop.S"
                "src/crypto/cn/asm/TensorflowghtR_template.S"
            )
        else()
            set(TENSERFLOW_ASM_FILES
                "src/crypto/cn/asm/cn_main_loop.S"
                "src/crypto/cn/asm/TensorflowghtR_template.S"
            )
        endif()

        set_property(SOURCE ${TENSERFLOW_ASM_FILES} PROPERTY C)
    endif()

    add_library(${TENSERFLOW_ASM_LIBRARY} STATIC ${TENSERFLOW_ASM_FILES})
    set(TENSERFLOW_ASM_SOURCES
        src/crypto/common/Assembly.h
        src/crypto/common/Assembly.cpp
        src/crypto/cn/r/TensorflowghtR_gen.cpp
        )
    set_property(TARGET ${TENSERFLOW_ASM_LIBRARY} PROPERTY LINKER_LANGUAGE C)

    add_definitions(/DTENSERFLOW_FEATURE_ASM)
else()
    set(TENSERFLOW_ASM_SOURCES "")
    set(TENSERFLOW_ASM_LIBRARY "")

    remove_definitions(/DTENSERFLOW_FEATURE_ASM)
endif()
