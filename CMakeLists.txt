#    Copyright (C) 2011 Colony authors,
#    
#    This file is part of Colony.
#    
#    Colony is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Colony is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(ColonySources colony)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/tests")

ADD_LIBRARY(colony SHARED ${ColonySources})
SET_TARGET_PROPERTIES(colony PROPERTIES LINKER_LANGUAGE C)
TARGET_LINK_LIBRARIES(colony pthread)

ADD_LIBRARY(colony_static STATIC ${ColonySources})
SET_TARGET_PROPERTIES(colony_static PROPERTIES LINKER_LANGUAGE C)

SET(ColonyTestSources tests/colony_test)

ADD_EXECUTABLE(colony_test tests.c ${ColonyTestSources})
TARGET_LINK_LIBRARIES(colony_test colony check)

