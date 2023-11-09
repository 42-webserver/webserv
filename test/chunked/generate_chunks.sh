#!/bin/bash
curl -v -X POST http://127.0.0.1/images/hi.txt \
-H "Transfer-Encoding: chunked" \
-H "Content-Type: application/json" \
-d '{"key1":"value1", "key2":"value2"}'
