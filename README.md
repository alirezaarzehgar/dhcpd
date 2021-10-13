# dhcpd
 Dhcpd is a dhcp server daemon.

### Uses
 - dhcp-lease
 - dhcp-network

### Dependencies
 - cunit
 - sqlite3 (for lease module)
 - python3 (for lease module)

# Build and run

First of all you should install `dhcp-lease` dependencies

```bash
apt install libcunit1-dev libsqlite3-dev $ python3 python3-pip
```

Installing python requirements

```bash
pip3 install -r dhcp-lease/tests/scripts/requirements.txt
```

Building project

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Now you have a fake pool and config database on `build/dhcp-lease/tests/databases/dhcpd.db`
path that your program use it.

You can copy `dhcpd.db` to `/var/lib/dhcp` and change <a href="https://github.com/alirezaarzehgar/dhcpd/blob/06882725d075de465b113ee2535e01f508f098d1/config.h.in#L1">this</a> line.

```c
#define DHCP_DATABASE_PATH                       "dhcp-lease/tests/databases/dhcpd.db"
```

Then you can run your server

```
# ./dhcpd
```