
BUILD_DIR := build/

all:
	cmake -B ${BUILD_DIR}
	cmake --build ${BUILD_DIR}
	build/Debug/app.exe

clean:
	rm -rf ${BUILD_DIR}
