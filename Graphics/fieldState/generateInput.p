use strict;
use warnings;

my $WIDTH = 1000;
my $HEIGHT = 1000;
my $FILE_HANDLE;
my $CASES = 10;

main();

sub main
{
	open($FILE_HANDLE, '>', 'data.txt');
	print $FILE_HANDLE "2\nShark s\nFish f\n$HEIGHT $WIDTH";
	printData();
	close($FILE_HANDLE);
	print "done\n";
}

sub printData
{
	my $switchNum;
	for(my $i = 0; $i < $HEIGHT; $i ++)
	{
		print $FILE_HANDLE "\n";
		for(my $j = 0; $j < $WIDTH; $j ++)
		{
			$switchNum = rand($CASES) % $CASES ;
			if($switchNum == 0)	{ print $FILE_HANDLE 's' }
			elsif($switchNum == 1){ print $FILE_HANDLE 'f' }
			else		{ print $FILE_HANDLE ' ' }
			
		}
	}
}