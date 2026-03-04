# 文件 rebuild.sh

# 进入当今脚本所在目录
cd $(dirname "$0")

# 当前目录全路径
ROOT_DIR=`pwd`

ClangToolChain=$ROOT_DIR/toolchain/clang.cmake
GccToolChain=$ROOT_DIR/toolchain/gcc.cmake

# SRC_DIR=demo1
# SRC_DIR=demo2
SRC_DIR=demo3

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

cmake -B "$BUILD_DIR" -S $SRC_DIR --toolchain $ClangToolChain -DCMAKE_BUILD_TYPE=$BUILD_TYPE
if [ $? != 0 ]; then
    echo "cmake创建项目失败"
    exit 0
fi

cmake --build "$BUILD_DIR" --config $BUILD_TYPE
if [ $? != 0 ]; then
    echo "cmake编译失败"
    exit 0
fi

cmake --install "$BUILD_DIR" --config $BUILD_TYPE --prefix "$ROOT_DIR/$PRODUCT_DIR"
if [ $? != 0 ]; then
    echo "cmake安装失败"
    exit 0
fi
