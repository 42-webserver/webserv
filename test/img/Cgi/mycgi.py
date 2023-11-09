#!/usr/bin/env python
import os
import sys
import datetime

# 스크립트가 위치한 디렉토리의 절대 경로를 얻습니다.
script_directory = os.path.dirname(os.path.abspath(__file__))

def write_response(status, content_type='text/plain', body='', headers=None):
    headers = headers or {}
    sys.stdout.write(f"Content-Type: {content_type}\r\n")
    sys.stdout.write(f"Status: {status}\r\n")
    for header, value in headers.items():
        sys.stdout.write(f"{header}: {value}\r\n")
    sys.stdout.write("\r\n")  # Header and body separation
    sys.stdout.write(body)
    sys.stdout.flush()

def log_request(file_path, method, status, additional_info=''):
    # 로그 파일의 절대 경로를 설정합니다.
    log_file_path = os.path.join(script_directory, 'log.txt')
    timestamp = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    log_message = f"{timestamp} - Method: {method}, Path: {file_path}, Status: {status}, Info: {additional_info}\n"
    with open(log_file_path, 'a') as log_file:
        log_file.write(log_message)

def main():
    method = os.environ.get('REQUEST_METHOD', 'GET')
    path_info = os.environ.get('PATH_INFO', '').lstrip('/\\')
    file_name = os.path.basename(path_info)
    # 파일 작업을 위한 경로를 스크립트 디렉토리 기준으로 설정합니다.
    file_path = os.path.normpath(os.path.join(script_directory, path_info))

    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    input_data = sys.stdin.buffer.read(content_length) if content_length else b''

    if method == 'POST':
        action = 'append'
        if os.path.exists(file_path):
            with open(file_path, 'ab') as file:
                file.write(input_data)
            status = '200 OK'
            additional_info = 'Data appended successfully'
        else:
            status = '400 Bad Request'
            additional_info = 'File does not exist'

    elif method == 'PUT':
        action = 'create/replace'
        with open(file_path, 'wb') as file:
            file.write(input_data)
        status = '200 OK'
        additional_info = 'File created or replaced successfully'

    elif method == 'DELETE':
        action = 'delete'
        if os.path.exists(file_path):
            os.remove(file_path)
            status = '200 OK'
            additional_info = 'File deleted successfully'
        else:
            status = '404 Not Found'
            additional_info = 'File not found'

    else:
        action = 'unknown'
        status = '405 Method Not Allowed'
        additional_info = 'Method not supported'

    write_response(status, body=additional_info)
    log_request(file_path, method, status, f"Action: {action}, Info: {additional_info}")

if __name__ == "__main__":
    main()
