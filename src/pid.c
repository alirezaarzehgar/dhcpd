/**
 * @file pid.c
 * @author alirezaarzehgar (alirezaarzehgar82@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-13
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "dhcpd/pid.h"
#include <libgen.h>

bool
saveCurrentPid (char *filename)
{
  char path[strlen (filename) + strlen (DHCPD_RUNTIME_DIR) + 1];

  int retva;

  FILE *pidFile;

  sprintf (path, DHCPD_RUNTIME_DIR "%s.pid", basename(filename));

  pidFile = fopen (path, "w");

  if (pidFile == NULL)
    return false;

  fprintf (pidFile, "%d\n", getpid());

  fclose (pidFile);

  return true;
}