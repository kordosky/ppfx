# The install_headers macro expects that header files 
# will be in a directory named ppfx
# install_headers()

file(GLOB headers [^.]*.h)
set(header_install_dir ${${product}_inc_dir})
##message( STATUS "installing headers ${headers} in ${header_install_dir}")
INSTALL( FILES ${headers}
         DESTINATION ${header_install_dir} )
