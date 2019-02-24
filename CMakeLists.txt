# Copyright 2018, GlobalLogic Inc.
# All rights reserved.
#

cmake_minimum_required(VERSION 3.1)

project(spell_checker)

add_executable(spell_checker_st
    project/spell_checker.h
    project/spell_checker.cpp
    main_st.cpp
)

target_include_directories(spell_checker_st PUBLIC
    project
)

set_target_properties(spell_checker_st PROPERTIES
    CXX_STANDARD 14
)


add_executable(spell_checker_mt
    project/spell_checker.h
    project/spell_checker.cpp
   main_mt.cpp
)

target_include_directories(spell_checker_mt PUBLIC
    project
)

set_target_properties(spell_checker_mt PROPERTIES
    CXX_STANDARD 14
)

target_link_libraries(spell_checker_mt
    pthread
    )
