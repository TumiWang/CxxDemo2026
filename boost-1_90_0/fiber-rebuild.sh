# 文件 accumulators-rebuild.sh

# 进入当今脚本所在目录
cd $(dirname "$0")

if [ $# -lt 1 ]; then
    echo "请指定编译的项目"
    exit 1
fi

# 当前目录全路径
PROJECT_DIR=fiber
CURRENT_DIR=`pwd`
ROOT_DIR=$(dirname $CURRENT_DIR)

SRC_DIR=$1

if [ ! -d "$CURRENT_DIR/$PROJECT_DIR/$SRC_DIR" ]; then
    echo "项目 $CURRENT_DIR/$PROJECT_DIR/$SRC_DIR 不存在"
    exit 1
fi

SysArch=`uname -m`
if [ "$SysArch" = "arm64" ]; then
    SysArch=aarch64
fi
if [ "$(uname -s)" = "Darwin" ]; then
    ToolchainFile=$ROOT_DIR/toolchain/maxos-static_libs-$SysArch.cmake
else
    # 这时, 认为是交叉编译的Linux环境
    # 例如: run_docker_image.sh 启动的docker 环境
    ToolchainFile=$ROOT_DIR/toolchain/ubuntu20_04-boost_1_90_0-$SysArch.cmake
fi

# 设置三个变量 分别表示安装目录、编译目录和编译类型
PRODUCT_DIR=product
BUILD_DIR=build
BUILD_TYPE=Release

# 清除 编译目录
# 如果之前编译过，这样就是重新编译
if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
fi
# 清理安装目录
if [ -d "$PRODUCT_DIR" ]; then
    rm -rf "$PRODUCT_DIR"
fi

cmake -B "$BUILD_DIR" -S $CURRENT_DIR/$PROJECT_DIR/$SRC_DIR --toolchain $ToolchainFile -DCMAKE_BUILD_TYPE=$BUILD_TYPE
if [ $? != 0 ]; then
    echo "cmake创建项目失败"
    exit 0
fi

cmake --build "$BUILD_DIR" --config $BUILD_TYPE
if [ $? != 0 ]; then
    echo "cmake编译失败"
    exit 0
fi

cmake --install "$BUILD_DIR" --config $BUILD_TYPE --prefix "$CURRENT_DIR/$PRODUCT_DIR"
if [ $? != 0 ]; then
    echo "cmake安装失败"
    exit 0
fi
