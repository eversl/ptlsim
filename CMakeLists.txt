
#g++ CPP_FLAGS -MM ptlsim.cpp kernel.cpp mm.cpp superstl.cpp ptlhwdef.cpp decode-core.cpp decode-fast.cpp decode-complex.cpp decode-x87.cpp decode-sse.cpp lowlevel-64bit.S lowlevel-32bit.S linkstart.S linkend.S uopimpl.cpp dcache.cpp config.cpp datastore.cpp injectcode.cpp ptlcalls.c cpuid.cpp ptlstats.cpp klibc.cpp glibc.cpp mathlib.cpp syscalls.cpp makeusage.cpp  ooocore.cpp ooopipe.cpp oooexec.cpp seqcore.cpp branchpred.cpp  > .depend
#g++ CPP_FLAGS -c linkstart.S
#g++ CPP_FLAGS -c lowlevel-64bit.S
#g++ CPP_FLAGS -c ptlsim.cpp
#g++ CPP_FLAGS -c kernel.cpp
#g++ CPP_FLAGS -c mm.cpp
#g++ CPP_FLAGS -c ptlhwdef.cpp
#g++ CPP_FLAGS -c decode-core.cpp
#g++ CPP_FLAGS -c decode-fast.cpp
#g++ CPP_FLAGS -c decode-complex.cpp
#g++ CPP_FLAGS -c decode-x87.cpp
#g++ CPP_FLAGS -c decode-sse.cpp
#g++ CPP_FLAGS -c uopimpl.cpp
#g++ CPP_FLAGS -c datastore.cpp
#g++ CPP_FLAGS -m64 -O99 -fomit-frame-pointer -c injectcode.cpp -o injectcode-64bit.o
#g++ CPP_FLAGS -c seqcore.cpp
#g++ CPP_FLAGS -c superstl.cpp
#g++ CPP_FLAGS -c config.cpp
#g++ CPP_FLAGS -c mathlib.cpp
#g++ CPP_FLAGS -c syscalls.cpp
#g++ CPP_FLAGS -c klibc.cpp
#g++ CPP_FLAGS -c glibc.cpp
#g++ CPP_FLAGS -E -C stats.h > stats.i
#cat stats.i | ./dstbuild PTLsimStats > dstbuild.temp.cpp
#g++ CPP_FLAGS -DDSTBUILD -include stats.h dstbuild.temp.cpp superstl.o config.o mathlib.o syscalls.o glibc.o datastore.o ptlhwdef.o -o dstbuild.temp
#./dstbuild.temp > ptlsim.dst
#rm -f dstbuild.temp destbuild.temp.cpp stats.i
#objcopy -I binary -O elf64-x86-64 -B i386 --rename-section .data=.dst,alloc,load,readonly,data,contents ptlsim.dst ptlsim.dst.o
#g++ CPP_FLAGS -c linkend.S
#g++ CPP_FLAGS -c branchpred.cpp
#g++ CPP_FLAGS -c dcache.cpp
#g++ CPP_FLAGS -c ooocore.cpp
#g++ CPP_FLAGS -c ooopipe.cpp
#g++ CPP_FLAGS -c oooexec.cpp
#ld -g -O2 linkstart.o lowlevel-64bit.o ptlsim.o kernel.o mm.o ptlhwdef.o decode-core.o decode-fast.o decode-complex.o decode-x87.o decode-sse.o uopimpl.o datastore.o injectcode-64bit.o seqcore.o superstl.o config.o mathlib.o syscalls.o klibc.o ptlsim.dst.o linkend.o  branchpred.o dcache.o ooocore.o ooopipe.o oooexec.o  -o ptlsim  -static --allow-multiple-definition -T ptlsim.lds -e ptlsim_preinit_entry `gcc -print-libgcc-file-name`
#g++ CPP_FLAGS -c ptlstats.cpp
#g++ -O99 -g -fomit-frame-pointer -pipe -march=k8 -falign-functions=16 -funroll-loops -funit-at-a-time -minline-all-stringops -fno-trapping-math -fno-stack-protector -fno-exceptions -fno-rtti -funroll-loops -mpreferred-stack-boundary=4 -fno-strict-aliasing -fno-stack-protector -Wreturn-type  -D_FORTIFY_SOURCE=0 -D__PTLSIM_OOO_ONLY__ -g -O2 ptlstats.o datastore.o ptlhwdef.o superstl.o config.o mathlib.o syscalls.o glibc.o -o ptlstats
#g++ CPP_FLAGS -c ptlcalls.c
#g++ -c CPP_FLAGS -O99 -g -fomit-frame-pointer -pipe -march=k8 -falign-functions=16 -funroll-loops -funit-at-a-time -minline-all-stringops -fno-trapping-math -fno-stack-protector -fno-exceptions -fno-rtti -funroll-loops -mpreferred-stack-boundary=4 -fno-strict-aliasing -fno-stack-protector -Wreturn-type  -D_FORTIFY_SOURCE=0 -D__PTLSIM_OOO_ONLY__ -m32 -O99 -fomit-frame-pointer -Wa,--32 ptlcalls.c -o ptlcalls-32bit.o
#g++ CPP_FLAGS -c cpuid.cpp
#g++ -O99 -g -fomit-frame-pointer -pipe -march=k8 -falign-functions=16 -funroll-loops -funit-at-a-time -minline-all-stringops -fno-trapping-math -fno-stack-protector -fno-exceptions -fno-rtti -funroll-loops -mpreferred-stack-boundary=4 -fno-strict-aliasing -fno-stack-protector -Wreturn-type  -D_FORTIFY_SOURCE=0 -D__PTLSIM_OOO_ONLY__ -O2 cpuid.o superstl.o config.o mathlib.o syscalls.o glibc.o -o cpuid







cmake_minimum_required(VERSION 3.3)
project(ptlsim)
enable_language(ASM)
enable_testing()


exec_program("git" ${CMAKE_CURRENT_SOURCE_DIR}
        ARGS "describe" "--always"
        OUTPUT_VARIABLE VERSION)

exec_program("hostname" ARGS "-f" OUTPUT_VARIABLE BUILDHOST)

exec_program("date" OUTPUT_VARIABLE SVNDATE)

include_directories(.)

set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

add_compile_options(-O99 -g -fomit-frame-pointer -pipe -march=k8 -falign-functions=16 -funroll-loops -funit-at-a-time -minline-all-stringops -fno-trapping-math -fno-stack-protector -fno-exceptions -fno-rtti -funroll-loops -mpreferred-stack-boundary=4 -fno-strict-aliasing -fno-stack-protector)

add_definitions(-D_FORTIFY_SOURCE=0 -D__PTLSIM_OOO_ONLY__ -DBUILDHOST="${BUILDHOST}" -DSVNREV=0 -DGITREV="${VERSION}" -DSVNDATE="${SVNDATE}")


add_custom_command(OUTPUT stats.i COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} -I${PROJECT_SOURCE_DIR} -E -C ${PROJECT_SOURCE_DIR}/stats.h > ${CMAKE_BINARY_DIR}/stats.i)

add_custom_command(OUTPUT dstbuild.temp.cpp COMMAND cat ${CMAKE_BINARY_DIR}/stats.i | ${PROJECT_SOURCE_DIR}/dstbuild PTLsimStats > dstbuild.temp.cpp DEPENDS stats.i)

add_custom_command(OUTPUT ptlsim.dst COMMAND ${CMAKE_BINARY_DIR}/dstbuild.temp > ${CMAKE_BINARY_DIR}/ptlsim.dst)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/ptlsim.dst.o COMMAND objcopy -I binary -O elf64-x86-64 -B i386 --rename-section .data=.dst,alloc,load,readonly,data,contents ptlsim.dst ${CMAKE_BINARY_DIR}/ptlsim.dst.o DEPENDS ptlsim.dst) # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(ptlstats ptlstats.cpp datastore.cpp ptlhwdef.cpp superstl.cpp config.cpp mathlib.cpp syscalls.cpp glibc.cpp)

add_executable(cpuid cpuid.cpp superstl.cpp config.cpp mathlib.cpp syscalls.cpp glibc.cpp)

find_library(LIBGCC libgcc_s libgcc)

set(PLT_SOURCES linkstart.S lowlevel-64bit.S ptlsim.cpp kernel.cpp mm.cpp ptlhwdef.cpp decode-core.cpp decode-fast.cpp decode-complex.cpp decode-x87.cpp decode-sse.cpp uopimpl.cpp datastore.cpp injectcode.cpp seqcore.cpp superstl.cpp config.cpp mathlib.cpp syscalls.cpp klibc.cpp ${CMAKE_BINARY_DIR}/ptlsim.dst.o linkend.S branchpred.cpp dcache.cpp ooocore.cpp ooopipe.cpp oooexec.cpp)
add_executable(ptlsim ${PLT_SOURCES})
set_target_properties(ptlsim PROPERTIES LINK_FLAGS "-nostdlib -O2 -Wl,-static,--allow-multiple-definition,--script=${PROJECT_SOURCE_DIR}/ptlsim.lds,--entry=ptlsim_preinit_entry")
add_dependencies(ptlsim dstbuild.temp)

add_executable(dstbuild.temp dstbuild.temp.cpp superstl.cpp config.cpp mathlib.cpp syscalls.cpp glibc.cpp datastore.cpp ptlhwdef.cpp)
set_source_files_properties(dstbuild.temp.cpp PROPERTIES COMPILE_FLAGS "-DDSTBUILD -include stats.h")

#set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(tests/semantics)
