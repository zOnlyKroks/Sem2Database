#!/bin/bash

# Wait for any signal to begin the process
echo "Waiting for interaction..."

# Use `read` to pause and wait for a signal (an input event)
read -p "Press [Enter] to start Valgrind..."

# Once Enter is pressed, start the application with Valgrind
echo "Starting Valgrind..."
valgrind --leak-check=full ./prog1

# Clean up the generated files after execution
echo "Cleaning up temporary files..."

# Example cleanup commands (delete any temp files, binaries, etc.)
rm -f ./prog1

# Exit the container once the tasks are finished
echo "Finished! Exiting the container."
exit 0
