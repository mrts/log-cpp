CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
SET (CMAKE_BACKWARDS_COMPATIBILITY 2.6)

PROJECT(logcpp)

# -- Unicode, highest warning level, warning as error

IF(WIN32)

  ADD_DEFINITIONS(-DUNICODE -D_UNICODE -UMBCS -U_MBCS)
  ADD_DEFINITIONS("/W4 /WX")

ELSE(WIN32)

  SET(CMAKE_CXX_COMPILER clang++)
  ADD_DEFINITIONS("-Wall -Werror -ferror-limit=5")

  FIND_PATH(PCSC_INCLUDE_DIRS WinSCard.h)
  INCLUDE_DIRECTORIES(${PCSC_INCLUDE_DIRS})

ENDIF(WIN32)

# -- libioccpp

INCLUDE_DIRECTORIES("lib/ioc-cpp/include")

# --- liblogcpp

INCLUDE_DIRECTORIES("include")

FILE(GLOB LIBLOGCPP_SRC
        src/[^.]*.cpp)

ADD_LIBRARY(liblogcpp STATIC ${LIBLOGCPP_SRC})

# -- libtestcpp

INCLUDE_DIRECTORIES("test/test-cpp/include")

FILE(GLOB LIBTESTCPP_SRC
	test/test-cpp/src/[^.]*.cpp)

ADD_LIBRARY(libtestcpp STATIC ${LIBTESTCPP_SRC})

# --- logcpp-test

FILE (GLOB LOGCPPTEST_SRC
        test/src/[^.]*.cpp)

ADD_EXECUTABLE(${PROJECT_NAME}-test ${LOGCPPTEST_SRC})

TARGET_LINK_LIBRARIES(${PROJECT_NAME}-test libtestcpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}-test liblogcpp)
