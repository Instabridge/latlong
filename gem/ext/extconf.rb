require 'mkmf'

$CFLAGS += ' -DZL_TABLE_PATH=\"table.h\" -std=gnu99 -Wno-gnu-flexible-array-initializer'

create_makefile 'zonelooker'
