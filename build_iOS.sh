rm ./UDPServer.a
rm ./UDPClient.a
cmake -DCMAKE_TOOLCHAIN_FILE=./ios.cmake -DIOS_PLATFORM=OS -H. -Bbuild.ios -GXcode
cmake --build build.ios/ --config Release
strip -x -S build.ios/Release-iphoneos/libUDPServer.a -o ./UDPServer.a
strip -x -S build.ios/Release-iphoneos/libUDPClient.a -o ./UDPClient.a
rm -r ./build.ios