# 文件 rebuild.sh

# 进入当今脚本所在目录
cd $(dirname "$0")

if [ $# -lt 1 ]; then
    echo "请指定编译的项目"
    exit 1
fi

# 当前目录全路径
CURRENT_DIR=`pwd`
ROOT_DIR=$(dirname $CURRENT_DIR)

SRC_DIR=$1

if [ ! -d "$SRC_DIR" ]; then
    echo "项目 $SRC_DIR 不存在"
    exit 1
fi

if [ "$2" = "clang" ]; then
    ToolchainFile=$ROOT_DIR/toolchain/clang-default.cmake
else
    ToolchainFile=$ROOT_DIR/toolchain/gcc-default.cmake
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

cmake -B "$BUILD_DIR" -S $SRC_DIR --toolchain $ToolchainFile -DCMAKE_BUILD_TYPE=$BUILD_TYPE
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
