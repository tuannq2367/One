protoc --cpp_out=. ./ServiceCore.proto
protoc --grpc_out=. ./ServiceCore.proto --plugin=protoc-gen-grpc="C:\Workspaces\vcpkg\installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe"