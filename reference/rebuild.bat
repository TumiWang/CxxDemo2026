@echo off

:: GB2312 CRLF

CD /d %~dp0

SET ROOT_DIR=%CD%

IF "%1_x" == "_x" (
    echo 请指定编译的项目
    goto end
)

:: SET VC_VERSION=Visual Studio 15 2017
SET VC_VERSION=Visual Studio 17 2022
SET ARCH_TYPE=x64
SET BUILD_TYPE=Release

SET PRUDOCT_DIR=product
SET BUILD_DIR=build

:: clean
IF EXIST "%BUILD_DIR%" (
    RMDIR /s /q "%BUILD_DIR%"
)
IF EXIST "%PRUDOCT_DIR%" (
    RMDIR /s /q "%PRUDOCT_DIR%"
)

MKDIR "%BUILD_DIR%"
MKDIR "%PRUDOCT_DIR%"

:: configure
cmake -B "%BUILD_DIR%" -S %1 -G "%VC_VERSION%" -A %ARCH_TYPE%
IF %ERRORLEVEL% neq 0 (
    echo CMake Failed for configuring
    goto end
)

:: build
cmake --build "%BUILD_DIR%" --config %BUILD_TYPE%
IF %ERRORLEVEL% neq 0 (
    echo CMake Failed for building
    goto end
)

:: install
cmake --install "%BUILD_DIR%" --config %BUILD_TYPE% --prefix "%PRUDOCT_DIR%"
IF %ERRORLEVEL% neq 0 (
    echo CMake Failed for the installation
    goto end
)

:end
