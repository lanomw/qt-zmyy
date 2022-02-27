cmake_minimum_required(VERSION 3.21)
project(zmyy)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# cmake路径
set(CMAKE_PREFIX_PATH "D:/Qt/5.15.2/mingw81_64/lib/cmake")
# minGW路径。用于windows环境下dll拷贝
set(MINGW_PREFIX_PATH "D:/Qt/Tools/mingw810_64/bin")
# 解决exe运行多出dos窗口
set(CMAKE_CXX_FLAGS -mwindows)
# 静态链接（解决编译后在无环境下运行缺少各种dll）
set(CMAKE_EXE_LINKER_FLAGS -static)
# 输出路径
set(EXECUTABLE_OUTPUT_PATH ./build)
# 需要使用的Qt模块
set(REQUIRED_LIBS Core Gui Widgets Network)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

#复制资源
file(COPY src/area.json DESTINATION ./build/assets FOLLOW_SYMLINK_CHAIN)
# 复制dll至打包目录
if (WIN32 AND MINGW_PREFIX_PATH)
    file(COPY ${MINGW_PREFIX_PATH}/libgcc_s_seh-1.dll DESTINATION ./build FOLLOW_SYMLINK_CHAIN)
    file(COPY ${MINGW_PREFIX_PATH}/libstdc++-6.dll DESTINATION ./build FOLLOW_SYMLINK_CHAIN)
    file(COPY ${MINGW_PREFIX_PATH}/libwinpthread-1.dll DESTINATION ./build FOLLOW_SYMLINK_CHAIN)
endif ()

add_executable(zmyy main.cpp
        src/Config.h
        src/model/Storage.h
        #        dll注入工具，仅windows下可用
        #        src/util/WeChatUtil.cpp
        #        src/util/WeChatUtil.h
        src/ui/AreaBox.cpp
        src/ui/AreaBox.h
        src/model/AreaCate.h
        src/model/Cate.h
        src/model/Template.h
        src/model/Hospital.h
        src/model/Product.h
        src/model/HospitalDetail.h
        src/model/NumbersVaccine.h
        src/model/SubDate.h
        src/lib/QtAES/qaesencryption.cpp
        src/lib/QtAES/qaesencryption.h
        src/lib/Hex.cpp
        src/lib/Hex.h
        src/util/CryptoUtil.cpp
        src/util/CryptoUtil.h
        src/model/DateDetail.h
        src/model/OrderPost.h
        src/util/Http.cpp
        src/util/Http.h
        src/ui/MainWidget.cpp
        src/ui/MainWidget.h
        src/service/MainService.cpp
        src/service/MainService.h
        src/util/JWT.cpp
        src/util/JWT.h)

# 查找Qt包
find_package(Qt5 COMPONENTS ${REQUIRED_LIBS} REQUIRED)

# 将Qt包链接到可执行文件
target_link_libraries(zmyy ${REQUIRED_LIBS_QUALIFIED})

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
    # 拷贝windows相关的库到目标文件所在的目录
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    # 拷贝每个需要的Qt库到目标文件所在的目录
    foreach (QT_LIB ${REQUIRED_LIBS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
