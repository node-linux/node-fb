NODE=-I/usr/include/node -I$(CURDIR)/node_modules/node-addon-api/ -I$(shell node -p "require('node-addon-api').include")
INCLUDE=$(NODE)
LIBS=

fb.node: clean
	g++ lib.cpp -shared -fPIC -o ./fb.node -Wall -Wextra $(LIBS) $(INCLUDE)

clean:
	rm -rf ./fb.node
