# Set compiler to clang
CC=clang++

# Figure out the build type, default to debug
ifndef BUILD
BUILD=debug
endif

# Figure out the compiler flags
ifeq ($(BUILD), release)
CFLAGS=-std=c++14 -O3 -arch arm64 -arch x86_64 -target arm64-apple-macos10.15 -mmacosx-version-min=10.15
BUILDDIR=Release
else
CFLAGS=-std=c++14 -g -O0 -arch arm64 -arch x86_64 -target arm64-apple-macos10.15 -mmacosx-version-min=10.15
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

all: $(BUILDDIR)/CityHash.o $(BUILDDIR)/Container.o $(BUILDDIR)/CoreLib.o $(BUILDDIR)/CoreUtils.o $(BUILDDIR)/Crc32.o $(BUILDDIR)/Diagnostic.o $(BUILDDIR)/DoubleNullStr.o $(BUILDDIR)/Errors.o $(BUILDDIR)/Event.o $(BUILDDIR)/ExceptionUtils.o $(BUILDDIR)/FilePath.o $(BUILDDIR)/FilePathUtils.o $(BUILDDIR)/FileSystemUtils.o $(BUILDDIR)/FileTraverse.o $(BUILDDIR)/IniFile.o $(BUILDDIR)/IPv6.o $(BUILDDIR)/Mac.o $(BUILDDIR)/MemBuffer.o $(BUILDDIR)/MemStream.o $(BUILDDIR)/NetUtils.o $(BUILDDIR)/Object.o $(BUILDDIR)/PlatformUtils.o $(BUILDDIR)/PowerUtils.o $(BUILDDIR)/ProcessUtils.o $(BUILDDIR)/RandUtils.o $(BUILDDIR)/RegexUtils.o $(BUILDDIR)/StlHelper.o $(BUILDDIR)/Str.o $(BUILDDIR)/StrUtils.o $(BUILDDIR)/TextFile.o $(BUILDDIR)/Thread.o $(BUILDDIR)/ThreadUtils.o $(BUILDDIR)/TimeUtils.o $(BUILDDIR)/Url.o $(BUILDDIR)/UrlUtils.o $(BUILDDIR)/WhitespaceParser.o
	mkdir -p ../bin/MacOS/$(BUILDDIR)
	libtool -static -o ../bin/MacOS/$(BUILDDIR)/libCoreLib.a $(BUILDDIR)/*.o

cleanall:
	rm -f ../bin/MacOS/Debug/libCoreLib.a
	rm -f ../bin/MacOS/Release/libCoreLib.a
	rm -f Debug/*.o
	rm -f Release/*.o

clean:
	rm -f ../bin/MacOS/$(BUILDDIR)/libCoreLib.a
	rm -f $(BUILDDIR)/*.o

rebuild: clean all

$(BUILDDIR)/CityHash.o: ../CityHash.cpp ../CityHash.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../CityHash.cpp -o $(BUILDDIR)/CityHash.o

$(BUILDDIR)/Container.o: ../Container.cpp ../Container.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../Container.cpp -o $(BUILDDIR)/Container.o

$(BUILDDIR)/CoreLib.o: ../CoreLib.cpp ../CoreLib.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../CoreLib.cpp -o $(BUILDDIR)/CoreLib.o

$(BUILDDIR)/CoreUtils.o: ../CoreUtils.cpp ../CoreUtils.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../CoreUtils.cpp -o $(BUILDDIR)/CoreUtils.o

$(BUILDDIR)/Crc32.o: ../Crc32.cpp ../Crc32.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../Crc32.cpp -o $(BUILDDIR)/Crc32.o

$(BUILDDIR)/Diagnostic.o: ../Diagnostic.cpp ../Diagnostic.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../Diagnostic.cpp -o $(BUILDDIR)/Diagnostic.o

$(BUILDDIR)/DoubleNullStr.o: ../DoubleNullStr.cpp ../DoubleNullStr.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../DoubleNullStr.cpp -o $(BUILDDIR)/DoubleNullStr.o

$(BUILDDIR)/Errors.o: ../Errors.cpp ../Errors.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../Errors.cpp -o $(BUILDDIR)/Errors.o

$(BUILDDIR)/Event.o: ../Event.cpp ../Event.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../Event.cpp -o $(BUILDDIR)/Event.o

$(BUILDDIR)/ExceptionUtils.o: ../ExceptionUtils.cpp ../ExceptionUtils.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../ExceptionUtils.cpp -o $(BUILDDIR)/ExceptionUtils.o

$(BUILDDIR)/FilePath.o: ../FilePath.cpp ../FilePath.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../FilePath.cpp -o $(BUILDDIR)/FilePath.o

$(BUILDDIR)/FilePathUtils.o: ../FilePathUtils.cpp ../FilePathUtils.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../FilePathUtils.cpp -o $(BUILDDIR)/FilePathUtils.o

$(BUILDDIR)/FileSystemUtils.o: ../FileSystemUtils.cpp ../FileSystemUtils.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../FileSystemUtils.cpp -o $(BUILDDIR)/FileSystemUtils.o

$(BUILDDIR)/FileTraverse.o: ../FileTraverse.cpp ../FileTraverse.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../FileTraverse.cpp -o $(BUILDDIR)/FileTraverse.o

$(BUILDDIR)/IniFile.o: ../IniFile.cpp ../IniFile.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../IniFile.cpp -o $(BUILDDIR)/IniFile.o

$(BUILDDIR)/IPv6.o: ../IPv6.cpp ../IPv6.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../IPv6.cpp -o $(BUILDDIR)/IPv6.o

$(BUILDDIR)/Mac.o: ../Mac.cpp ../Mac.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../Mac.cpp -o $(BUILDDIR)/Mac.o

$(BUILDDIR)/MemBuffer.o: ../MemBuffer.cpp ../MemBuffer.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../MemBuffer.cpp -o $(BUILDDIR)/MemBuffer.o

$(BUILDDIR)/MemStream.o: ../MemStream.cpp ../MemStream.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../MemStream.cpp -o $(BUILDDIR)/MemStream.o

$(BUILDDIR)/NetUtils.o: ../NetUtils.cpp ../NetUtils.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../NetUtils.cpp -o $(BUILDDIR)/NetUtils.o

$(BUILDDIR)/Object.o: ../Object.cpp ../Object.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../Object.cpp -o $(BUILDDIR)/Object.o

$(BUILDDIR)/PlatformUtils.o: ../PlatformUtils.cpp ../PlatformUtils.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../PlatformUtils.cpp -o $(BUILDDIR)/PlatformUtils.o

$(BUILDDIR)/PowerUtils.o: ../PowerUtils.cpp ../PowerUtils.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../PowerUtils.cpp -o $(BUILDDIR)/PowerUtils.o

$(BUILDDIR)/ProcessUtils.o: ../ProcessUtils.cpp ../ProcessUtils.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../ProcessUtils.cpp -o $(BUILDDIR)/ProcessUtils.o

$(BUILDDIR)/RandUtils.o: ../RandUtils.cpp ../RandUtils.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../RandUtils.cpp -o $(BUILDDIR)/RandUtils.o

$(BUILDDIR)/RegexUtils.o: ../RegexUtils.cpp ../RegexUtils.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../RegexUtils.cpp -o $(BUILDDIR)/RegexUtils.o

$(BUILDDIR)/StlHelper.o: ../StlHelper.cpp ../StlHelper.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../StlHelper.cpp -o $(BUILDDIR)/StlHelper.o

$(BUILDDIR)/Str.o: ../Str.cpp ../Str.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../Str.cpp -o $(BUILDDIR)/Str.o

$(BUILDDIR)/StrUtils.o: ../StrUtils.cpp ../StrUtils.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../StrUtils.cpp -o $(BUILDDIR)/StrUtils.o

$(BUILDDIR)/TextFile.o: ../TextFile.cpp ../TextFile.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../TextFile.cpp -o $(BUILDDIR)/TextFile.o

$(BUILDDIR)/Thread.o: ../Thread.cpp ../Thread.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../Thread.cpp -o $(BUILDDIR)/Thread.o

$(BUILDDIR)/ThreadUtils.o: ../ThreadUtils.cpp ../ThreadUtils.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../ThreadUtils.cpp -o $(BUILDDIR)/ThreadUtils.o

$(BUILDDIR)/TimeUtils.o: ../TimeUtils.cpp ../TimeUtils.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../TimeUtils.cpp -o $(BUILDDIR)/TimeUtils.o

$(BUILDDIR)/Url.o: ../Url.cpp ../Url.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../Url.cpp -o $(BUILDDIR)/Url.o

$(BUILDDIR)/UrlUtils.o: ../UrlUtils.cpp ../UrlUtils.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../UrlUtils.cpp -o $(BUILDDIR)/UrlUtils.o

$(BUILDDIR)/WhitespaceParser.o: ../WhitespaceParser.cpp ../WhitespaceParser.h
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SFLAGS) -c ../WhitespaceParser.cpp -o $(BUILDDIR)/WhitespaceParser.o
