from fastapi import Request, HTTPException, status
from ..config import settings

async def verify_api_key(request: Request):
    if not settings.enable_auth:
        return
    api_key = request.headers.get("X-API-Key")
    if not api_key or api_key != settings.api_key:
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Invalid or missing X-API-Key header"
        )
