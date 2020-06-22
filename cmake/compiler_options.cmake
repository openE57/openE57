if(MSVC)
  
  list(APPEND compiler_options 
    /W4
    /permissive-
    $<$<CONFIG:RELEASE>:/O2 /Ob2 >
    $<$<CONFIG:MINSIZEREL>:/O1 /Ob1>
    $<$<CONFIG:RELWITHDEBINFO>:/Zi /O2 /Ob1>
    $<$<CONFIG:DEBUG>:/Zi /Ob0 /Od /RTC1>)

  list(APPEND compiler_definitions
    _UNICODE
    WINDOWS
    $<$<OR:$<CONFIG:RELEASE>,$<CONFIG:RELWITHDEBINFO>,$<CONFIG:MINSIZEREL>>:NDEBUG>
    $<$<CONFIG:DEBUG>:_DEBUG>)

  list(APPEND linker_flags
    $<$<BOOL:${BUILD_SHARED_LIBS}>:/LTCG>
  )

  set(MSVC_RUNTIME_TYPE $<IF:$<BOOL:${BUILD_WITH_MT}>,MultiThreaded$<$<CONFIG:Debug>:Debug>,MultiThreaded$<$<CONFIG:Debug>:Debug>>DLL)

else(MSVC)

  list(APPEND compiler_options 
      -Wall
      -Wextra
      -Wpedantic
      $<$<CONFIG:RELEASE>:-O2>
      $<$<CONFIG:DEBUG>:-O0>
      $<$<CONFIG:DEBUG>:-g>
      $<$<CONFIG:DEBUG>:-p>
      $<$<CONFIG:DEBUG>:-pg>
      $<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>)

  list(APPEND compiler_definitions
   $<$<OR:$<CONFIG:RELEASE>,$<CONFIG:MINSIZEREL>>:_FORTIFY_SOURCE=2>
  )

 list(APPEND linker_flags
 -Wl,-z,defs
 -Wl,-z,now
 -Wl,-z,relro
 $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:-Wl,-pie>
 $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:-fpie>
 $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:-pipe>
 $<$<BOOL:${BUILD_SHARED_LIBS}>:-static-libstdc++>
 $<$<CONFIG:DEBUG>:-fno-omit-frame-pointer>
 $<$<CONFIG:DEBUG>:-fsanitize=address>
 $<$<CONFIG:DEBUG>:-fsanitize=leak>
 $<$<CONFIG:DEBUG>:-fsanitize=undefined>
 -fstack-protector
 -fbounds-check
 -fstack-clash-protection
 -fPIC)

endif()