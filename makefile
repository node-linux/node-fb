mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

NODE=-I/usr/include/node -I$(mkfile_dir)node_modules/node-addon-api/
INCLUDE=$(NODE)
LIBS=

fb.node: clean
	g++ lib.cpp -shared -fPIC -o ./fb.node -Wall -Wextra $(LIBS) $(INCLUDE)

clean:
	$(if $(wildcard ./*.node), rm *.node)
