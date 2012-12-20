/*
 * hexstream.c
 * This file is part of base-16-streamer
 *
 * Copyright (C) 2012 - Triston J. Taylor
 *
 * base-16-streamer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * base-16-streamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with base-16-streamer. If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>

int sbyteseq(register char *a, register char *b) {
    if ( a == b ) return 1; if ( ! a || ! b ) return 0;   
    while ( *a != 0 && *b !=0 ) if (*a++ != *b++) return 0;
    return 1;
}

int garbage(register int item, register char *garbage) {
    /* ignore user supplied list of garbage */
    while ( *garbage != 0 ) if ( item == *garbage++ ) return 1;
    return 0;
}

int hexascii(register int data) { /* Did you notice? No memory access... */
    
    //    0-9 = 48...........57
    if ( data > 47 && data < 58 ) return (data - 48);
    
    //    A-F = 65...........70
    if ( data > 64 && data < 71 ) return (data - 55);
    
    //    a-f = 97...........102
    if ( data > 96 && data < 103 ) return (data - 87);

    return -1;
    
}

extern int main(int argc, char **argv)
{

    register int z = -1, i = 0, r, e;
    
    int quotient, remainder, trash = 0;
    
    const int b = 16;
    
    if (argc > 1)
        while (++i < argc) {
            if (sbyteseq(argv[i], "--deferred") || sbyteseq(argv[i], "-d")) {
                trash = ++i;
                continue;
            }
        }
        
parse:
    e = 0;
    if ( (quotient = getchar()) != EOF ) {
        if (trash && garbage(quotient, argv[trash])) goto parse;
        e++;
        if ( (remainder = getchar()) != EOF ) {
            while (trash && garbage(remainder, argv[trash])) {
                if ( (remainder = getchar()) != EOF ) continue;
            }
			e++;
			if ( (i = hexascii(quotient)) != z) {
				e++;
				if ( (r = hexascii(remainder)) != z) {
					i = (i * b + r); putchar(i); goto parse;
				} // 3
			} // 2  
		}
	} // 0
    
    return e;
    
    /* ERROR CODES
    
        0 = NO INPUT REMAINING
        1 = MISSING SECONDARY HEX CODE
        2 = INVALID PRIMARY HEX CODE
        3 = INVALID SECONDARY HEX CODE
        
    */

}

