import time
from fastapi import Request, HTTPException, status
from collections import defaultdict

class TokenBucketRateLimiter:
    def __init__(self, requests_per_minute: int = 120):
        self.rate = requests_per_minute / 60.0
        self.capacity = requests_per_minute
        self.buckets = defaultdict(lambda: [time.time(), self.capacity])

    def check(self, client_ip: str):
        now = time.time()
        last_check, tokens = self.buckets[client_ip]
        tokens = min(self.capacity, tokens + (now - last_check) * self.rate)
        if tokens < 1.0:
            raise HTTPException(
                status_code=status.HTTP_429_TOO_MANY_REQUESTS,
                detail="Rate limit exceeded. Please retry in a moment."
            )
        self.buckets[client_ip] = [now, tokens - 1.0]

limiter = TokenBucketRateLimiter()

async def rate_limit_middleware(request: Request, call_next):
    client_ip = request.client.host if request.client else "127.0.0.1"
    limiter.check(client_ip)
    return await call_next(request)
