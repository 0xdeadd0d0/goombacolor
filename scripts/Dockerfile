# Latest Devkitarm ARM (GBA, 3DS, DS, etc.)
FROM devkitpro/devkitarm:latest

RUN apt-get update && apt-get install -y --no-install-recommends \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

COPY --from=ghcr.io/astral-sh/uv:latest /uv /uvx /bin/

WORKDIR /workspace

RUN uv venv /opt/venv --python 3.14.7

RUN uv pip install jinja2 ftfy --python /opt/venv

CMD ["make"]
