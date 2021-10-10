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

dhcpNetworkPktInfo_t
getReplyDependencies (pktDhcpPacket_t
                      *discovery)
{
  char *clientIpAddress = TEST_FAKE_DATA_DHCP_NETWORK_YOUR_IP_ADDRESS;

  char *serverIdentifier = TEST_FAKE_DATA_DHCP_NETWORK_SERVER_IDENTIFIER;

  int leaseTime = TEST_FAKE_DATA_DHCP_NETWORK_IP_ADDRESS_LEASE_TIME;

  char *subnet = TEST_FAKE_DATA_DHCP_NETWORK_SUBNET_MASK;

  char *router = TEST_FAKE_DATA_DHCP_NETWORK_ROUTER;

  char *domain = TEST_FAKE_DATA_DHCP_NETWORK_DOMAIN_NAME;

  dhcpNetworkPktInfo_t info =
  {
    .fields = {
      {.func = (pktGenCallbackFunc_t)pktGenFieldYourIpAddress, .param = clientIpAddress},
      PKT_GEN_CALLBACK_NULL,
    },

    .options =
    {
      {.func = (pktGenCallbackFunc_t)pktGenOptDhcpServerIdentifier, .param = serverIdentifier},
      {.func = (pktGenCallbackFunc_t)pktGenOptIpAddrLeaseTime, .param = (void *)leaseTime},
      {.func = (pktGenCallbackFunc_t)pktGenOptSubnetMask, .param = subnet},
      {.func = (pktGenCallbackFunc_t)pktGenOptRouter, .param = router},
      {.func = (pktGenCallbackFunc_t)pktGenOptDomainName, .param = domain},
      PKT_GEN_CALLBACK_NULL,
    }
  };

  (void *)discovery;      /* Quit unused warning */

  return info;
}

char *
ackHandler (pktDhcpPacket_t *pkt)
{
  printf ("lease!");
  return NULL;
}

int
main (int argc, char const *argv[])
{
  /* declaration */
  int retval;

  char address[PKT_ADDR_MAX_LEN];

  int port;

  /* definition */
  strcpy (address, "192.168.133.30");

  port = 67;

  retval = dhcpNetworkListener (address, port, getReplyDependencies,
                                ackHandler);

  if (retval == EXIT_FAILURE)
    perror ("dhcpNetworkListener");

  return 0;
}
