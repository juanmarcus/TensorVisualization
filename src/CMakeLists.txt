#FILE (GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
#FILE (GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

#SET (moc_headers
    
#)

#QT4_WRAP_CPP (moc_sources ${moc_headers})

#SET (full_sources
#    ${sources}
#    ${headers}
#    ${moc_sources}
#)

#ADD_EXECUTABLE (main ${full_sources})
#TARGET_LINK_LIBRARIES (main ${LIBS})

# c++ executables
FILE (GLOB cpp_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

FOREACH (cpp_source ${cpp_sources})
    get_filename_component (target ${cpp_source} NAME_WE)
    add_executable (${target} ${cpp_source})
    target_link_libraries (${target} ${LIBS})
ENDFOREACH (cpp_source)