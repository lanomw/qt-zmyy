cmake_minimum_required(VERSION 3.21)
project(qt_zmyy)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#cmake路径
set(CMAKE_PREFIX_PATH "D:/Qt/5.15.2/mingw81_64/lib/cmake")

#复制资源
file(COPY src/area.json DESTINATION assets FOLLOW_SYMLINK_CHAIN)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

add_executable(qt_zmyy main.cpp
        src/MainWidget.cpp
        src/MainWidget.h
        src/Config.h
        src/model/Storage.h
        src/util/WeChatUtil.cpp
        src/util/WeChatUtil.h
        src/AreaBox.cpp
        src/AreaBox.h
        src/model/AreaCate.h
        src/model/Cate.h
        src/model/Template.h
        src/model/Hospital.h
        src/model/Product.h
        src/model/HospitalDetail.h
        src/model/NumbersVaccine.h
        src/model/SubDate.h
        src/lib/Base64.cpp
        src/lib/Base64.h
        src/lib/AES.cpp
        src/lib/AES.h src/lib/Hex.cpp src/lib/Hex.h src/util/CryptoUtil.cpp src/util/CryptoUtil.h src/model/DateDetail.h src/model/OrderPost.h src/util/Http.cpp src/util/Http.h)
target_link_libraries(qt_zmyy
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        )

if (WIN32)
    set(DEBUG_SUFFIX)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets Network)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
