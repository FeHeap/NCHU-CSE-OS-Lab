#!/usr/bin/perl
use strict;

my $numbers = @ARGV;
my $target;

if($numbers == 0 || $numbers == 1) {
	print "Welcome to the Number Guessing Game!\n";
	
	if($numbers == 0) {
		print "Please enter the target number(1~10):";
		chomp($target = <STDIN>);
	}
	else {
		$target = $ARGV[0];
	}
}
else {
	print "Error! The number of parameters is wrong.\n";
	exit 1;
}

if($target < 1 || $target > 10) {
	print "Error! $target is not between 1 and 10.\n";
	exit 1;
}


my($lower, $upper) = (1, 10);
my $answer;

while(1) {
	&askGuess($lower, $upper);
	chomp($answer = <STDIN>);
	
	last if($answer == $target);

	if($lower <= $answer && $answer <= $upper){
		if($lower <= $target && $target < $answer) {
			$upper = $answer - 1;
		}
		else {
			$lower = $answer + 1;
		}
	}
}

print "Bingo!\n";



sub askGuess {
	my($lower, $upper) = @_;
	print "Please enter a number between $lower and $upper:";
}
