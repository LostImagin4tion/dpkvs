# DPKVS

DPKVS stands for Distributed Persistent Key-Value Store. It is non-relational (NoSQL) database for storing key-value pairs.

Built with C++, so DPKVS is ⚡ blazingly fast ⚡ and 🔥 memory-unsafe 🔥

## Features

* Persistence using Append-Only Log and Protobuf serialization
* gRPC API
* Logging using [spdlog](https://github.com/gabime/spdlog)
* Configuration using [yaml-cpp](https://github.com/jbeder/yaml-cpp)
* Distributed (WIP)
* Master-Slave Replication (WIP)
* Data Partitioning using Consistent Hashing (WIP)
* Eventual Consistency (WIP)

### Persistence

As for now, data is stored in in-memory hash map. But every modification operation (`PUT` or `REMOVE`) is being written to Append-Only Log on a disk.

Every operation log consists of:
* Header (log size, CRC32C and LSN)
* Payload (key and value)

| Field               | Size       |
|---------------------|------------|
| log size            | 4 bytes    |
| CRC32C checksum     | 4 bytes    |
| Log Sequence Number | 2-10 bytes |
| Payload proto       | variant    |

Payload proto consists of:

| Field | Size                                                                                 |
|-------|--------------------------------------------------------------------------------------|
| key   | 2 bytes + 1 byte * key_size                                                          |
| value | 0 bytes, if operation == `REMOVE`. <br/>Else 2 + N bytes, where N - value proto size |

Value proto consists of: 

| Field                        | Size                          |
|------------------------------|-------------------------------|
| value itself                 | 2 bytes + 1 byte * value_size |
| additional flags size        | 1 byte + 1 byte * flags_size  |
| expiry timestamp             | 9 bytes                       |

Therefore, log size is:
* `[12, 20] + key_size` bytes for `REMOVE` operations
* `[26, 34] + key_size + value_size + flags_size` bytes for `PUT` operations

For example, size of `put("hello", "world"), flags=1, expiry=24h` operation will be just 37 bytes:
* Log Sequence Number takes 2 bytes
* key_size and value_size are equal to 5 bytes
* flags_size is equal to 1 byte

It will be serialized to:

```shell
root:~$ xxd -g 1 append-only-log.txt
00000000: 1d 00 00 00 38 b3 83 90 12 1b 0a 05 68 65 6c 6c  ....8.......hell
00000010: 6f 12 12 12 05 77 6f 72 6c 64 18 01 20 b5 83 98  o....world.. ...
00000020: e9 ea 9b 8f 03                                   .....
```

## Code Style

This project follows [YTsaurus code style conventions](https://github.com/ytsaurus/ytsaurus/blob/main/yt/styleguide/cpp.md).

## Testing

Code is being covered with unit and integration test using Google Test framework.

## Deploy

To be done
