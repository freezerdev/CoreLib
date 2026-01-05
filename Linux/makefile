# Figure out compiler, default to clang
ifdef COMP
CC=$(COMP)
else
CC=clang++
endif

# Figure out the target architecture, default to 64-bit
ifndef ARCH
ARCH=-m64
endif

# Figure out the build type, default to debug
ifndef BUILD
BUILD=debug
endif

# Figure out the architecture folder
ifeq ($(ARCH), -m32)
ARCHDIR=x86
else
ARCHDIR=x64
endif

# Figure out the compiler flags and build folder
ifeq ($(BUILD), release)
CFLAGS=-std=c++14 -O3
BUILDDIR=Release
else
CFLAGS=-std=c++14 -g
BUILDDIR=Debug
endif

# Figure out the sanitizer flags (if any)
ifeq ($(SANITIZER), address)
SFLAGS=-fsanitize=address -fno-omit-frame-pointer
else ifeq ($(SANITIZER), thread)
SFLAGS=-fsanitize=thread
else ifeq ($(SANITIZER), memory)
SFLAGS=-fsanitize=memory -fno-omit-frame-pointer
else ifeq ($(SANITIZER), undefined)
SFLAGS=-fsanitize=undefined
endif

all: $(ARCHDIR)/$(BUILDDIR)/CityHash.o $(ARCHDIR)/$(BUILDDIR)/Container.o $(ARCHDIR)/$(BUILDDIR)/CoreLib.o $(ARCHDIR)/$(BUILDDIR)/CoreUtils.o $(ARCHDIR)/$(BUILDDIR)/Crc32.o $(ARCHDIR)/$(BUILDDIR)/Diagnostic.o $(ARCHDIR)/$(BUILDDIR)/DoubleNullStr.o $(ARCHDIR)/$(BUILDDIR)/Errors.o $(ARCHDIR)/$(BUILDDIR)/Event.o $(ARCHDIR)/$(BUILDDIR)/ExceptionUtils.o $(ARCHDIR)/$(BUILDDIR)/FilePath.o $(ARCHDIR)/$(BUILDDIR)/FilePathUtils.o $(ARCHDIR)/$(BUILDDIR)/FileSystemUtils.o $(ARCHDIR)/$(BUILDDIR)/FileTraverse.o $(ARCHDIR)/$(BUILDDIR)/IniFile.o $(ARCHDIR)/$(BUILDDIR)/IPv6.o $(ARCHDIR)/$(BUILDDIR)/Mac.o $(ARCHDIR)/$(BUILDDIR)/MemBuffer.o $(ARCHDIR)/$(BUILDDIR)/MemStream.o $(ARCHDIR)/$(BUILDDIR)/NetUtils.o $(ARCHDIR)/$(BUILDDIR)/PlatformUtils.o $(ARCHDIR)/$(BUILDDIR)/PowerUtils.o $(ARCHDIR)/$(BUILDDIR)/ProcessUtils.o $(ARCHDIR)/$(BUILDDIR)/RandUtils.o $(ARCHDIR)/$(BUILDDIR)/RegexUtils.o $(ARCHDIR)/$(BUILDDIR)/StlHelper.o $(ARCHDIR)/$(BUILDDIR)/Str.o $(ARCHDIR)/$(BUILDDIR)/StrUtils.o $(ARCHDIR)/$(BUILDDIR)/TextFile.o $(ARCHDIR)/$(BUILDDIR)/Thread.o $(ARCHDIR)/$(BUILDDIR)/ThreadUtils.o $(ARCHDIR)/$(BUILDDIR)/TimeUtils.o $(ARCHDIR)/$(BUILDDIR)/Url.o $(ARCHDIR)/$(BUILDDIR)/UrlUtils.o $(ARCHDIR)/$(BUILDDIR)/WhitespaceParser.o
	mkdir -p ../bin/Linux/$(ARCHDIR)/$(BUILDDIR)
	ar rcs ../bin/Linux/$(ARCHDIR)/$(BUILDDIR)/libCoreLib.a $(ARCHDIR)/$(BUILDDIR)/*.o

cleanall:
	rm -f ../bin/Linux/x86/Debug/libCoreLib.a
	rm -f ../bin/Linux/x86/Release/libCoreLib.a
	rm -f ../bin/Linux/x64/Debug/libCoreLib.a
	rm -f ../bin/Linux/x64/Release/libCoreLib.a
	rm -f x86/Debug/*.o
	rm -f x86/Release/*.o
	rm -f x64/Debug/*.o
	rm -f x64/Release/*.o

clean:
	rm -f ../bin/Linux/$(ARCHDIR)/$(BUILDDIR)/libCoreLib.a
	rm -f $(ARCHDIR)/$(BUILDDIR)/*.o

rebuild: clean all

$(ARCHDIR)/$(BUILDDIR)/CityHash.o: ../CityHash.cpp ../CityHash.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../CityHash.cpp -o $(ARCHDIR)/$(BUILDDIR)/CityHash.o

$(ARCHDIR)/$(BUILDDIR)/Container.o: ../Container.cpp ../Container.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../Container.cpp -o $(ARCHDIR)/$(BUILDDIR)/Container.o

$(ARCHDIR)/$(BUILDDIR)/CoreLib.o: ../CoreLib.cpp ../CoreLib.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../CoreLib.cpp -o $(ARCHDIR)/$(BUILDDIR)/CoreLib.o

$(ARCHDIR)/$(BUILDDIR)/CoreUtils.o: ../CoreUtils.cpp ../CoreUtils.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../CoreUtils.cpp -o $(ARCHDIR)/$(BUILDDIR)/CoreUtils.o

$(ARCHDIR)/$(BUILDDIR)/Crc32.o: ../Crc32.cpp ../Crc32.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../Crc32.cpp -o $(ARCHDIR)/$(BUILDDIR)/Crc32.o

$(ARCHDIR)/$(BUILDDIR)/Diagnostic.o: ../Diagnostic.cpp ../Diagnostic.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../Diagnostic.cpp -o $(ARCHDIR)/$(BUILDDIR)/Diagnostic.o

$(ARCHDIR)/$(BUILDDIR)/DoubleNullStr.o: ../DoubleNullStr.cpp ../DoubleNullStr.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../DoubleNullStr.cpp -o $(ARCHDIR)/$(BUILDDIR)/DoubleNullStr.o

$(ARCHDIR)/$(BUILDDIR)/Errors.o: ../Errors.cpp ../Errors.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../Errors.cpp -o $(ARCHDIR)/$(BUILDDIR)/Errors.o

$(ARCHDIR)/$(BUILDDIR)/Event.o: ../Event.cpp ../Event.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../Event.cpp -o $(ARCHDIR)/$(BUILDDIR)/Event.o

$(ARCHDIR)/$(BUILDDIR)/ExceptionUtils.o: ../ExceptionUtils.cpp ../ExceptionUtils.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../ExceptionUtils.cpp -o $(ARCHDIR)/$(BUILDDIR)/ExceptionUtils.o

$(ARCHDIR)/$(BUILDDIR)/FilePath.o: ../FilePath.cpp ../FilePath.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../FilePath.cpp -o $(ARCHDIR)/$(BUILDDIR)/FilePath.o

$(ARCHDIR)/$(BUILDDIR)/FilePathUtils.o: ../FilePathUtils.cpp ../FilePathUtils.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../FilePathUtils.cpp -o $(ARCHDIR)/$(BUILDDIR)/FilePathUtils.o

$(ARCHDIR)/$(BUILDDIR)/FileSystemUtils.o: ../FileSystemUtils.cpp ../FileSystemUtils.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../FileSystemUtils.cpp -o $(ARCHDIR)/$(BUILDDIR)/FileSystemUtils.o

$(ARCHDIR)/$(BUILDDIR)/FileTraverse.o: ../FileTraverse.cpp ../FileTraverse.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../FileTraverse.cpp -o $(ARCHDIR)/$(BUILDDIR)/FileTraverse.o

$(ARCHDIR)/$(BUILDDIR)/IniFile.o: ../IniFile.cpp ../IniFile.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../IniFile.cpp -o $(ARCHDIR)/$(BUILDDIR)/IniFile.o

$(ARCHDIR)/$(BUILDDIR)/IPv6.o: ../IPv6.cpp ../IPv6.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../IPv6.cpp -o $(ARCHDIR)/$(BUILDDIR)/IPv6.o

$(ARCHDIR)/$(BUILDDIR)/Mac.o: ../Mac.cpp ../Mac.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../Mac.cpp -o $(ARCHDIR)/$(BUILDDIR)/Mac.o

$(ARCHDIR)/$(BUILDDIR)/MemBuffer.o: ../MemBuffer.cpp ../MemBuffer.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../MemBuffer.cpp -o $(ARCHDIR)/$(BUILDDIR)/MemBuffer.o

$(ARCHDIR)/$(BUILDDIR)/MemStream.o: ../MemStream.cpp ../MemStream.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../MemStream.cpp -o $(ARCHDIR)/$(BUILDDIR)/MemStream.o

$(ARCHDIR)/$(BUILDDIR)/NetUtils.o: ../NetUtils.cpp ../NetUtils.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../NetUtils.cpp -o $(ARCHDIR)/$(BUILDDIR)/NetUtils.o

$(ARCHDIR)/$(BUILDDIR)/PlatformUtils.o: ../PlatformUtils.cpp ../PlatformUtils.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../PlatformUtils.cpp -o $(ARCHDIR)/$(BUILDDIR)/PlatformUtils.o

$(ARCHDIR)/$(BUILDDIR)/PowerUtils.o: ../PowerUtils.cpp ../PowerUtils.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../PowerUtils.cpp -o $(ARCHDIR)/$(BUILDDIR)/PowerUtils.o

$(ARCHDIR)/$(BUILDDIR)/ProcessUtils.o: ../ProcessUtils.cpp ../ProcessUtils.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../ProcessUtils.cpp -o $(ARCHDIR)/$(BUILDDIR)/ProcessUtils.o

$(ARCHDIR)/$(BUILDDIR)/RandUtils.o: ../RandUtils.cpp ../RandUtils.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../RandUtils.cpp -o $(ARCHDIR)/$(BUILDDIR)/RandUtils.o

$(ARCHDIR)/$(BUILDDIR)/RegexUtils.o: ../RegexUtils.cpp ../RegexUtils.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../RegexUtils.cpp -o $(ARCHDIR)/$(BUILDDIR)/RegexUtils.o

$(ARCHDIR)/$(BUILDDIR)/StlHelper.o: ../StlHelper.cpp ../StlHelper.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../StlHelper.cpp -o $(ARCHDIR)/$(BUILDDIR)/StlHelper.o

$(ARCHDIR)/$(BUILDDIR)/Str.o: ../Str.cpp ../Str.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../Str.cpp -o $(ARCHDIR)/$(BUILDDIR)/Str.o

$(ARCHDIR)/$(BUILDDIR)/StrUtils.o: ../StrUtils.cpp ../StrUtils.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../StrUtils.cpp -o $(ARCHDIR)/$(BUILDDIR)/StrUtils.o

$(ARCHDIR)/$(BUILDDIR)/TextFile.o: ../TextFile.cpp ../TextFile.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../TextFile.cpp -o $(ARCHDIR)/$(BUILDDIR)/TextFile.o

$(ARCHDIR)/$(BUILDDIR)/Thread.o: ../Thread.cpp ../Thread.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../Thread.cpp -o $(ARCHDIR)/$(BUILDDIR)/Thread.o

$(ARCHDIR)/$(BUILDDIR)/ThreadUtils.o: ../ThreadUtils.cpp ../ThreadUtils.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../ThreadUtils.cpp -o $(ARCHDIR)/$(BUILDDIR)/ThreadUtils.o

$(ARCHDIR)/$(BUILDDIR)/TimeUtils.o: ../TimeUtils.cpp ../TimeUtils.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../TimeUtils.cpp -o $(ARCHDIR)/$(BUILDDIR)/TimeUtils.o

$(ARCHDIR)/$(BUILDDIR)/Url.o: ../Url.cpp ../Url.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../Url.cpp -o $(ARCHDIR)/$(BUILDDIR)/Url.o

$(ARCHDIR)/$(BUILDDIR)/UrlUtils.o: ../UrlUtils.cpp ../UrlUtils.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../UrlUtils.cpp -o $(ARCHDIR)/$(BUILDDIR)/UrlUtils.o

$(ARCHDIR)/$(BUILDDIR)/WhitespaceParser.o: ../WhitespaceParser.cpp ../WhitespaceParser.h
	mkdir -p $(ARCHDIR)/$(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) $(ARCH) -c ../WhitespaceParser.cpp -o $(ARCHDIR)/$(BUILDDIR)/WhitespaceParser.o
