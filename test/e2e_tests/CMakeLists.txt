add_test(NAME TreeE2E
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/test.sh 
            $<TARGET_FILE:hwt> 
            $<TARGET_FILE:set>
			${CMAKE_CURRENT_SOURCE_DIR}/data
)