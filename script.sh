#!/bin/bash

# Define the port number
PORT=1601

# Find the PID using the specified port
PID=$(lsof -t -i:$PORT)

# Check if a process is using the port
if [ -z "$PID" ]; then
    echo "Port $PORT is not in use."
else
    # Kill the process using the port
    echo "Killing process $PID using port $PORT..."
    kill $PID
fi
