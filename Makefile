all:
	cd Base/src; make
	cd Base/src; sudo make install
	cd Base/tests; make
	cd Base/tests/; ./test
	cd App/src; make
	cd App/src; sudo make install
	cd App/sample; make

clean:
	cd Base/src; make clean
	cd Base/tests; make clean
	cd App/src; make clean
	cd App/sample; make clean

