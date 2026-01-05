#!/bin/bash

if [ "$1" == "Address" ] || [ "$1" == "address" ]; then
	make COMP=g++ ARCH=-m32 BUILD=debug SANITIZER=address
elif [ "$1" == "Thread" ] || [ "$1" == "thread" ]; then
	make COMP=g++ ARCH=-m32 BUILD=debug SANITIZER=thread
elif [ "$1" == "Memory" ] || [ "$1" == "memory" ]; then
	make COMP=g++ ARCH=-m32 BUILD=debug SANITIZER=memory
elif [ "$1" == "Undefined" ] || [ "$1" == "undefined" ]; then
	make COMP=g++ ARCH=-m32 BUILD=debug SANITIZER=undefined
else
	make COMP=g++ ARCH=-m32 BUILD=debug
fi
