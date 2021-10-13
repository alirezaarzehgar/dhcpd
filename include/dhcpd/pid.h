#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#if !defined(DHCPD_PID_H)
#define DHCPD_PID_H

#define DHCPD_RUNTIME_DIR               "/run/"

bool saveCurrentPid(char *filename);

#endif // DHCPD_PID_H
