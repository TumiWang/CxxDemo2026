cd $(dirname "$0")

SysArch=`uname -m`
if [ "$SysArch" = "arm64" ]; then
    SysArch=aarch64
fi

if command -v docker >/dev/null 2>&1; then
    DockerTool=docker
elif command -v podman >/dev/null 2>&1; then
    DockerTool=podman
else
    echo "没有找到 docker 或 podman"
    exit 1
fi

$DockerTool run --rm -it -w /source -v $(pwd):/source crosscomiling/$SysArch:clang20 bash