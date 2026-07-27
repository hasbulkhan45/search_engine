import os
from pydantic import BaseModel

class Settings(BaseModel):
    app_name: str = "SearchEngine API"
    environment: str = os.getenv("ENV", "development")
    api_key: str = os.getenv("API_KEY", "dev-secret-key-12345")
    db_path: str = os.getenv("DB_PATH", "search_engine.db")
    host: str = "0.0.0.0"
    port: int = 8000
    enable_auth: bool = False

settings = Settings()
# Auth: multiple authorized API keys
