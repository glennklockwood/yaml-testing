.PHONY: all

YAML_ROOT ?= /opt/local

CFLAGS = -I$(YAML_ROOT)/include
LDFLAGS = -L$(YAML_ROOT)/lib -lyaml

all: yaml yaml-doc
