# JSON Packer

**A JSON binary packer**.


_JSON packer_ is a small utility that reads sequences of JSON records and encode them in a binary format.
Internally, _JSON packer_ uses [cJSON](https://github.com/DaveGamble/cJSON) to parse the objects.

By default, the records are encrypted. The binary stream along with the encryption keys are written to an output stream.

The [ASN.1](https://www.itu.int/en/ITU-T/asn1/Pages/introduction.aspx) _DER_ encoding format is the program's default.
A _BSON_ enconder is expected to be released soon.


## Basic usage

In the simple use case, _JSON packer_ expects a _JSON_ file with a series of records:

	./jsonp <json-records-file> 

This will generate a binary encoded file and an encryption file in the same folder.


## Building

The program's can be compiled using _CMake_:

	mkdir build
	cd build
	cmake ..
	make
 
