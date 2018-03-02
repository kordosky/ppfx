file(GLOB root_files *.root)
install( FILES ${root_files} DESTINATION ${product}/${version}/data/MC )

add_subdirectory(QGSP)
add_subdirectory(FTFP)