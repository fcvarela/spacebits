#!/usr/bin/perl -w

use strict;
use Data::Dumper;
use SOAP::Lite;
use utf8;

our $serviceSMSProxy;
our $serviceUser;
our $servicePass;

do "/servers/spacebits/config/sms.pl";

my $serviceSMS = SOAP::Lite
  -> uri ( 'SMSPTGW/SOAP/SMS' )
  -> proxy ( $serviceSMSProxy,
  )
;

my $serviceLoginResult = $serviceSMS -> serviceLogin (
  SOAP::Data -> name ( 'serviceUser' => $serviceUser ),
  SOAP::Data -> name ( 'servicePass' => $servicePass )
);

my $serviceSessionId;
if ( $serviceLoginResult -> fault ) {
 print "-ERRO: ", $serviceLoginResult -> faultcode, " ", $serviceLoginResult -> faultstring, "\n";
 die;
}
else {
  $serviceSessionId = $serviceLoginResult -> result;
}
die unless $serviceSessionId;

my $smsSendResult = $serviceSMS -> smsSend (
    SOAP::Data -> name ( 'serviceSessionId' => $serviceSessionId ),
    SOAP::Data -> name ( 'smsTo' => $ARGV[0]),
    SOAP::Data -> name ( 'smsMessage' => $ARGV[1]),
    SOAP::Data -> name ( 'smsSMSC' => 'tmn'),
);

if ( $smsSendResult -> fault ) {
 print "-ERRO: ", $smsSendResult -> faultcode, " ", $smsSendResult -> faultstring, "\n";
 die;
}
else {
 print "RESULT: ", $smsSendResult -> result, "\n";
}
