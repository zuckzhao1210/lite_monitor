#!/bin/bash

protoc --proto_path=./ --cpp_out=../ report_message.proto
protoc --proto_path=./ --cpp_out=../  response_items.proto
protoc --proto_path=./ --cpp_out=../  request_items.proto
protoc --proto_path=./ --cpp_out=../  response_message.proto
