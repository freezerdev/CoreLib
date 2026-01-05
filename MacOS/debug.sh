#!/bin/bash

if [ "$1" == "Address" ] || [ "$1" == "address" ]; then
	make BUILD=debug SANITIZER=address
elif [ "$1" == "Thread" ] || [ "$1" == "thread" ]; then
	make BUILD=debug SANITIZER=thread
elif [ "$1" == "Memory" ] || [ "$1" == "memory" ]; then
	make BUILD=debug SANITIZER=memory
elif [ "$1" == "Undefined" ] || [ "$1" == "undefined" ]; then
	make BUILD=debug SANITIZER=undefined
else
	make BUILD=debug
fi
