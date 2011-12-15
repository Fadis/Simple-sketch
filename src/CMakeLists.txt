#add_definitions( -DRESOURCE_DIRECTORY=${CMAKE_CURRENT_SOURCE_DIR}/resources )

ADD_EXECUTABLE( sample main.cpp fb.cpp )
if( APPLE )
set_target_properties( sample PROPERTIES LINK_FLAGS "-framework Cocoa -framework OpenGL" )
endif( APPLE )
target_link_libraries( sample ${Boost_LIBRARIES} ${GLFW_LIBRARIES} ${CG_LIBRARY} ${CG_GL_LIBRARY} ${GLEW_LIBRARY} ${IL_LIBRARIES} ${ILU_LIBRARIES} )
