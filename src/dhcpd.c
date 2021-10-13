/**
 * @file dhcpd.c
 * @author alirezaarzehgar (alirezaarzehgar82@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-10
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "dhcpd/dhcpd.h"
#include "network/listener.h"
#include "lease/lease.h"
#include "dhcpd/file.h"
#include "dhcpd/pid.h"
#include <getopt.h>

static char databasePath[MAX_DATABASE_PATH_LEN];

static struct option long_options[] =
{
  {"database", required_argument, NULL, 'f'},
  {"address", required_argument, NULL, 'a'},
  {"port", required_argument, NULL, 'p'},
  {"foreground", required_argument, NULL, 'd'},
  {"background", required_argument, NULL, 'b'},
};

dhcpNetworkPktInfo_t
getReplyDependencies (pktDhcpPacket_t
                      *pkt, char *serverIdentifier)
{
  dhcpLeaseInit (databasePath);

  dhcpLeasePoolResult_t lease = dhcpLeaseGetIpFromPool (pktMacHex2str (
                                  pkt->chaddr));

  dhcpLeaseClose();

  char *clientIp = malloc (DHCP_LEASE_IP_STR_LEN);

  unsigned int time = lease.config.lease_time;

  void *leaseTime = &time;

  char *netmask = malloc (DHCP_LEASE_SUBNET_STR_LEN);

  char *router = malloc (DHCP_LEASE_IP_STR_LEN);

  char *domain = malloc (DHCP_LEASE_DOMAIN_STR_MAX_LEN);

  strncpy (clientIp, lease.ip, DHCP_LEASE_IP_STR_LEN);

  strncpy (netmask, lease.config.mask, DHCP_LEASE_SUBNET_STR_LEN);

  strncpy (router, lease.config.router, DHCP_LEASE_IP_STR_LEN);

  strncpy (domain, lease.config.domain, DHCP_LEASE_DOMAIN_STR_MAX_LEN);

  dhcpNetworkPktInfo_t info =
  {
    .fields = {
      {.func = (pktGenCallbackFunc_t)pktGenFieldYourIpAddress, .param = clientIp},
      PKT_GEN_CALLBACK_NULL,
    },

    .options =
    {
      {.func = (pktGenCallbackFunc_t)pktGenOptDhcpServerIdentifier, .param = serverIdentifier},
      {.func = (pktGenCallbackFunc_t)pktGenOptIpAddrLeaseTime, .param = leaseTime},
      {.func = (pktGenCallbackFunc_t)pktGenOptSubnetMask, .param = netmask},
      {.func = (pktGenCallbackFunc_t)pktGenOptRouter, .param = router},
      {.func = (pktGenCallbackFunc_t)pktGenOptDomainName, .param = domain},
      PKT_GEN_CALLBACK_NULL,
    }
  };

  return info;
}

char *
ackHandler (pktDhcpPacket_t *pkt)
{
  int retval;

  char mac[DHCP_LEASE_MAC_STR_MAX_LEN + 2];

  memcpy (mac, pktMacHex2str (pkt->chaddr), DHCP_LEASE_MAC_STR_MAX_LEN + 2);

  retval = dhcpLeaseInit (databasePath);

  if (!retval)
    return "could not lease ip";

  dhcpLeasePoolResult_t lease = dhcpLeaseGetIpFromPool (mac);

  retval = dhcpLeaseIpAddress (lease.id, mac, pktGetHostName (pkt));

  dhcpLeaseClose();

  if (!retval)
    return "could not lease ip";

  return NULL;
}

int
main (int argc, char const *argv[])
{
  /* declaration */
  int retval;

  char address[DHCP_LEASE_IP_STR_LEN + 1];

  int port;

  int opt;

  int index_opt;

  int background = -1;

  /* default values */
  bzero (&address, sizeof (address));

  strcpy (databasePath, DHCP_DATABASE_PATH);

  port = 67;

  while ((opt = getopt_long (argc, (char *const *) argv, "f:a:p:db", long_options,
                             &index_opt)) != -1)
    {
      switch (opt)
        {
        case 'f':
          strncpy (databasePath, optarg, MAX_DATABASE_PATH_LEN);
          break;

        case 'a':
          /* TODO Address validation */
          if (true)
            strncpy (address, optarg, DHCP_LEASE_IP_STR_LEN + 1);

          break;

        case 'p':
          /* TODO Port validation*/
          if (true)
            port = atoi (optarg);
          break;

        case 'b':
          if (background == -1)
            background = true;
          break;

        case 'd':
          if (background == -1)
            background = false;
          break;
        }
    }

  if (!databaseExists (databasePath))
    {
      fprintf (stderr, "%s: %s\n", databasePath, strerror (ENOENT));
      return EXIT_FAILURE;
    }

  /* TODO need to better validation */
  if (address[0] == '\0')
    {
      fprintf (stderr, "address argument is required\n");
      return EXIT_FAILURE;
    }

  if (background == true)
    daemon (0, 0);

  saveCurrentPid ((char *)argv[0]);

  retval = dhcpNetworkListener (address, port, getReplyDependencies,
                                ackHandler);

  if (retval == EXIT_FAILURE)
    perror ("dhcpNetworkListener");

  return EXIT_SUCCESS;
}
