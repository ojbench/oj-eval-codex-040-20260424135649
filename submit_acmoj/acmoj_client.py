#!/usr/bin/env python3
import requests, json, argparse, os
from typing import Optional, Dict, Any
from datetime import datetime

class ACMOJClient:
    def __init__(self, token: str):
        self.api_base = https://acm.sjtu.edu.cn/OnlineJudge/api/v1
        self.headers = {
            Authorization: fBearer
