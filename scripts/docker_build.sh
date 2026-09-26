#!/usr/bin/env bash
set -e

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
IMAGE_NAME="devkitarm-uv-goomba"

echo "=== Build docker image ==="
docker build -t "$IMAGE_NAME" -f "$REPO_ROOT/scripts/Dockerfile" "$REPO_ROOT"

echo "=== Build goombacolor ==="
if [ $# -eq 0 ]; then
    docker run --rm -v "$REPO_ROOT:/workspace" "$IMAGE_NAME" make clean all
else
    docker run --rm -v "$REPO_ROOT:/workspace" "$IMAGE_NAME" make "$@"
fi
