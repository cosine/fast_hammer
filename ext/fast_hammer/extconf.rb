# Copyright (c) 2009, Michael H. Buselli
# See LICENSE for details.

require 'mkmf'

extension_name = 'fast_hammer'
dir_config(extension_name)
$objs = [ "fast_hammer.#{$OBJEXT}" ]
$CFLAGS = '-O3'
create_makefile(extension_name)
