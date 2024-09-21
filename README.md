# C-Projects

These console projects are built while going through a rigorous training session in C. These projects implement several data structures using C and follows industry-standard practices in C-development. These projects are made as much clean, readable, portable, modular, fail-proof, and uncrashable as possible. The input taking mechanism is carefully designed to handle any unusual and illegal input to prevent crashing. Also, the files are checked for possible corruption before reading any data from it.

## Student Information Management

This project has three tables in its database - department table, student table, and grade table. These tables had unique primary keys, foreign keys and other restrains. Text files are used as the database. It provides mechanism for performing CRUD operations on these tables. It implements linked list to store and save the data.

## Bitmap management system

A bitmap is a custom sized continuous stream of bits where each single bit is handled separately. This project performs several operations such as create, destroy, set bit, reset bit, OR, AND, NOT, and string-parsing on bitmaps. 

## Ipv4 Packet Management

This project takes an Wireshark output file (.txt) as input where each frame contains Ipv4 datagram. Then it extracts the packet byte stream from the file and analyzes it. Then it separates and counts the packets with the same source and destination ips. It implements a linked list and a hash map to do so. 
