#!/bin/bash

# Declare server binary path
BIN_PATH=../build/bin/webserver

# Start webserver locally
$BIN_PATH ../config/test_config &

# Save server information
SERVER_PID=$!
SERVER_IP=localhost
SERVER_PORT=8080

# Set test timeout
TIMEOUT=0.5

# TEST 1: Valid HTTP echo request to server
TEST_1_SUCCESS=0

# Curl server and save response with the host and user-agent headers removed
timeout $TIMEOUT curl -s -i -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT/echo > integration_test_1_result

# Check if response matches expected or not
diff integration_tests/integration_test_1_expected integration_test_1_result

RESULT=$?

rm integration_test_1_result

if [ $RESULT -eq 0 ];
then
    TEST_1_SUCCESS=1
    echo "Test 1 Success"
else
    echo "Test 1 Failure"
fi

# TEST 2: Invalid empty request to server
TEST_2_SUCCESS=0

# Start netcat with timeout and expect no response
printf "hello" | timeout $TIMEOUT nc $SERVER_IP $SERVER_PORT > integration_test_2_result

# Check for empty response
diff integration_tests/integration_test_2_expected integration_test_2_result

RESULT=$?

rm integration_test_2_result

if [ $RESULT -eq 0 ];
then
    TEST_2_SUCCESS=1
    echo "Test 2 Success"
else
    echo "Test 2 Failure"
fi

# TEST 3: Invalid HTTP request to server
TEST_3_SUCCESS=0

# Curl server and save response with the host and user-agent headers removed
timeout $TIMEOUT curl -s -i -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT > integration_test_3_result

# Check if response matches expected or not
diff integration_tests/integration_test_3_expected integration_test_3_result

RESULT=$?

rm integration_test_3_result

if [ $RESULT -eq 0 ];
then
    TEST_3_SUCCESS=1
    echo "Test 3 Success"
else
    echo "Test 3 Failure"
fi

# TEST 4: Valid HTTP static request to server
TEST_4_SUCCESS=0

# Curl server and save response with the host and user-agent headers removed
timeout $TIMEOUT curl -s -i -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT/static1/example.txt > integration_test_4_result

# Check if response matches expected or not
diff integration_tests/integration_test_4_expected integration_test_4_result

RESULT=$?

rm integration_test_4_result

if [ $RESULT -eq 0 ];
then
    TEST_4_SUCCESS=1
    echo "Test 4 Success"
else
    echo "Test 4 Failure"
fi

# Stop server
kill -9 $SERVER_PID

# Return success or failure
if [ $TEST_1_SUCCESS -ne 1 ] || [ $TEST_2_SUCCESS -ne 1 ] || 
   [ $TEST_3_SUCCESS -ne 1 ] || [ $TEST_4_SUCCESS -ne 1 ];
then
    exit 1
fi

exit 0