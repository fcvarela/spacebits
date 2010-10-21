#!/usr/bin/perl -n

if($_ =~ m{<script.*\bsrc="(.*?)".*></script>}){
    print "<script type=\"text/javascript\"><!--\n";
    system('cat', $1);
    print "--></script>\n";
}elsif($_ =~ m{<link\s+rel="stylesheet"\s+type="text/css"\s+href="(.*?)"\s*/>}){
    print "<style type=\"text/css\"><!--\n";
    system('cat', $1);
    print "--></style>\n";
}else{
    print $_;
}
