# av-app
proof of concept for an antivirus app service

## Build
- make

## Run
- make run_server
- make run_client ARGS=<'path'>

## Run with valgrind
- make run_server_vg
- make run_client_vg ARGS=<'path'>

## Clean
- make clean

# TODO:
- implemented only single file transfer, need recursive folder
- need integrate thread-pool event processing
- need integrate parallel aho corasick (multi pattern matching) for plugins