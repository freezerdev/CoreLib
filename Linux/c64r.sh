#!/bin/bash

if [ "$1" == "Address" ] || [ "$1" == "address" ]; then
	make COMP=clang++ ARCH=-m64 BUILD=release SANITIZER=address
elif [ "$1" == "Thread" ] || [ "$1" == "thread" ]; then
	make COMP=clang++ ARCH=-m64 BUILD=release SANITIZER=thread
elif [ "$1" == "Memory" ] || [ "$1" == "memory" ]; then
	make COMP=clang++ ARCH=-m64 BUILD=release SANITIZER=memory
elif [ "$1" == "Undefined" ] || [ "$1" == "undefined" ]; then
	make COMP=clang++ ARCH=-m64 BUILD=release SANITIZER=undefined
else
	make COMP=clang++ ARCH=-m64 BUILD=release
fi
