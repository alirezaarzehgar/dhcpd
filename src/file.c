/**
 * @file file.c
 * @author alirezaarzehgar (alirezaarzehgar82@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-13
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "dhcpd/file.h"
#include <unistd.h>
#include <fcntl.h>

bool
databaseExists (char *path)
{
  return access (path, F_OK) == 0 && access (path, W_OK) == 0;
}