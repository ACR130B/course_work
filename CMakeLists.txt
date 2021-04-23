cmake_minimum_required(VERSION 3.17)
project(vet)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_PREFIX_PATH C:/Qt/5.15.2/msvc2019_64)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(vet app.cpp main_window.cpp main_window.h)
find_package(Qt5 COMPONENTS Widgets Core Gui Sql REQUIRED)
target_link_libraries(vet PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Sql)

# Установка исполняемого файла в необходимую директорию
install(TARGETS vet DESTINATION bin)

# Установка базы данных
install(FILES ${PROJECT_SOURCE_DIR}/database.db DESTINATION bin)

# Установка документации
install(DIRECTORY ${PROJECT_SOURCE_DIR}/documentation/ DESTINATION bin/docs/)

# Установка компонентов Qt5
install(FILES $<TARGET_FILE:Qt5::Core> DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Gui> DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Widgets> DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Sql> DESTINATION bin)

# Установка SQL драйверов
install(FILES $<TARGET_FILE:Qt5::QSQLiteDriverPlugin> DESTINATION bin/sqldrivers)

# Установка плагинов для платформы windows
install(FILES $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> DESTINATION bin/platforms)
install(FILES $<TARGET_FILE:Qt5::QWindowsDirect2DIntegrationPlugin> DESTINATION bin/platforms)