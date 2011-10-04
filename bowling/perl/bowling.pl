#!/usr/bin/perl
#use strict;
#use warnings;

{
    my %points = (
        '0' => 0, '1' => 1, '2' => 2, '3' => 3, '4' => 4,  '5' => 5,
        '6' => 6, '7' => 7, '8' => 8, '9' => 9, '/' => 10, 'X' => 10
    );

    while (<>) {
        my $score = 0;
        my @marks = split(' ', $_);
        for (my $frames = 10; $frames--; ) {
            my $points1 = $points{shift(@marks)};
            if ($points1 == 10) {
                $score += ($marks[1] eq '/') ?  20 : 10 + $points{$marks[0]} + $points{$marks[1]};
            } else {
                my $points2 = $points{shift(@marks)};
                $score += ($points2 == 10) ? 10 + $points{$marks[0]} : $points1 + $points2;
            }

        }
        print "$score\n";
    }
}

