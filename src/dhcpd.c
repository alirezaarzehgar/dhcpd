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
#include "dhcpd/config.h"
#include "lease/lease.h"

dhcpNetworkPktInfo_t
getReplyDependencies (pktDhcpPacket_t
                      *discovery)
{
  char *serverIdentifier = "192.168.133.30";

  dhcpLeaseInit (DHCP_DATABASE_PATH);

  dhcpLeasePoolResult_t lease = dhcpLeaseGetIpFromPool();

  dhcpLeaseClose();

  char *clientIp = malloc (DHCP_LEASE_IP_STR_LEN);

  unsigned int leaseTime = lease.config.lease_time;

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
      {.func = (pktGenCallbackFunc_t)pktGenOptIpAddrLeaseTime, .param = (void *) leaseTime},
      {.func = (pktGenCallbackFunc_t)pktGenOptSubnetMask, .param = netmask},
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
  dhcpLeaseInit (DHCP_DATABASE_PATH);

  dhcpLeasePoolResult_t lease = dhcpLeaseGetIpFromPool();

  char mac[DHCP_LEASE_MAC_STR_MAX_LEN];

  char host[DHCP_LEASE_HOSTNAME_STR_MAX_LEN];

  strncpy (mac, pktMacHex2str (pkt->chaddr), DHCP_LEASE_HOSTNAME_STR_MAX_LEN);

  strncpy (host, pktGetHostName (pkt), DHCP_LEASE_HOSTNAME_STR_MAX_LEN);

  dhcpLeaseIpAddress (lease.id, mac, host);

  dhcpLeaseClose();

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
