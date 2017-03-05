use strict;
use warnings;
use 5.010;
 
my $DATA_LINES_LENGTH = 1000;
my $MAX_DATA = 10000;
my $MAX_DELTA = 100;
my $FILE_HANDLE;

main();

sub main
{
	open($FILE_HANDLE, '>', 'data.txt');
	print $FILE_HANDLE "2\n";
	print $FILE_HANDLE "$DATA_LINES_LENGTH\nSharks\n";
	printDataLine();
	print $FILE_HANDLE "\nFishes\n";
	printDataLine();
	close $FILE_HANDLE;
	print "done\n";
}

sub printDataLine
{
	my $bFirst = 1;
	my $currentNum;
	state $lastNum = 0;
	for(1 .. $DATA_LINES_LENGTH)
	{
		if(!$bFirst)
		{
			print $FILE_HANDLE " ";
		}
		else
		{
			$bFirst = 0;
		}
		$currentNum = rand($MAX_DELTA);
		$currentNum *= ($currentNum % 2 == 0) ? -1: 1;
		$currentNum += $lastNum;
		if($currentNum < 0)
		{
			$currentNum = -$currentNum;
		}
		elsif($currentNum > $MAX_DATA)
		{
			#print "$currentNum ";
			$currentNum = ($MAX_DATA << 1) - $currentNum;
			#print "$currentNum\n";
		}
		$lastNum = $currentNum;
		$currentNum =~ s/\.\d+//;		
		
		print $FILE_HANDLE $currentNum;
	}
}