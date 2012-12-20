/*
 * streamhex.c
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
#include <stdlib.h>
#include <string.h>

char
    output_format[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    *format_prefix = "",
    *format_suffix = "",
    *format_delimiter = "";

int format_extended = 0, streamed = 0, groupsize = 1, grouped=0;

void streamunit(char unit) {
    if (format_extended) {
        if (grouped == groupsize) {
            fflush(stdout);
            printf("%s", format_delimiter);
            grouped = 0;
        }
        printf("%s", format_prefix);
        printf(output_format, unit);
        printf("%s", format_suffix);
        grouped++;
    } else printf(output_format, unit);

}

extern int main(int argc, char **argv) {
    
    char
            
        *break_sequence = 0,
        *break_sequence_hold = 0;
        
    size_t break_sequence_len = 0;
    
    int
    
        encoding_width = 2,
        output_format_type = 1,
        break_sequence_type = 0,
        c = 0, i = 0, y = 0, new_line = 1;
        
    if (argc > 1) {
    
        while (++c < argc) {
            
            if (*argv[c] == '-') {
            
                if (argv[c][1] == '-') {
                
                    /* Long Options */
                    if (! strcmp(argv[c], "--hex-break")) {
hex_break:
                        break_sequence_type = 2;
                        break_sequence_len = strlen(argv[++c]);
                        break_sequence = argv[c];
                        continue;
                        
                    } else if (! strcmp(argv[c], "--raw-break")) {
raw_break:
                        break_sequence_type = 1;
                        break_sequence_len = strlen(argv[++c]);
                        break_sequence = argv[c];
                        continue;
                        
                    } else if (! strcmp(argv[c], "--lower-case")) {
lower_case:
                        output_format_type = 2;
                        continue;
                        
                    } else if (! strcmp(argv[c], "--encoding-width")) {
encoding_width:         /* encoding width must be 2, 4, or 8 */
                        if (argv[c+1] && sscanf(argv[++c], "%i", &encoding_width)) {
                            if (
                                encoding_width != 2,4,8 && encoding_width != 4
                                                    &&
                                             encoding_width != 8
                            ) {
                                fprintf(stderr, "%s%s\n", argv[0],
                                ": invalid encoding width: argument not 2, 4, or 8.");
                                exit(1);
                            }
                        } else {
                            fprintf(stderr, "%s%s%s\n", argv[0], ": error: ",
                            "unable to parse encoding width argument");
                            exit(1);                        
                        }
                        continue;
                        
                    } else if (! strcmp(argv[c], "--bypass-conversion")) {
bypass_conversion:
                        output_format_type = 3; --new_line;
                        continue;
                        
                    } else if (! strcmp(argv[c], "--prefix")) {
prefix_addition:
                        if (argv[c+1]) {
                            format_prefix = argv[++c]; format_extended = 1;
                        } else {
                            fprintf(stderr, "%s%s%s\n", argv[0], ": error: ",
                            "missing prefix argument");
                            exit(1);                                                    
                        }
                        continue;
                        
                    } else if (! strcmp(argv[c], "--suffix")) {
suffix_addition:
                        if (argv[c+1]) {
                            format_suffix = argv[++c]; format_extended = 1;
                        } else {
                            fprintf(stderr, "%s%s%s\n", argv[0], ": error: ",
                            "missing suffix argument");
                            exit(1);                                                    
                        }
                        continue;
                        
                    } else if (! strcmp(argv[c], "--delimiter")) {
delimiter_addition:
                        if (argv[c+1]) {
                            format_delimiter = argv[++c]; format_extended = 1;
                        } else {
                            fprintf(stderr, "%s%s%s\n", argv[0], ": error: ",
                            "missing delimiter argument");
                            exit(1);                                                    
                        }
                        continue;
                        
                    } else if (! strcmp(argv[c], "--chunk-break")) {
chunk_break:            /* argument param MUST parse as an integer using sscanf. */
                        if (
                                            ! argv[c+1]
                                                ||
                            ! sscanf(argv[++c], "%zu", &break_sequence_len)
                            
                        ) {
                            fprintf(stderr, "%s%s%s\n", argv[0], ": error: ",
                            "unable to parse chunk length argument");
                            exit(1);                        
                        }
                        break_sequence_type = 3;
                        continue;
                        
                    } else if (! strcmp(argv[c], "--grouping")) {
group_split:            /* argument param MUST parse as an integer using sscanf. */
                        if (
                                            ! argv[c+1]
                                                ||
                               ! sscanf(argv[++c], "%u", &groupsize)
                            
                        ) {
                            fprintf(stderr, "%s%s%s\n", argv[0], ": error: ",
                            "unable to parse group length argument");
                            exit(1);                        
                        }
                        format_extended = 1;
                        continue;
                        
                    }
                } else {
                
                    /* Short Options */
                    if (! strcmp(argv[c], "-r") ) goto raw_break;
                    if (! strcmp(argv[c], "-h") ) goto hex_break;
                    if (! strcmp(argv[c], "-e") ) goto encoding_width;                    
                    if (! strcmp(argv[c], "-l") ) goto lower_case;
                    if (! strcmp(argv[c], "-b") ) goto bypass_conversion;
                    if (! strcmp(argv[c], "-c") ) goto chunk_break;
                    if (! strcmp(argv[c], "-p") ) goto prefix_addition;
                    if (! strcmp(argv[c], "-s") ) goto suffix_addition;
                    if (! strcmp(argv[c], "-d") ) goto delimiter_addition;
                    if (! strcmp(argv[c], "-g") ) goto group_split;
                    
                    if (! strcmp(argv[c], "-n") ) {
                        --new_line; continue;
                    }
                    
                    fputs("Unrecognized short option!", stderr);
                    
                }
            } else if (*argv[c] == '+') {
            
                if (! strcmp(argv[c], "+n") ) {
                    ++new_line; continue;
                }
                
            } else {
                /* Not an option */
                fputs("Unrecognized option!", stderr);
                break;            
            }
        }
    }
    
    /* All options processed. Generate the output format */
    switch (output_format_type) {
        case 1: // uppercase hex
            sprintf(output_format, "%%0%iX", encoding_width);
        break;
        
        case 2: // lowercase hex
            sprintf(output_format, "%%0%ix", encoding_width);        
        break;
        
        case 3: // no conversion
            sprintf(output_format, "%%c");        
        break;
    }
        
    if (break_sequence_type) {
                
        if (break_sequence_type == 1) {
        
raw_scan:   /* Raw Type */
            i = 0; break_sequence_hold = calloc(break_sequence_len, 1);
            while ((c = getchar()) != EOF) {
                if ( c == break_sequence[i] ) {
                    break_sequence_hold[i] = c;
                    i++; if ( i == break_sequence_len ) break;
                } else {
                    if (i) {
                        for (y = 0; y < i; y++ ) {
                            streamunit(break_sequence_hold[y]);
                        }
                        free(break_sequence_hold);
                        break_sequence_hold = calloc(break_sequence_len, 1);
                        i = 0;
                    }
                    streamunit(c);
                }
            }
            
        } else if (break_sequence_type == 2) {
        
            /* Translation Type */
            
            /* Convert the hex ascii back to hex value and store the result
               in break_sequence, then goto raw_scan
            */
            break_sequence_hold = calloc((break_sequence_len / 2) + 1, 1); // padded
            
            while (i < (break_sequence_len/2)) {
                i += sscanf(break_sequence+i, "%hhx",
                    (unsigned char*) (break_sequence_hold + y++));
            }
                 
            break_sequence = strdup(break_sequence_hold);
            free(break_sequence_hold);
            break_sequence_len = y;
            
            goto raw_scan;
            
        } else if (break_sequence_type == 3) {
            /* chunk type */
            while ( (break_sequence_len--) && (c = getchar()) != EOF) streamunit(c);

        }
        
        /* Its mostly pointless to free these pointers as they are tied to
           the life of this program. Doing it anyway...
        */
        
        free(break_sequence_hold); // note this may be a null pointer..
        if (break_sequence_type == 2) free(break_sequence);
        
    } else {
    
        while ( (c = getchar()) != EOF ) streamunit(c);
    
    }
    
    if (new_line > 0) puts("");
        
}

