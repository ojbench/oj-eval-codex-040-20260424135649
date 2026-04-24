#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import requests
import json
import argparse
import os
from typing import Optional, Dict, Any
from datetime import datetime

class ACMOJClient:
    def __init__(self, token: str):
        self.api_base = "https://acm.sjtu.edu.cn/OnlineJudge/api/v1"
        self.headers = {
            "Authorization": f"Bearer {token}",
            "Content-Type": "application/x-www-form-urlencoded",
            "User-Agent": "ACMOJ-Python-Client/2.2",
        }
        self.submission_log_file = "/workspace/submission_ids.log"

    def _make_request(self, method: str, endpoint: str, data: Optional[Dict[str, Any]] = None, params: Optional[Dict[str, Any]] = None) -> Optional[Dict[str, Any]]:
        url = f"{self.api_base}{endpoint}"
        try:
            if method.upper() == "GET":
                r = requests.get(url, headers=self.headers, params=params, timeout=10, proxies={"https": None, "http": None})
            elif method.upper() == "POST":
                r = requests.post(url, headers=self.headers, data=data, timeout=10, proxies={"https": None, "http": None})
            else:
                print(f"Unsupported HTTP method: {method}")
                return None
            if r.status_code == 204:
                return {"status": "success", "message": "Operation successful"}
            r.raise_for_status()
            return r.json() if r.content else {"status": "success"}
        except requests.exceptions.RequestException as e:
            print(f"API Request failed: {e}")
            try:
                print(f"Response text: {r.text}")
            except Exception:
                pass
            return None

    def _save_submission_id(self, sid: int) -> None:
        try:
            ts = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            with open(self.submission_log_file, "a", encoding="utf-8") as f:
                f.write(json.dumps({"timestamp": ts, "submission_id": sid}) + "\n")
            print(f"Submission ID {sid} saved to {self.submission_log_file}")
        except Exception as e:
            print(f"Warning: Failed to save submission ID: {e}")

    def submit_code(self, problem_id: int, language: str, code_text: str) -> Optional[Dict[str, Any]]:
        res = self._make_request("POST", f"/problem/{problem_id}/submit", data={"language": language, "code": code_text})
        if res and "id" in res:
            self._save_submission_id(res["id"])
        return res

    def get_submission_detail(self, submission_id: int) -> Optional[Dict[str, Any]]:
        return self._make_request("GET", f"/submission/{submission_id}")

    def abort_submission(self, submission_id: int) -> Optional[Dict[str, Any]]:
        return self._make_request("POST", f"/submission/{submission_id}/abort")


def main() -> None:
    ap = argparse.ArgumentParser(description="ACMOJ API Command Line Client")
    ap.add_argument("--token", default=os.environ.get("ACMOJ_TOKEN"), help="ACMOJ Access Token")
    sub = ap.add_subparsers(dest="cmd", required=True)

    p_submit = sub.add_parser("submit", help="Submit a C++ source file")
    p_submit.add_argument("--problem-id", type=int, required=True)
    p_submit.add_argument("--language", required=True)
    p_submit.add_argument("--code-file", required=True)

    p_status = sub.add_parser("status", help="Check submission status")
    p_status.add_argument("--submission-id", type=int, required=True)

    p_abort = sub.add_parser("abort", help="Abort submission evaluation")
    p_abort.add_argument("--submission-id", type=int, required=True)

    args = ap.parse_args()

    if not args.token:
        print("Error: Access token not provided. Use --token or set ACMOJ_TOKEN environment variable.")
        return

    client = ACMOJClient(args.token)

    if args.cmd == "submit":
        try:
            with open(args.code_file, "r", encoding="utf-8") as f:
                code_text = f.read()
        except FileNotFoundError:
            print(f"Error: Code file not found at {args.code_file}")
            raise SystemExit(1)
        except Exception as e:
            print(f"Error: Failed to read code file: {e}")
            raise SystemExit(1)
        result = client.submit_code(args.problem_id, args.language, code_text)
    elif args.cmd == "status":
        result = client.get_submission_detail(args.submission_id)
    else:
        result = client.abort_submission(args.submission_id)

    if result:
        print(json.dumps(result))
    else:
        raise SystemExit(1)

if __name__ == "__main__":
    main()
