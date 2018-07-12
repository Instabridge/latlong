#include <ruby.h>
#include "zonelooker.h"

VALUE rb_zonelooker_lookup(VALUE self, VALUE lat, VALUE lon);

static const struct zl_table * table = NULL;

void Init_zonelooker()
{
	VALUE mod = rb_define_module("ZoneLooker");
	rb_define_singleton_method(mod, "lookup", rb_zonelooker_lookup, 2);
	table = zl_get_table(NULL);
	rb_define_const(mod, "DEGREE_PIXELS", INT2NUM(zl_deg_pixels(table)));
}

VALUE rb_zonelooker_lookup(VALUE self, VALUE lat, VALUE lon) {
	Check_Type(lat, T_FLOAT);
	Check_Type(lon, T_FLOAT);
	
	const char *zone = zl_lookup(table, RFLOAT_VALUE(lat), RFLOAT_VALUE(lon));
	if (zone == NULL) {
		return Qnil;
	} else {
		return rb_str_new_cstr(zone);
	}
}
