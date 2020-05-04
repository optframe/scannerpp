all:
	echo "type 'make test' or 'make install'

test:
	cd tests && make

install:
	# this will require sudo to copy header to /usr/local/include/scannerpp/
	./install.sh && echo "Finished successfully"
	
test-install:
	cd tests && make test-install && echo "Finished successfully"

test-coverage:
	cd tests && make test-coverage && echo "Finished successfully"
