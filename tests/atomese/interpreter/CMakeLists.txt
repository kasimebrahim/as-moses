ADD_CXXTEST(ProgramInterpreterUTest)
TARGET_LINK_LIBRARIES(ProgramInterpreterUTest
	atomese
	${COGUTIL_LIBRARY}
	${ATOMSPACE_atombase_LIBRARY}
)