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
  dhcpNetworkPktInfo_t info =
  {
    .fields = {
      {.func = (pktGenCallbackFunc_t)pktGenFieldYourIpAddress, .param = TEST_FAKE_DATA_DHCP_NETWORK_YOUR_IP_ADDRESS},
      PKT_GEN_CALLBACK_NULL,
    },

    .options =
    {
      {.func = (pktGenCallbackFunc_t)pktGenOptDhcpServerIdentifier, .param = TEST_FAKE_DATA_DHCP_NETWORK_SERVER_IDENTIFIER},
      {.func = (pktGenCallbackFunc_t)pktGenOptIpAddrLeaseTime, .param = (void *)TEST_FAKE_DATA_DHCP_NETWORK_IP_ADDRESS_LEASE_TIME},
      {.func = (pktGenCallbackFunc_t)pktGenOptSubnetMask, .param = TEST_FAKE_DATA_DHCP_NETWORK_SUBNET_MASK},
      {.func = (pktGenCallbackFunc_t)pktGenOptRouter, .param = TEST_FAKE_DATA_DHCP_NETWORK_ROUTER},
      {.func = (pktGenCallbackFunc_t)pktGenOptDomainName, .param = TEST_FAKE_DATA_DHCP_NETWORK_DOMAIN_NAME},
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
                                getReplyDependencies, ackHandler);

  if (retval == EXIT_FAILURE)
    perror ("dhcpNetworkListener");

  return 0;
}
