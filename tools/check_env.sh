
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