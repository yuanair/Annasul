add_executable(main)
target_sources(main PRIVATE src/main.cpp)
target_sources(main PRIVATE 
	FILE_SET all_my_modules TYPE CXX_MODULES
	 	FILES
			src/lib.cppm)