#!/bin/bash

# Wait for any signal to begin the process
echo "Waiting for interaction..."

# Use `read` to pause and wait for a signal (an input event)
read -p "Press [Enter] to start Valgrind..."

# Once Enter is pressed, start the application with Valgrind
echo "Starting Valgrind..."
valgrind --leak-check=full ./prog1
