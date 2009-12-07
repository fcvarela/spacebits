#!/usr/bin/env perl

#/***************************************************************************
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# ***************************************************************************/

use IO::Socket;
use strict;

my $udp_sock = IO::Socket::INET->new(Proto=>'udp', PeerPort=>6666, PeerAddr=>'127.0.0.1') or die "Could not create udp socket: $!\n";
my $udp_msg = 'a file to have your advice';
for (my $count=0; $count<1000000; $count++) {
    $udp_sock->send($udp_msg) or die "Send error: $!\n"; 
}

