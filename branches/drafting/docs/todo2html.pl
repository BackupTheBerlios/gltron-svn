#/usr/bin/perl -w

$date = localtime;
print "<html>\n",
    "<head>\n",
	"<title>GLtron TODO list (updated $date)</title>\n",
	"<!-- generated by todo2html.pl, Andreas Umbach <marvin\@dataway.ch> -->\n",
	"<style type=\"text/css\">\n",
	"<-- h1,h2,h3,h4 { font-family:Helvetica; }\n",
	"    th { font-family:Helvetica; }\n",
	"    td { font-family:Helvetica; } -->\n",
	"</style>\n",
	"</head>\n",
	"<body bgcolor=white><h2>GLtron TODO list</h2>\n",
	"last update on <b>$date</b><p>\n";

$start = 1;
while(<>) {
	if(/^\(([^\)]*)\)/) {
		$priority = $1;
		s/^\($1\)//;
		print "<tr> <td> $priority <td> ", $_, "\n";
	} else {
		if(! $start) {
			print "</table>\n";
		}
		print "<h3>", $_, "\n";
		print "<table border=1><tr><th>Priority<th>Task\n";
		$start = 0;
	}
}
print "</table>\n",
	"</body></html>\n";

