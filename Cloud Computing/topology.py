#!/usr/bin/python
from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import Controller, RemoteController, OVSController
from mininet.cli import CLI
from mininet.log import setLogLevel, info
from mininet.util import dumpNodeConnections
from mininet.link import Link, Intf, TCLink
import os
from time import sleep
import sys


class Topology(Topo):


    def __init__(self):
        "Create Topology."

        # Initialize topology
        Topo.__init__(self)


        #### There is a rule of naming the hosts and switch, so please follow the rules like "h1", "h2" or "s1", "s2" for hosts and switches!!!!

        # Add hosts
        host1 = self.addHost('h1', ip='10.0.0.1/24', mac='00:00:00:00:00:01')
        host2 = self.addHost('h2', ip='10.0.0.2/24', mac='00:00:00:00:00:02')
        host3 = self.addHost('h3', ip='10.0.0.3/24', mac='00:00:00:00:00:03')
        host4 = self.addHost('h4', ip='10.0.0.4/24', mac='00:00:00:00:00:04')


        # Add switches
        switch1 = self.addSwitch('s1')
        switch2 = self.addSwitch('s2')
        switch3 = self.addSwitch('s3')
        switch4 = self.addSwitch('s4')


        self.addLink(host1, switch1)        # connect host1 with swtich A
        self.addLink(host2, switch2)
        self.addLink(host3, switch3)
        self.addLink(host4, switch4)

        self.addLink(switch1, switch2)    # connect switch 1 port 2 to switch 2 port 2
        self.addLink(switch1, switch4)
        self.addLink(switch2, switch3)
        self.addLink(switch4, switch3)


# This is for "mn --custom"
topos = { 'mytopo': ( lambda: Topology() ) }


# This is for "python *.py"
if __name__ == '__main__':
    setLogLevel( 'info' )

    topo = Topology()
    net = Mininet(topo=topo, link=TCLink, controller=OVSController)       # The TCLink is a special setting for setting the bandwidth in the future.

    # 1. Start mininet
    net.start()

    # Wait for links setup (sometimes, it takes some time to setup, so wait for a while before mininet starts)
    print "\nWaiting for links to setup . . . .",
    sys.stdout.flush()
    for time_idx in range(3):
        print ".",
        sys.stdout.flush()
        sleep(1)


    # 2. Start the CLI commands
    info( '\n*** Running CLI\n' )
    CLI( net )


    # 3. Stop mininet properly
    net.stop()
    print('Mininet stoped.')


    ### If you did not close the mininet, please run "mn -c" to clean up and re-run the mininet
