FROM python:3.12-slim

WORKDIR /app

# Install gcc for compile-checking in scorer
RUN apt-get update && apt-get install -y gcc && rm -rf /var/lib/apt/lists/*

# Install dependencies
COPY pyproject.toml README.md LICENSE ./
COPY src/ src/
RUN pip install --no-cache-dir ".[api]"

# Create data directory for API keys and usage
RUN mkdir -p /data
ENV SYNTHEX_DATA_DIR=/data

EXPOSE 8000

CMD ["uvicorn", "synthex.api.server:app", "--host", "0.0.0.0", "--port", "8000"]
