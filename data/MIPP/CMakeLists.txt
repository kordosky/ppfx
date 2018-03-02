file(GLOB xml_files *.xml)
install( FILES ${xml_files} DESTINATION ${product}/${version}/data/MIPP )

file(GLOB root_files *.root)
install( FILES ${root_files} DESTINATION ${product}/${version}/data/MIPP )
