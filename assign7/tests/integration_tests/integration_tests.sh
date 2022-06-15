#!/bin/bash

# Declare server binary path
BIN_PATH=../build/bin/webserver

# Start webserver locally
$BIN_PATH ../config/test_config &

# Need this to pass tests
sleep 0.1

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

# TEST 5: Valid Create
# Clean the test dir
rm -rf ../database/test
rm -rf entity_info.txt
sleep 0.1

# Curl server and create
timeout $TIMEOUT curl -s -i -X POST -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT/api/test -d '{"a":111}'

# Check if file exists or not
FILE=../database/test/1

test -f "$FILE";
RESULT=$?

if [ $RESULT -eq 0 ];
then
    TEST_5_SUCCESS=1
    echo "$FILE exists."
else 
    echo "$FILE NOT exists."
fi

# TEST 6: Valid Get
TEST_6_SUCCESS=0

# Curl server and get list
timeout $TIMEOUT curl -s -i -X GET -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT/api/test > integration_test_6_result

# Check if file exists or not
diff integration_tests/integration_test_6_expected integration_test_6_result

RESULT=$?

rm integration_test_6_result

if [ $RESULT -eq 0 ];
then
    TEST_6_SUCCESS=1
    echo "Test 6 Success"
else
    echo "Test 6 Failure"
fi

# TEST 7: Valid Retrieve
TEST_7_SUCCESS=0

# Curl server and get one entity
timeout $TIMEOUT curl -s -i -X GET -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT/api/test/12 > integration_test_7_result

# Check if file exists or not
diff integration_tests/integration_test_7_expected integration_test_7_result

RESULT=$?

rm integration_test_7_result

if [ $RESULT -eq 0 ];
then
    TEST_7_SUCCESS=1
    echo "Test 7 Success"
else
    echo "Test 7 Failure"
fi

# TEST 8: Valid Update
TEST_8_SUCCESS=0

# Curl server and update entity
timeout $TIMEOUT curl -s -i -d '{"a":123}' -X PUT -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT/api/test/6

# Curl server and get updated entity
timeout $TIMEOUT curl -s -i -X GET -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT/api/test/6 > integration_test_8_result

# Check if file exists or not
diff integration_tests/integration_test_8_expected integration_test_8_result

RESULT=$?

rm integration_test_8_result

if [ $RESULT -eq 0 ];
then
    TEST_8_SUCCESS=1
    echo "Test 8 Success"
else
    echo "Test 8 Failure"
fi

# TEST 9: Valid delete
TEST_9_SUCCESS=0

# Curl server and delete
timeout $TIMEOUT curl -s -i -X DELETE -H "Host:" -H "User-Agent:" $SERVER_IP:$SERVER_PORT/api/test/6

# Check if file exists or not
FILE=../database/test/6

test -f "$FILE";
RESULT=$?

if [ $RESULT -ne 0 ];
then
    TEST_9_SUCCESS=1
    echo "$FILE NOT exists."
else 
    echo "$FILE exists."
fi

rm -rf ../database/test
rm -rf entity_info.txt

# Stop server
kill -9 $SERVER_PID

# Return success or failure
if [ $TEST_1_SUCCESS -ne 1 ] || [ $TEST_2_SUCCESS -ne 1 ] || 
   [ $TEST_3_SUCCESS -ne 1 ] || [ $TEST_4_SUCCESS -ne 1 ] || 
   [ $TEST_6_SUCCESS -ne 1 ] || [ $TEST_7_SUCCESS -ne 1 ] || 
   [ $TEST_8_SUCCESS -ne 1 ] || [ $TEST_9_SUCCESS -ne 1 ];
then
    exit 1
fi

exit 0